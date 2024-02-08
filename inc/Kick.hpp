#ifndef KICK_HPP
# define KICK_HPP


#include "channel.hpp"
#include "error.hpp"
#include "client.hpp"
#include "invite.hpp"
#include "privmsg.hpp"
#include "servsocket.hpp"
#include <map>

#define KICK "KICK"
#define TOPIC "TOPIC"
#define INVITE "INVITE"

//--------------------------------------------------->>> Temsa:

class Kick
{
public:
    Kick();
    ~Kick();
    
    void 	go_to_kick(std::string data, SERVSOCKET &server, int fd);
};

void toUpper(std::string& str);

#endif