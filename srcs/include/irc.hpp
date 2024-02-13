#ifndef IRC_HPP
#define IRC_HPP

#define SUCCESS 0
#define FAILURE 1

#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <set>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>


# define RPL_WELCOME ":Welcome to the ft_IRC Internet Relay Network " + CLIENT
# define RPL_NOTOPIC ":No topic is set"
# define RPL_ENDOFWHOIS ":End of WHOIS list"
# define RPL_YOUREOPER ":You are now an IRC operator"
# define RPL_UNAWAY ":You are no longer marked as being away"
# define RPL_NOWAWAY ":You have been marked as being away"
# define RPL_ENDOFWHO ":End of WHO list"
# define RPL_WHOISIDLE " :seconds idle"

# define ERR_NOTREGISTERED ":You have not registered"
# define ERR_NOSUCHSERVICE ":No such service"
# define ERR_NOPRIVILEGES ":Permission Denied- You're not an IRC operator"
# define ERR_NOSUCHNICK ":No such nick/channel"
# define ERR_NORECIPIENT ":No recipient given (PRIVMSG)"
# define ERR_NOTEXTTOSEND ":No text to send"
# define ERR_CANNOTSENDTOCHAN ":Cannot send to channel"
# define ERR_CHANOPRIVSNEEDED ":You're not channel operator"
# define ERR_USERNOTINCHANNEL ":They aren't on that channel"
# define ERR_NEEDMOREPARAMS ":Not enough parameters"
# define ERR_ALREADYREGISTRED ":Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH ":Password incorrect"
# define ERR_NOOPERHOST ":No O-lines for your host"
# define ERR_NOSUCHCHANNEL ":No such channel"
# define ERR_NOTONCHANNEL " :You're not on that channel"
# define ERR_USERONCHANNEL ":is already on channel"
# define ERR_INVITEONLYCHAN " :Cannot join channel (+i)"
# define ERR_BADCHANNELKEY " :Cannot join channel (+k)"
# define ERR_UNKNOWNCOMMAND ":Unknown command"
# define ERR_NONICKNAMEGIVEN ":No nickname given"
# define ERR_NICKNAMEINUSE ":Nickname is already in use"
# define ERR_ERRONEUSNICKNAME ":Erroneous nickname"
# define ERR_UMODEUNKNOWNFLAG ":Unknown MODE flag"
# define ERR_USERSDONTMATCH ":Cannot change mode for other users"
# define ERR_NOORIGIN ":No origin specified"
# define ERR_NOSUCHSERVER ":No such server"
# define ERR_NOORIGIN ":No origin specified"
# define ERR_UNKNOWNMODE ":is unknown mode char to me"



#endif
