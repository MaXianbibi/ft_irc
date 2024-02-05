#include "Server.hpp"

Server::Server()
{
    std::cout << "Server created" << std::endl;
    // set the port to 0, if the user doesn't specify a port, the default port will be used in the bindSocket function
    port = 0;
}

Server::~Server(){}

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

    serv_addr.sin_family = AF_INET; // ipv4
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
    char buffer[1024] = { 0 };
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);    
    if (bytes_received < 0)
        fatal("Error on recv");
    else if (bytes_received == 0){
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
    while (rcvSocket() != FAILURE);
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


/// @brief exit the program with a message
/// @param message exit message
/// @return void
void Server::fatal(const char *message)
{
    perror(message);
    exit(1);
}

// getter
int Server::get_sockfd() const{return sockfd;}
int Server::get_port() const{return port;}
int Server::get_client_fd() const{return client_fd;}

// Setter
void Server::set_sockfd(int sockfd){this->sockfd = sockfd;}
void Server::set_port(int port){this->port = port;}

