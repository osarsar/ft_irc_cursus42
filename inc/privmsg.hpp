#pragma once


#include "servsocket.hpp"
#include "channel.hpp"
#include "client.hpp"
#include <fstream>
#define PRIVMSG "privmsg"
#define LPRIVMSG "PRIVMSG"

class SERVSOCKET;
class client;
class privmsg {
    public :
        std::string message;
        privmsg();
        ~privmsg();
        
        void    parse_msg(std::string str, SERVSOCKET &server, client &Client);
        void    msg_to_client(int fd, std::string message, std::string receiver, client &Client, SERVSOCKET &server);
        void    msg_to_channel(SERVSOCKET &server, std::string message, std::string receiver, client &Client, bool flag);
        int     client_fd(std::string str, SERVSOCKET &server);
};