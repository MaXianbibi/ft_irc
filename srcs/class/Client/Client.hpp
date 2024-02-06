#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../../include/irc.hpp"



struct commands {
    std::string command;
    std::vector<std::string> params;
}; typedef struct commands commands;

class Client
{
private:

    // socket & connection
    int _socket;
    std::string _ip;
    int _port;

    // user info
        // NICK`
        std::string _nickname;      // param 0

        // USER
        std::string _username;      // USER param 0
        std::string _mode;          // USER param 1
        std::string _unused;        // USER param 2
        std::string _realname;      // USER param 3

        // HOSTNAME
        std::string _hostname;     // ?? jsp si on va l'utiliser



    // command & command list
    std::string buffer_command;
    std::vector<std::string> command_list;
    std::vector<commands> _commands_parsed;

    bool _first_time_connected;

    public:
    Client();
    ~Client();

    // tools
    void clear_command();
    void clear_command_list();
    void parse_command();
    void parse_list_command();

    // getter
    std::string get_nickname() const;
    std::string get_username() const;
    std::string get_realname() const;
    std::string get_hostname() const;
    std::string get_command() const;
    std::vector<std::string> get_command_list() const;
    int get_socket() const;
    std::string get_ip() const;
    int get_port() const;
    std::string get_mode() const;
    std::string get_unused() const;
    std::vector<commands> get_commands_parsed() const;

    bool get_first_time_connected() const;

    // setter
    void set_nickname(std::string nickname);
    void set_username(std::string username);
    void set_realname(std::string realname);
    void set_hostname(std::string hostname);
    void set_command(std::string command);
    void set_socket(int socket);
    void set_ip(std::string ip);
    void set_port(int port);
    void set_mode(std::string mode);
    void set_unused(std::string unused);
    void set_first_time_connected(bool first_time_connected);


};

#endif