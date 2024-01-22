#pragma once

#include "servsocket.hpp"
#include "channel.hpp"
#include "client.hpp"

#define PRIVMSG "privmsg"

class SERVSOCKET;
class privmsg {
    public :
        std::string channel;
        std::string receiver;
        std::string message;
        privmsg();
        ~privmsg();
        void    parse_msg(std::string str, SERVSOCKET &server);
};