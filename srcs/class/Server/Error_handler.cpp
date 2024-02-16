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

/// @brief  not enough parameters
/// @param client 
/// @param commands_name 
void Server::send_error_461(Client &client, std::string commands_name)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 461 " + client.get_nickname() + " " + commands_name + " :Not enough parameters\r\n";
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

/// @brief  Invite only channel
/// @param client 
/// @param channelName 
void Server::send_error_473(Client &client, std::string &channelName)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 473 " + client.get_nickname() + " " + channelName + " :Invite only channel\r\n";
    client.sendMessage(rq);
}

/// @brief Cannot join channel (+l)
/// @param client 
/// @param channelName 
void Server::send_error_471(Client &client, std::string &channelName)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 471 " + client.get_nickname() + " " + channelName + " :Cannot join channel (+l)\r\n";
    client.sendMessage(rq);
}

/// @brief incorrect password
/// @param client 
void Server::send_error_451(Client &client)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 451 " + client.get_nickname() + " :Incorrect password\r\n";
    client.sendMessage(rq);
}

void Server::send_error_475(Client &client, std::string &channelName)
{
    std::string server_name = SERVER_NAME;
    std::string rq = ":" + server_name + " 475 " + client.get_nickname() + " " + channelName + " :Bad channel key\r\n";
    client.sendMessage(rq);
}