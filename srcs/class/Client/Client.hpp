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
    int _socket;
    std::string _ip;
    int _port;



    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;

    std::string buffer_command;
    std::vector<std::string> command_list;

    std::vector<commands> _commands_parsed;

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

    std::vector<commands> get_commands_parsed() const;


    // setter
    void set_nickname(std::string nickname);
    void set_username(std::string username);
    void set_realname(std::string realname);
    void set_hostname(std::string hostname);
    void set_command(std::string command);
    void set_socket(int socket);
    void set_ip(std::string ip);
    void set_port(int port);


};

#endif