#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../include/irc.hpp"

#define DEFAULT_IRC_PORT 6667

class Server
{
private:
    int sockfd;
    int port;
    struct sockaddr_in serv_addr;
    int client_fd;
    
public:
    Server();
    ~Server();

    //init  
    int InitSocket();
    int bindSocket();

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