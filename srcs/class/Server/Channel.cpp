/**
 * @file Channel.cpp
 * @author Justin Morneau
 * @brief gere les cannaux / channels du serveur IRC
 * @version 0.1
 * @date 2024-02-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Server.hpp"

/// @brief Broadcast to every client in the channel
/// @param message Message to broadcast
void s_channel::broadcast(std::string message)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
            (*it)->sendMessage(message);
    
}

/// @brief Broadcast to every client in the channel except the client
/// @param message 
/// @param client 
void s_channel::broadcast(std::string message, Client & client)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if ((*it) != &client)
        {
            std::cout << "DEBUG : " << (*it)->get_nickname() << std::endl;
            (*it)->sendMessage(message);
        }
    }
    
}

/// @brief  kick a client from the channel ( doesn't send a message to the client )
/// @param target target to kick
void s_channel::kickClient(Client &target)
{
    std::vector<Client *>::iterator it = clients.begin();
    for (; it != clients.end(); ++it)
    {
        if ((*it)->get_nickname() == target.get_nickname())
        {
            target.set_is_joinned_channel(false);
            target.set_join_channel("");
            clients.erase(it);
            break;
        }
    }
}

/// @brief  get a client by his nickname
/// @param nickname 
/// @return 
Client* s_channel::get_client_by_nick(std::string nickname)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if ((*it)->get_nickname() == nickname)
            return *it;
    }
    return NULL;
}


/// @brief  check if a client is in the channel
/// @param client 
/// @return true if the client is in the channel, false otherwise 
bool s_channel::is_client_in_channel(Client &client)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (*it == &client)
            return true;
    }
    return false;
}

/// @brief remove a client from the channel
/// @param client 
void s_channel::remove_client(Client &client)
{
    std::vector<Client *>::iterator it = clients.begin();
    for (; it != clients.end(); ++it)
    {
        if ((*it)->get_nickname() == client.get_nickname())
        {
            clients.erase(it);
            break;
        }
    }
}

/// @brief add a client to the channel
/// @param client 
void s_channel::add_client(Client &client)
{
    clients.push_back(&client);
}

std::string s_channel::get_mode_string()
{
    std::string mode = "+";
    if (this->mode.i)
        mode += "i";
    if (this->mode.t)
        mode += "t";
    if (this->mode.k)
        mode += "k";
    if (this->mode.o)
        mode += "o";
    if (this->mode.l)
        mode += "l";
    return mode;
}