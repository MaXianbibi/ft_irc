/**
 * @file Client.hpp
 * @author Justin Morneau
 * @brief Class client
 * @version 0.1
 * @date 2024-02-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

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
        std::string _unused;        // USER param 2
        std::string _realname;      // USER param 3


        // HOSTNAME
        std::string _hostname;     // ?? jsp si on va l'utiliser


    std::string join_channel;
    // command & command list
    std::string buffer_command;
    std::vector<std::string> command_list;
    std::vector<commands> _commands_parsed;

    bool _first_time_connected;

    bool authentified;
    bool has_joinded_channel;



    public:
    Client();
    ~Client();

    // tools
    void clear_command();
    void clear_command_list();
    void clear_commands_parsed();
    void parse_command();
    void parse_list_command();
    void sendMessage(std::string message);
    bool isOperator();
    bool get_first_time_connected() const;

    struct 
    {
        bool o;
    } mode;

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
    std::string get_unused() const;
    std::vector<commands> get_commands_parsed() const;
    std::string get_join_channel() const;
    bool get_authentified() const;

    bool is_joinned_channel() const;

    // setter
    void set_nickname(std::string nickname);
    void set_username(std::string username);
    void set_realname(std::string realname);
    void set_hostname(std::string hostname);
    void set_command(std::string command);
    void set_socket(int socket);
    void set_ip(std::string ip);
    void set_port(int port);
    void set_unused(std::string unused);
    void set_first_time_connected(bool first_time_connected);
    void set_join_channel(std::string channel);
    void set_authentified(bool authentified);
    void set_is_joinned_channel(bool has_joinded_channel);

};

#endif