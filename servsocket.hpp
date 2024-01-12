#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <poll.h>
#include <map>

class POLLFD
{
    public:
        std::vector<struct pollfd> vector;
    
    public:
        void push(int fd, short events, short revents);

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

    public:
        int mysocket(int ipvs, const int type);
        void mybind(std::string ip, int port);
        void mylisten(int nb_client);
        int  myaccept(void);
        std::string myrecv(unsigned int size, int fd);
        void mysend(int fd, std::string data_send);
        void show();

        class ErrorOnSocket : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

