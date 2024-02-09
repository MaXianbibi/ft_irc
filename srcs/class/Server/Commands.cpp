#include "Server.hpp"


/// @brief Change ou init le nickname du Client
/// @param client info client
/// @param it iterator of the command
void Server::NickCommand(Client &client, std::vector<commands>::iterator &it)
{
    if (it->params.size() == 0)
        return;
    if (it->params[0].empty())
        return;
    // new client
    if (client.get_nickname().empty())
    {
        if (clients_by_nick.find(it->params[0]) != clients_by_nick.end())
        {
            std::string rp = "433 " + it->params[0] + " :Nickname is already in use\r\n";
            send(client.get_socket(), rp.c_str(), rp.size(), 0);
        }
        else
        {
            clients_by_nick[it->params[0]] = &client;
            client.set_nickname(it->params[0]);
        }
        // std::cout << "youpi" << std::endl;
        return;
    }
    // change nickname
    if (clients_by_nick.find(it->params[0]) != clients_by_nick.end()) // nickname already in use
    {
        std::string rp = "433 " + it->params[0] + " :Nickname is already in use\r\n";
        send(client.get_socket(), rp.c_str(), rp.size(), 0);
    }
    else
    {
        clients_by_nick.erase(client.get_nickname());
        clients_by_nick[it->params[0]] = &client;
        client.set_nickname(it->params[0]);
    }
}

/// @brief  Rempli les informations du client, nickname, username, mode, unused, realname
/// @param client client info
/// @param it command info
void Server::UserCommand(Client &client, std::vector<commands>::iterator &it)
{

    client.set_username(it->params[0]);
    client.set_unused(it->params[2]);
    client.set_realname(it->params[3]);
}

/// @brief Quit the client, remove it from the master set and the clients map
/// @param i client socket fd
void Server::QuitCommand(int &i)
{

    FD_CLR(i, &master); // Remove from master set
    clients_by_nick.erase(clients.at(i).get_nickname());
    clients.erase(i);   // Remove from clients map
    close(i);           // Bye!
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

    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
        std::cout << " NICKNAME : " << it->second.get_nickname() << std::endl;


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
    msgWelcome = ":" + serverName + " 004 " + client.get_nickname() + " :" + serverName + " 0.1 " + "o" + " " + "itkol" + "\r\n";
    send(i, msgWelcome.c_str(), msgWelcome.size(), 0);
    client.set_first_time_connected(false);
}


/// @brief Repond par les informations du client visé
/// @param it ( iterator of the command )
/// @param client ( client info )
/// @param i ( client socket fd )
void Server::WhoisCommand(std::vector<commands>::iterator &it, Client &client, int &i)
{
        try
        {
            Client client_target = get_client_by_nick(it->params[0]);

            std::string server_name = SERVER_NAME;
            std::string rp = ":" + server_name + " 311 " + client.get_nickname() + " :" + client_target.get_nickname() + " " + client_target.get_username() + " " + client_target.get_ip() + " * :" + client_target.get_realname() + "\r\n";
            if (send(i, rp.c_str(), rp.size(), 0) < 0)
                perror("ERROR on send");
            std::string rp2 = ":" + server_name + " 318 " + client.get_nickname() + " " + client_target.get_nickname() + " :End of WHOIS list ( mf )\r\n";
            if (send(i, rp2.c_str(), rp2.size(), 0) < 0)
                perror("ERROR on send");
        }
        catch (std::runtime_error &e)
        {
            std::string rs = ":" + std::string(SERVER_NAME) + " 401 " + it->params[0] + " :No such nick/channel\r\n";
            if (send(i, rs.c_str(), rs.size(), 0) < 0)
                perror("ERROR on send");
        }
}

/// @brief gere la command mode selon si c'est un channel ou un client
/// @bug defois le client n'est pas trouvé
/// @param it 
/// @param i 
void Server::ModeCommand(std::vector<commands>::iterator &it, int &i)
{
    if (it->params.size() < 1)
        return;
    if (it->params[0].empty())
        return;
    std::string caracters = "#&!+";

    if (caracters.find(it->params[0][0]) != std::string::npos)
    {
        std::cout << "channel" << std::endl; // C'est un canal
    }
    else
    {
        try
        {
            Client &target = get_client_by_nick(it->params[0]);
            if (it->params[1].empty())
                return;
            if (it->params[1][0] == '+')
            {
                for (size_t i = 1; i < it->params[1].size(); i++)
                {
                    if (target.get_mode().find(it->params[1][i]) == std::string::npos)
                        target.set_mode(target.get_mode() + it->params[1][i]);
                }
            }
            else if (it->params[1][0] == '-')
            {
                for (size_t i = 1; i < it->params[1].size(); i++)
                {
                    if (target.get_mode().find(it->params[1][i]) != std::string::npos)
                        target.set_mode(target.get_mode().erase(target.get_mode().find(it->params[1][i]), 1));
                }
            }
            // :<server_name> 221 <nickname> :+i
            std::string rp = ":" + std::string(SERVER_NAME) + " 221 " + target.get_nickname() + " :+" + target.get_mode() + "\r\n";
            if (send(i, rp.c_str(), rp.size(), 0) < 0)
                perror("ERROR on send");
        }
        catch (std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

/// @brief gere la commade Cap, si le client demande les LS, on lui repond
/// @param it (iterator of the command)
/// @param i (client socket fd)
void Server::CapCommand(std::vector<commands>::iterator &it, int &i)
{
    if (!it->params.empty() && it->params[0] == "LS")
    {
        std::string rp = ":" + std::string(SERVER_NAME) + " CAP * LS :\r\n";
        if (send(i, rp.c_str(), rp.size(), 0) < 0)
            perror("ERROR on send");
    }
}

/// @brief affiche les log du serv
/// @param buffer le message a afficher
void Server::Log(char buffer[1024])
{
    std::cout << "Received: " << buffer << std::endl;
}

/// @brief send a message to every client except the sender and the server (ourself)
/// @param i (client fd)
/// @param buffer (message to send)
/// @param n (size of the message)
void Server::msgToEveryClient(int &i, char buffer[1024], int n)
{
    {
        // We got some data from a client
        for (int j = 0; j <= fdmax; j++)
        {
            // Send to everyone!
            if (FD_ISSET(j, &master))
            {
                // Except the listener and ourselves
                if (j != sockfd && j != i)
                {
                    if (send(j, buffer, n, 0) == -1)
                    {
                        perror("ERROR on send");
                        // ? fatal("ERROR on recv");
                    }
                }
            }
        }
    }
}