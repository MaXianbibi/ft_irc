#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../include/irc.hpp"

# include "../Client/Client.hpp"

#define DEFAULT_IRC_PORT 6667
#define DEFAULT_BACKLOG 50

#define BUF_SIZE 1024
#define SERVER_NAME "KanyesFanClub"

#define __CREATION_DATE__ "2024-02-06"
#define __CREATION_TIME__ "12:00:00"


#define CLIENT client.getNick() + "!" + client.getUser() + "@" + client.getIP()

struct s_channel
{
    struct 
    {
        bool i;
        bool t;
        bool k;
        bool o;
        bool l;
    } mode;
    

    std::vector<Client*> clients;
    std::string topic;
    std::string password;


    // func
    void kickClient(Client &target);
    void broadcast(std::string message);
    Client* get_client_by_nick(std::string nickname);
    
}; typedef struct s_channel s_channel;

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

    // clients
    std::map<int, Client> clients;
    std::map<std::string, Client* > clients_by_nick;

    // channels
    std::map<std::string, s_channel > channels;

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
    void Log(char buffer[1024]);


    // over one client ( u need to use acceptSocket() before
    int loopSocket();
    void newClient();
    void newMessage(int &i);

    void KickCommand(std::vector<commands>::iterator &it, Client &client);

    // commands
    // PRIVMSG #channel :Your message here\r\n
    void NickCommand(Client &client, std::vector<commands>::iterator &it);
    void UserCommand(Client &client, std::vector<commands>::iterator &it);
    void QuitCommand(int &i);
    void PingCommand(std::vector<commands>::iterator &it, int &i);
    void FirstTimeConnectionMsg(Client &client, int &i);
    void CapCommand(std::vector<commands>::iterator &it, int &i);
    void msgToEveryClient(int &i, char buffer[1024], int n);
    void ModeCommand(std::vector<commands>::iterator &it, int &i);
    void PrivmsgCommand(std::vector<commands>::iterator &it, Client &client);
    void joinCommand(std::vector<commands>::iterator &it, Client &client);
    void WhoisCommand(std::vector<commands>::iterator &it, Client &client, int &i);

    // over multiple clients
    int selectInit();
    int selectLoop();

    // fatal
    void fatal(const char *message);

    // getter
    int get_sockfd() const;
    int get_port() const;
    int get_client_fd() const;
    std::map<int, Client> get_clients() const;
    std::map<std::string, Client*> get_clients_by_nick() const;
    Client& get_client_by_nick(std::string nickname);

    // Setter
    void set_sockfd(int sockfd);
    void set_port(int port);
};


#endif