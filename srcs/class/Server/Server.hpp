#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../include/irc.hpp"

#define DEFAULT_IRC_PORT 6667
#define DEFAULT_BACKLOG 50

class Server
{
private:

    int sockfd;
    int port;
    struct sockaddr_in serv_addr;


    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addr_size;
    
public:
    Server();
    ~Server();

    //init  
    int InitSocket();
    int bindSocket();
    int listenSocket();
    int acceptSocket();
    int rcvSocket();
    int loopSocket();
    int sendSocket(const char *message);
    int closeSocket();

    // fatal
    void fatal(const char *message);

    // getter 
    int get_sockfd() const;
    int get_port() const;
    int get_client_fd() const;
    // Setter
    void set_sockfd(int sockfd);
    void set_port(int port);

};


#endif