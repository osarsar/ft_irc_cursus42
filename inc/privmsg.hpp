#pragma once

#include "servsocket.hpp"
#include "channel.hpp"
#include "client.hpp"
#include <fstream>
#define PRIVMSG "privmsg"

class privmsg {
    public :
        std::string channel_receive;
        std::string receiver;
        std::string message;
        privmsg();
        ~privmsg();
        void    parse_msg(std::string str, SERVSOCKET &server, client &Client);
        void    msg_to_client(int fd, std::string message, client &Client);
        int     client_fd(std::string str, SERVSOCKET &server);
};