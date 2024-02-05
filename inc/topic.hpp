#ifndef TOPIC_HPP
# define TOPIC_HPP


#include "channel.hpp"
#include "client.hpp"
#include "invite.hpp"
#include "privmsg.hpp"
#include "servsocket.hpp"
#include "channel.hpp"
#include "error.hpp"

//--------------------------------------------------->>> Temsa:

class Topic
{
private:
    
public:
    Topic();
    ~Topic();

    void    go_to_topic(std::string data, SERVSOCKET &server, int fd);
};


#endif