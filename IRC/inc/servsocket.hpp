#pragma once


#include "channel.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <poll.h>
#include <map>

class channel;
class client;



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
        std::map<int, std::string> fd_buff;
        std::string servpass;
        sockaddr_in server_addr;
        int socket_server;
        sockaddr_in client_addr;
        int socket_client;
        char client_ip[INET_ADDRSTRLEN];
        std::map <std::string, channel> channel_map;
        std::vector<client> database;
        std::vector<client>::iterator it;

    public:
        void push();
        int mysocket(int ipvs, const int type);
        void mybind(std::string ip, int port);
        void mylisten(int nb_client);
        int  myaccept(void);
        std::string Temsa_recv(unsigned int size, int fd, int &check, SERVSOCKET &serv);
        client* get_client_Tem(int fd);
        void mysend(int fd, std::string data_send);
        void registration(int client_fd, client &client, std::string data, SERVSOCKET server);
        void nickname(int client_fd, client &client, std::string data, SERVSOCKET server);
        void username(int client_fd, client &client, std::string data, SERVSOCKET server);
        void upper(int client_fd, std::string data, SERVSOCKET server, client &client);
        void quit(POLLFD &vector, int client_fd, std::string data, int i, SERVSOCKET server, client &client);
        channel &add_channel(std::string name, channel &Channel);
        std::string&    ltrim(std::string& str);
        std::string&    rtrim(std::string& str);
        std::string&    trim(std::string& str);
        void show();

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
