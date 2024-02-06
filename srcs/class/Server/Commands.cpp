#include "Server.hpp"


/// @brief Change ou init le nickname du client
/// @param client info client
/// @param it iterator of the command
void Server::NickCommand(Client &client, std::vector<commands>::iterator &it)
{
    client.set_nickname(it->params[0]);
}

/// @brief  Rempli les informations du client, nickname, username, mode, unused, realname
/// @param client client info
/// @param it command info
void Server::UserCommand(Client &client, std::vector<commands>::iterator &it)
{

    client.set_username(it->params[0]);
    client.set_mode(it->params[1]);
    client.set_unused(it->params[2]);
    client.set_realname(it->params[3]);
}

/// @brief Quit the client, remove it from the master set and the clients map
/// @param i client socket fd
void Server::QuitCommand(int &i)
{

    close(i);           // Bye!
    FD_CLR(i, &master); // Remove from master set
    clients.erase(i);   // Remove from clients map
    std::cout << "Client " << i << " disconnected" << std::endl;
    return;
}

/// @brief A chaque 10 seconde environ, le client envoye un message PING au serveur,
// le serveur doit repondre avec un message PONG suivit des parametres
/// @param it    iterator of the command
/// @param i      client socket fd
void Server::PingCommand(std::vector<commands>::iterator &it, int &i)
{

    std::string rp = "PONG";
    if (it->params.size() > 0)
    {
        for (size_t i = 0; i < it->params.size(); i++)
            rp += " " + it->params[i];
    }
    rp += "\r\n";
    if (send(i, rp.c_str(), rp.size(), 0) < 0)
        perror("ERROR on send");
}

/// @brief Send 4 welcome messages to the client
/// @param client client inforamtion
/// @param i client's socket fd
void Server::FirstTimeConnectionMsg(Client &client, int &i)
{
    std::string serverName(SERVER_NAME);
    std::string msgWelcome(":" + serverName + " 001 " + client.get_nickname() + " :Welcome to the IRC Network, " + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + "\r\n");
    send(i, msgWelcome.c_str(), msgWelcome.size(), 0);
    msgWelcome.clear();
    msgWelcome = ":" + serverName + " 002 " + client.get_nickname() + " :Your host is " + serverName + ", running version 0.1\r\n";
    send(i, msgWelcome.c_str(), msgWelcome.size(), 0);
    msgWelcome.clear();
    msgWelcome = ":" + serverName + " 003 " + client.get_nickname() + " :This server was created " + __CREATION_DATE__ + " " + __CREATION_TIME__ + "\r\n";
    send(i, msgWelcome.c_str(), msgWelcome.size(), 0);
    msgWelcome.clear();
    msgWelcome = ":" + serverName + " 004 " + client.get_nickname() + " " + serverName + " 0.1 " + "i" + " " + "n" + "\r\n";
    send(i, msgWelcome.c_str(), msgWelcome.size(), 0);
    client.set_first_time_connected(false);
}