#include "Server.hpp"

Server::Server()
{
    std::cout << "Server created" << std::endl;
    // set the port to 0, if the user doesn't specify a port, the default port will be used in the bindSocket function
    port = 0;
}

Server::~Server() {}

/// @brief init the socket & configure it
/// @param
/// @return
int Server::InitSocket()
{
    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // configure the socket
    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (sockfd < 0)
        fatal("Error opening socket");

    // set the socket to non-blocking
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0)
        fatal("Error getting socket flags");

    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
        fatal("Error setting socket to non-blocking");

    return SUCCESS;
}

/// @brief bind the socket to the port & the address (INADDR_ANY) ( INADDR_ANY = listen to all the interfaces)
/// @param
/// @return
int Server::bindSocket()
{
    memset(&serv_addr, 0, sizeof(serv_addr));
    if (port == 0)
        port = DEFAULT_IRC_PORT;

    serv_addr.sin_family = AF_INET;         // ipv4
    serv_addr.sin_addr.s_addr = INADDR_ANY; // any address
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        fatal("Error on binding");
    return SUCCESS;
}

/// @brief listen to the socket
/// @return
int Server::listenSocket()
{
    if (listen(sockfd, DEFAULT_BACKLOG) < 0)
        fatal("Error on listening");

    return SUCCESS;
}

/// @brief Accept the socket
/// @return
int Server::acceptSocket()
{
    memset(&client_addr, 0, sizeof(client_addr));
    addr_size = sizeof(client_addr);
    client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
    if (client_fd < 0)
        fatal("Error on accept");
    return SUCCESS;
}

/// @brief Receive the socket and print the message
/// @return FAILURE if the connection is closed
int Server::rcvSocket()
{
    char buffer[BUF_SIZE] = {0};
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received < 0)
        fatal("Error on recv");
    else if (bytes_received == 0)
    {
        std::cout << "Connection closed" << std::endl;
        return FAILURE;
    }
    else
    {
        if (bytes_received == 1)
            sendSocket("pong\n");
        else
            std::cout << "Received: " << buffer << std::endl;
    }
    return SUCCESS;
}

/// @brief loop while the connection is open
/// @return
int Server::loopSocket()
{
    while (rcvSocket() != FAILURE)
        ;
    return SUCCESS;
}

int Server::sendSocket(const char *message)
{
    int bytes_sent = send(client_fd, message, strlen(message), 0);
    if (bytes_sent < 0)
        fatal("Error on send");
    return SUCCESS;
}

int Server::closeSocket()
{
    close(client_fd);
    close(sockfd);
    return SUCCESS;
}

/// @brief init les differents elements pour le select loop
/// @return
int Server::selectInit()
{
    // init sockets for select
    InitSocket();
    bindSocket();
    listenSocket();

    // clear the master and temp sets
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // add the sockfd to the master set
    FD_SET(sockfd, &master);
    fdmax = sockfd;

    return SUCCESS;
}

/// @brief loop over cient while the connection is open
/// @return
int Server::selectLoop()
{

    while (1)
    {
        read_fds = master; // Copy it
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
            fatal("ERROR on select");
        // Run through the existing connections looking for data to read
        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            { // We got one!!
                if (i == sockfd)
                    newClient();
                else
                    newMessage(i);
            }
        }
    }
    return SUCCESS;
}

