#ifndef KICK_HPP
# define KICK_HPP


#include "channel.hpp"

#define KICK "KICK"
#define TOPIC "TOPIC"
#define INVITE "INVITE"

//--------------------------------------------------->>> Temsa:

class Kick
{
public:
    Kick();
    ~Kick();
    
    void 	go_to_kick(std::string data, SERVSOCKET &server);
    void 	kick(client *kicker, client *target);
};

void toUpper(std::string& str);

#endif