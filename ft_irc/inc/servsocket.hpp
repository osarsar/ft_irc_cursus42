#pragma once

#include "channel.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <poll.h>
#include <map>
class channel;
class POLLFD
{
    public:
        std::vector<struct pollfd> vector;
        void push(int fd, short events, short revents);
        std::vector<int> getFds() const;
};

class SERVSOCKET
{
    public:
        std::string servpass;
        sockaddr_in server_addr;
        int socket_server;
        sockaddr_in client_addr;
        int socket_client;
        std::map<std::string, std::string> database;
        std::vector <channel *> channel_vec;

    public:
        int mysocket(int ipvs, const int type);
        void mybind(std::string ip, int port);
        void mylisten(int nb_client);
        int  myaccept(void);
        std::string myrecv(unsigned int size, int fd);
        void mysend(int fd, std::string data_send);
        void show();
        channel    *add_channel(std::string name, channel *Channel);

        class ErrorOnMySocket : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class ErrorOnMyBind : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class ErrorOnMyRecv : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class ErrorOnMyListen : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class ErrorOnMyAccept : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class ErrorOnMySend : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class ErrorOnPassword : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