/// @brief new message from a client (i)
/// @brief parse message and applieds commands in consequence
/// @param i (client socket fd)
void Server::newMessage(int &i)
{
    int n;
    char buffer[BUF_SIZE];
    // Handle data from a client
    if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0)
    {
        // Got error or connection closed by client
        if (n == 0)
        {
            // Connection closed
            printf("selectserver: socket %d hung up\n", i);
        }
        else
        {
            perror("ERROR on recv");
            // ? fatal("ERROR on recv");
        }
        close(i);           // Bye!
        FD_CLR(i, &master); // Remove from master set
        clients.erase(i);   // Remove from clients map
    }
    else
    {

        if (n < BUF_SIZE)
            buffer[n] = '\0';

        Client &client = clients.at(i);

        Log(buffer);
        std::string string_buffer(buffer);

        client.set_command(string_buffer);
        client.parse_command();
        client.parse_list_command();

        std::vector<commands> commands_parsed = clients.at(i).get_commands_parsed();
        client.get_commands_parsed().clear();

        
        std::vector<commands>::iterator it = commands_parsed.begin();
        for (; it != commands_parsed.end(); ++it)
        {
            if (it->command == "NICK")
                client.set_nickname(it->params[0]);
            else if (it->command == "USER")
            {
                client.set_username(it->params[0]);
                client.set_mode(it->params[1]);
                client.set_unused(it->params[2]);
                client.set_realname(it->params[3]); 
            }
            else if (it->command == "QUIT")
            {
                close(i);           // Bye!
                FD_CLR(i, &master); // Remove from master set
                clients.erase(i);   // Remove from clients map
                return;
            }
        }
        if (client.get_first_time_connected() == true)
        {
            std::string serverName(SERVER_NAME);
            std::string msgWelcome(":" + serverName + " 001 " + client.get_nickname() + " :Welcome to the IRC Network, " + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + "\r\n");
            send(i, msgWelcome.c_str(), msgWelcome.size(), 0);
            msgWelcome.clear();
            msgWelcome = ":" + serverName + " 002 " + client.get_nickname() + " :Your host is " + serverName + ", running version 0.1\r\n";
            send(i, msgWelcome.c_str(), msgWelcome.size(), 0);
            // :KanyesFanClub 003 votrePseudo :This server was created dateDeCreation
            msgWelcome.clear();
            msgWelcome = ":" + serverName + " 003 " + client.get_nickname() + " :This server was created " + __CREATION_DATE__ + " " + __CREATION_TIME__ + "\r\n";
            send(i, msgWelcome.c_str(), msgWelcome.size(), 0);
            // :KanyesFanClub 004 votrePseudo KanyesFanClub versionServeur userModes channelModes
            msgWelcome.clear();
            msgWelcome = ":" + serverName + " 004 " + client.get_nickname() + " " + serverName + " 0.1 " + "i" + " " + "n" + "\r\n";
            send(i, msgWelcome.c_str(), msgWelcome.size(), 0);

            client.set_first_time_connected(false);
        }    
        // msgToEveryClient(i, buffer, n);
    }
}


/// @brief affiche les log du serv 
/// @param buffer le message a afficher
void Server::Log(char buffer[1024])
{
    std::cout << "Received: " << buffer << std::endl;
}

/// @brief send a message to every client except the sender and the server (ourself)
/// @param i (client fd)
/// @param buffer (message to send)
/// @param n (size of the message)
void Server::msgToEveryClient(int &i, char buffer[1024], int n)
{
    {
        // We got some data from a client
        for (int j = 0; j <= fdmax; j++)
        {
            // Send to everyone!
            if (FD_ISSET(j, &master))
            {
                // Except the listener and ourselves
                if (j != sockfd && j != i)
                {
                    if (send(j, buffer, n, 0) == -1)
                    {
                        perror("ERROR on send");
                        // ? fatal("ERROR on recv");
                    }
                }
            }
        }
    }
}

/// @brief add a new client to the master set
/// @return
void Server::newClient()
{
    int newsockfd;
    socklen_t clilen;
    struct sockaddr_in cli_addr;

    // Handle new connections
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
    {
        perror("ERROR on accept");
    }
    else
    {
        int flags = fcntl(newsockfd, F_GETFL, 0);
        if (flags < 0)
        {
            perror("ERROR on fcntl(F_GETFL)");
            close(newsockfd);
        }
        else
        {
            if (fcntl(newsockfd, F_SETFL, flags | O_NONBLOCK) < 0)
            {
                perror("ERROR on fcntl(F_SETFL)");
                close(newsockfd);
            }
            else
            {
                FD_SET(newsockfd, &master);
                if (newsockfd > fdmax)
                {
                    fdmax = newsockfd;
                }

                clients.insert(std::make_pair(newsockfd, Client()));

                clients[newsockfd].set_socket(newsockfd);
                clients[newsockfd].set_ip(inet_ntoa(cli_addr.sin_addr));
                clients[newsockfd].set_port(ntohs(cli_addr.sin_port));
                printf("selectserver: new connection from %s on socket %d\n",
                       inet_ntoa(cli_addr.sin_addr), newsockfd);
            }
        }
    }
}

/// @brief exit the program with a message
/// @param message exit message
/// @return void
void Server::fatal(const char *message)
{
    perror(message);
    exit(1);
}

// getter
int Server::get_sockfd() const { return sockfd; }
int Server::get_port() const { return port; }
int Server::get_client_fd() const { return client_fd; }
std::map<int, Client> Server::get_clients() const { return clients; }

// Setter
void Server::set_sockfd(int sockfd) { this->sockfd = sockfd; }
void Server::set_port(int port) { this->port = port; }
