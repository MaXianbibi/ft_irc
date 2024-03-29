/**
 * @file Client.cpp
 * @author Justin Morneau
 * @brief class client
 * @version 0.1
 * @date 2024-02-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Client.hpp"

Client::Client()
{
    _first_time_connected = true;

    memset(&mode, 0, sizeof(mode));

    _nickname = "";
    _username = "";
    _realname = "";
    _hostname = "";
    _socket = 0;
    _ip = "";
    _port = 0;
    _unused = "";

    join_channel = "";

    authentified = false;
    has_joinded_channel = false;
}

Client::~Client()
{
}

void Client::clear_command()
{
    buffer_command.clear();
}

void Client::clear_command_list()
{
    int size = command_list.size();
    for (int i = 0; i < size; i++)
    {
        command_list[i].clear();
    }
}

void Client::clear_commands_parsed()
{
    _commands_parsed.clear();
}

/// @brief parse the buffer_command into command_list
/// En gros, on prend la valeur raw et on la divise en plusieurs commandes non traitées
void Client::parse_command()
{
    if (buffer_command.empty())
        return;
    size_t pos = 0;

    while ((pos = buffer_command.find("\r\n")) != std::string::npos)
    {
        std::string line = buffer_command.substr(0, pos);
        buffer_command.erase(0, pos + 2);
        command_list.push_back(line);
    }
    buffer_command.clear();
}

/// @brief parse the command_list into _commands_parsed
/// on traite les commandes
void Client::parse_list_command()
{
    if (command_list.empty())
        return;

    for (std::vector<std::string>::iterator it = command_list.begin(); it != command_list.end(); ++it)
    {
        std::string line = *it;
        std::string command;
        std::vector<std::string> params;
        size_t pos = line.find(" ");

        command = (pos != std::string::npos) ? line.substr(0, pos) : line;
        line = (pos != std::string::npos) ? line.substr(pos + 1) : "";
        while (!line.empty() && (pos = line.find(" ")) != std::string::npos)
        {
            if (line[0] == ':')
            {
                break;
            }
            params.push_back(line.substr(0, pos));
            line = line.substr(pos + 1);
        }
        if (!line.empty())
            params.push_back(line[0] == ':' ? line.substr(1) : line);
    
        commands cmd;
        cmd.command = command;
        cmd.params = params;
        if (!command.empty())
            _commands_parsed.push_back(cmd);
    }
    clear_command_list();
}


/// @brief  send a message to the client
/// @param message 
void Client::sendMessage(std::string message)
{
    if (send(_socket, message.c_str(), message.size(), 0) == -1)
        perror("ERROR on send");
}


/// @brief is he operator of the channel
/// @return 
bool Client::isOperator()
{
    return mode.o;
}

// getter
std::string Client::get_nickname() const { return _nickname; }
std::string Client::get_username() const { return _username; }
std::string Client::get_realname() const { return _realname; }
std::string Client::get_hostname() const { return _hostname; }
std::string Client::get_command() const { return buffer_command; }
std::vector<std::string> Client::get_command_list() const { return command_list; }
int Client::get_socket() const { return _socket; }
std::string Client::get_ip() const { return _ip; }
int Client::get_port() const { return _port; }
std::vector<commands> Client::get_commands_parsed() const { return _commands_parsed; }
std::string Client::get_unused() const { return _unused; }
bool Client::get_first_time_connected() const { return _first_time_connected; }
std::string Client::get_join_channel() const { return join_channel; }
bool Client::get_authentified() const { return authentified; }
bool Client::is_joinned_channel() const { return has_joinded_channel; }

// setter
void Client::set_nickname(std::string nickname) { _nickname = nickname; }
void Client::set_username(std::string username) { _username = username; }
void Client::set_realname(std::string realname) { _realname = realname; }
void Client::set_hostname(std::string hostname) { _hostname = hostname; }
void Client::set_command(std::string command) { buffer_command = command; }
void Client::set_socket(int socket) { _socket = socket; }
void Client::set_ip(std::string ip) { _ip = ip; }
void Client::set_port(int port) { _port = port; }
void Client::set_join_channel(std::string channel) { join_channel = channel; }

void Client::set_unused(std::string unused) { _unused = unused; }
void Client::set_first_time_connected(bool first_time_connected) { _first_time_connected = first_time_connected; }
void Client::set_authentified(bool authentified) { this->authentified = authentified; }
void Client::set_is_joinned_channel(bool has_joinded_channel) { this->has_joinded_channel = has_joinded_channel; }