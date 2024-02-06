#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../include/irc.hpp"

#define DEFAULT_IRC_PORT 6667
#define DEFAULT_BACKLOG 50

#define BUF_SIZE 1024

class Server
{
private:
    // socket
    int sockfd;
    int port;
    struct sockaddr_in serv_addr;

    // unique client
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addr_size;

    // select
    fd_set master;   
    fd_set read_fds;
    int fdmax;


public:
    Server();
    ~Server();

    //init socket
    int InitSocket();
    int bindSocket();
    int listenSocket();

    // tools
    int acceptSocket();
    int rcvSocket();
    int sendSocket(const char *message);
    int closeSocket();

    // over one client ( u need to use acceptSocket() before
    int loopSocket();
    void newClient();
    void newMessage(int &i);

    // over multiple clients
    int selectInit();
    int selectLoop();


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