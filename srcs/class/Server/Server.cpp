#include "Server.hpp"

Server::Server()
{
    std::cout << "Server created" << std::endl;
    port = 0;
}

Server::~Server()
{
}

/// @brief init the socket & configure it 
/// @param void
/// @return  void
int Server::InitSocket()
{
    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // configure the socket  
    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (sockfd < 0)
        fatal("Error opening socket");
}

/// @brief bind the socket to the port & the address (INADDR_ANY) ( INADDR_ANY = listen to all the interfaces)
/// @param void
/// @return  void
int Server::bindSocket()
{
    bzero((char *)&serv_addr, sizeof(serv_addr));
    if (port == 0)
        port = DEFAULT_IRC_PORT;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        fatal("Error on binding");
}

// fatal
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

