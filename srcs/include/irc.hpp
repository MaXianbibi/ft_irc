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

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

enum e_command
{
    NICK,
    USER,
    PRIVMSG,
    JOIN,
    PART,
    QUIT,
    PING,
    PONG,
    MOTD,
    LUSERS,
    WHOIS,
    WHO,
    LIST,
    TOPIC,
    NAMES,
    KICK,
    MODE,
    OPER,
    AWAY,
    REHASH,
    RESTART,
    SUMMON,
    USERS,
    WALLOPS,
    USERHOST,
    ISON,
    TIME,
    VERSION,
    STATS,
    LINKS,
    ADMIN,
    INFO,
    SERVICE,
    SERVLIST,
    SQUERY,
    SQUIT,
    CONNECT,
    TRACE,
    KILL,
    ERROR,
    UNKNOWN
};

#endif
