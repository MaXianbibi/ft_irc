#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../include/irc.hpp"

# include "../Client/Client.hpp"

#define DEFAULT_IRC_PORT 6667
#define DEFAULT_BACKLOG 50

#define BUF_SIZE 1024
#define SERVER_NAME "KanyesFanClub"
#define DEFAULT_TOPIC "What do you think of Vulture ?"

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
    std::set<Client *> inviteList;


    std::string topic;
    std::string password;
    unsigned int limit;
    std::string name;

    // func
    void kickClient(Client &target);
    void broadcast(std::string message);
    Client* get_client_by_nick(std::string nickname);
    bool is_client_in_channel(Client &client);

    // client gestion
    void remove_client(Client &client);
    void add_client(Client &client);
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
    bool is_channel_by_name(std::string &channelName);
    void applyChannelMode(s_channel &channel, Client &client, const std::string &mode, const std::string &param = "");

    // over one client ( u need to use acceptSocket() before
    int loopSocket();
    void newClient();
    void newMessage(int &i);


    // commands
    // PRIVMSG #channel :Your message here\r\n
    void NickCommand(Client &client, std::vector<commands>::iterator &it);
    void UserCommand(Client &client, std::vector<commands>::iterator &it);
    void QuitCommand(int &i);
    void PingCommand(std::vector<commands>::iterator &it, int &i);
    void FirstTimeConnectionMsg(Client &client, int &i);
    void CapCommand(std::vector<commands>::iterator &it, int &i);
    void msgToEveryClient(int &i, char buffer[1024], int n);
    void ModeCommand(std::vector<commands>::iterator &it, Client &client);
    void PrivmsgCommand(std::vector<commands>::iterator &it, Client &client);
    void joinCommand(std::vector<commands>::iterator &it, Client &client);
    void WhoisCommand(std::vector<commands>::iterator &it, Client &client, int &i);
    void TopicCommand(std::vector<commands>::iterator &it, Client &client);
    void KickCommand(std::vector<commands>::iterator &it, Client &client);
    void InviteCommand(std::vector<commands>::iterator &it, Client &client);


    // send error
    void send_error_401(Client &client, std::string &targetName);  // No such nick
    void send_error_403(Client &client, std::string &channelName); // No such channel
    void send_error_442(Client &client, std::string &channelName); // You're not on that channel 
    void send_error_461(Client &client);                           // Not enough parameters
    void send_error_482(Client &client, std::string &channelName); // You're not channel operator



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
    Client *get_client_by_nick_ptr(std::string nickname);

    // Setter
    void set_sockfd(int sockfd);
    void set_port(int port);
};


#endif