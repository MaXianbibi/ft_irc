#include "Server.hpp"

/// @brief Broadcast to every client in the channel
/// @param message Message to broadcast
void s_channel::broadcast(std::string message)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
            (*it)->sendMessage(message);
    
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