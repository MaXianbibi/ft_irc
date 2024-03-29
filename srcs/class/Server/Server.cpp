/**
 * @file Server.cpp
 * @author Justin Morneau
 * @brief Serveur IRC
 * @version 0.1
 * @date 2024-02-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Server.hpp"

Server::Server()
{
    std::cout << "Server created" << std::endl;
    // set the port to 0, if the user doesn't specify a port, the default port will be used in the bindSocket function
    port = 0;
    keypass = "1234";

    logfile.open("logfile.log");

    if (!logfile.is_open())
        fatal("Error opening logfile");

    coutbuf = std::cout.rdbuf(); 
    std::cout.rdbuf(logfile.rdbuf()); 



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
    int flags = fcntl(sockfd, F_SETFL, O_NONBLOCK);

    if (flags < 0)
        fatal("Error getting socket flags");

    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
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


    std::cout << "Server started on port " << port << std::endl;



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
        FD_CLR(i, &master); // Remove from master set
        clients_by_nick.erase(clients.at(i).get_nickname());
        clients.erase(i); // Remove from clients map
        close(i);         // Bye!
    }
    else
    {
        if (n < BUF_SIZE)
            buffer[n] = '\0';

        Log(buffer);

        Client &client = clients.at(i);
        if (buffer[n - 1] && buffer[n - 1] == '\n')
        {
            if (!client.half_buffer.empty())
            {
                client.half_buffer += buffer;
                memset(buffer, 0, sizeof(buffer));

                strcpy(buffer, std::string(client.half_buffer + buffer).c_str());
                client.half_buffer.clear();
            }
        }
        else
        {
            client.half_buffer += buffer;
            return ;
        }
        std::string string_buffer(buffer);
        client.set_command(string_buffer);
        client.parse_command();
        client.parse_list_command();
        std::vector<commands> commands_parsed = clients.at(i).get_commands_parsed();
        client.clear_commands_parsed();

        std::vector<commands>::iterator it = commands_parsed.begin();
        for (; it != commands_parsed.end(); ++it)
        {
            std::cout << "Commands : " << it->command << " [" << i << "]" << std::endl;
            for (std::vector<std::string>::iterator it_param = it->params.begin(); it_param != it->params.end(); ++it_param)
            {
                std::cout << "Params : " << *it_param << std::endl;
            }
            try {
                if (it->command == "NICK")
                    NickCommand(client, it);
                else if (it->command == "USER")
                    UserCommand(client, it);
                else if (it->command == "QUIT")
                    return QuitCommand(i);
                else if (it->command == "PING")
                    PingCommand(it, i);
                else if (it->command == "CAP")
                    CapCommand(it, i);
                else if (it->command == "MODE")
                    ModeCommand(it, client);
                else if (it->command == "WHOIS")
                    WhoisCommand(it, client, i);
                else if (it->command == "JOIN")
                    joinCommand(it, client);
                else if (it->command == "PRIVMSG") 
                    PrivmsgCommand(it, client);
                else if (it->command == "KICK")
                    KickCommand(it, client);
                else if (it->command == "TOPIC")
                    TopicCommand(it, client);
                else if (it->command == "INVITE")
                    InviteCommand(it, client);
                else if (it->command == "PART")
                    channels.at(client.get_join_channel()).kickClient(client);
                else if (it->command == "PASS")
                {
                    PassCommand(it, client);
                    if (client.get_authentified() == false)
                        return ;
                }
            else
                client.sendMessage("421 " + client.get_nickname() + " :Unknown command\r\n");
            }
            catch(...) {
                std::cerr << "Command failed" << std::endl;
            }


        }
        if (client.get_first_time_connected() == true)
            FirstTimeConnectionMsg(client, i);
        // msgToEveryClient(i, buffer, n);
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
        int flags = fcntl(newsockfd, F_SETFL, O_NONBLOCK);
        if (flags < 0)
        {
            perror("ERROR on fcntl(F_GETFL)");
            close(newsockfd);
        }
        else
        {
            if (fcntl(newsockfd, F_SETFL, O_NONBLOCK) < 0)
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

void Server::close_log_file()
{
    std::cout.rdbuf(coutbuf);
    logfile.close();
}

/// @brief exit the program with a message
/// @param message exit message
/// @return void
void Server::fatal(const char *message)
{
    close_log_file();


    perror(message);
    exit(1);
}


/// @brief Retourne une référence à l'objet Client trouvé.
/// @param nickname 
/// @return 
Client &Server::get_client_by_nick(std::string nickname)
{
    std::map<std::string, Client *>::iterator it = clients_by_nick.find(nickname);
    if (it != clients_by_nick.end())
    {
        return *(it->second); // Retourne une référence à l'objet Client trouvé.
    }
    else
    {
        throw std::runtime_error("Client not found");
    }
}


/// @brief get a client by nickname on the server
/// @param nickname 
/// @return client ptr if found, NULL otherwise
Client *Server::get_client_by_nick_ptr(std::string nickname)
{
    std::map<std::string, Client *>::iterator it = clients_by_nick.find(nickname);
    if (it != clients_by_nick.end())
        return it->second;
    return NULL;
}

void Server::disconnect_client(Client &client)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " QUIT :Client disconnected\r\n";
    if (send(client.get_socket(), rq.c_str(), rq.size(), 0) < 0)
        fatal("Error on send");
    FD_CLR(client.get_socket(), &master); // Remove from master set
    clients_by_nick.erase(client.get_nickname());
    clients.erase(client.get_socket()); // Remove from clients map
    close(client.get_socket());         // Bye!
}

// getter
int Server::get_sockfd() const { return sockfd; }
int Server::get_port() const { return port; }
int Server::get_client_fd() const { return client_fd; }
std::map<int, Client> Server::get_clients() const { return clients; }
std::map<std::string, Client *> Server::get_clients_by_nick() const { return clients_by_nick; }
std::string Server::get_keypass() const { return keypass; }

// Setter
void Server::set_sockfd(int sockfd) { this->sockfd = sockfd; }
void Server::set_port(int port) { this->port = port; }
void Server::set_keypass(std::string keypass) { this->keypass = keypass; }
