#include "Server.hpp"

/// @brief send a 403 error to the client : 403 = No such channel
/// @param client client info
/// @param channelName name of the channel
void Server::send_error_403(Client &client, std::string &channelName)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 403 " + client.get_nickname() + " " + channelName + " :No such channel\r\n";
    if (send(client.get_socket(), rq.c_str(), rq.size(), 0) < 0)
        fatal("Error on send");
}

/// @brief send a 461 error to the client : 461 = Not enough parameters
/// @param client 
void Server::send_error_461(Client &client)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 461 " + client.get_nickname() + " Error :Not enough parameters\r\n";
    if (send(client.get_socket(), rq.c_str(), rq.size(), 0) < 0)
        fatal("Error on send");
}

/// @brief send a 482 error to the client : 482 = You're not channel operator
/// @param client
/// @param channelName
void Server::send_error_482(Client &client, std::string &channelName)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 482 " + client.get_nickname() + " " + channelName + " :You're not channel operator\r\n";
    if (send(client.get_socket(), rq.c_str(), rq.size(), 0) < 0)
        fatal("Error on send");
}

/// @brief Client is not on that channel
/// @param client 
/// @param channelName 
void Server::send_error_442(Client &client, std::string &channelName)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 442 " + client.get_nickname() + " " + channelName + " :You're not on that channel\r\n";
    if (send(client.get_socket(), rq.c_str(), rq.size(), 0) < 0)
        fatal("Error on send");
}

/// @brief  No such nick
/// @param client 
/// @param targetName 
void Server::send_error_401(Client &client, std::string &targetName)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 401 " + client.get_nickname() + " " + targetName + " :No such nick\r\n";
    if (send(client.get_socket(), rq.c_str(), rq.size(), 0) < 0)
        fatal("Error on send");
}