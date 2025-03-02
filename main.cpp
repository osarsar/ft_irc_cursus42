#include "./inc/servsocket.hpp"
#include "./inc/channel.hpp"
#include "./inc/client.hpp"
#include "./inc/privmsg.hpp"
#include "./inc/Kick.hpp"
#include "./inc/topic.hpp"
#include "./inc/invite.hpp"


void    executables(size_t &i, std::string data, int fd, client &client, privmsg &Privmsg, SERVSOCKET &server, channel &Channel, Kick &kick, Topic &topic, Invite &invite)
{
    server.trim(data);
    std::string command = data.substr(0, data.find(" "));
    toUpper(command);
    server.trim(command);
    Channel.NameVec.clear();
    Channel.PassVec.clear();

    if (!server.database[i - 1].registration_check && (command == KICK || command == TOPIC || command == INVITE ||\
        command == JOIN || command == PRIVMSG || command == MODE))
        server.mysend(fd, ERR_NOTREGISTERED(std::to_string(fd), IP));
    else if (command == JOIN && server.database[i - 1].registration_check)
    {
        int args = 0;
        args = Channel.fill(data);
        for (int k = 0; k < (int)Channel.NameVec.size(); k++) {
            if (Channel.PassVec.size() == Channel.NameVec.size() || Channel.NameVec.size() == 1) {
                if (Channel.PassVec.size() != 0)
                    Channel.join(Channel.NameVec[k], Channel.PassVec[k], server.database[i - 1], server);
                else
                    Channel.join(Channel.NameVec[k], std::string(""), server.database[i - 1], server);
            }
        }
    }
    else if (command == MODE && server.database[i - 1].registration_check)
        Channel.mode(data, server, server.database[i - 1]);
    else if (command == LPRIVMSG && server.database[i - 1].registration_check)
        Privmsg.parse_msg(data, server, server.database[i - 1]);
    else if (command == PART)
        Channel.part(data, server.database[i - 1], server);
    else if (command == KICK)
        kick.go_to_kick(data, server, fd);
    else if (command == TOPIC)
        topic.go_to_topic(data, server, fd);
    else if (command == INVITE)
        invite.go_to_invite(data, server, fd, Channel);
    else if ((command != "PASS" && command != "NICK" && command != "USER" && command != "UPPER" && command != "QUIT") && !command.empty())
        server.mysend(fd, ERR_UNKNOWNCOMMAND(server.client_ip, client.ip ,command));
}

int main(int ac, char** av) 
{
    if (ac != 3)
    {
        std::cerr << RED"Invalid arguments"RESET << std::endl;
        exit (1);
    }
    if (!is_numeric(av[1]))
    {
        std::cerr << RED"Invalid port"RESET << std::endl;
        exit (1);
    }
    try{
        std::string data;
        std::string port;

        SERVSOCKET server;
        privmsg Privmsg;
        channel Channel;
        Kick kick;
        Topic topic;
        Invite invite;
    
        port = av[1];
        server.servpass = av[2];
        int server_fd = server.mysocket(AF_INET, SOCK_STREAM);
        int value = f_stoi(port);
        server.mybind(IP, value);
        server.mylisten(5);
        std::cout << GREEN << "------- MY SERVER ------" << RESET << std::endl;
        std::cout << PURPLE << "Server Listening on port " << port << " ..." << RESET << std::endl;
        
        POLLFD vector;
        int client_fd;
        size_t i = 0;

        vector.push(server_fd, POLLIN, 0);
        while (true)
        {
            try
            {
                if (poll(vector.vector.data(), vector.vector.size(), -1) == -1)
                    exit(1);
                i = 0;
                while (i < vector.vector.size())
                {
                    if (vector.vector[i].revents & POLLIN)
                    {
                        if (vector.vector[i].fd == server_fd)
                        {
                            client_fd = server.myaccept();
                            vector.push(client_fd, POLLIN, 0);
                            server.push();
                        }
                        else
                        {
                            int check = 0;
                            data = server.Temsa_recv(1024, vector.vector[i].fd, check, server);
                            if (check == 1)
                            {
                                server.registration(vector.vector[i].fd, server.database[i - 1], data, server);
                                server.nickname(vector.vector[i].fd, server.database[i - 1], data, server);
                                server.username(vector.vector[i].fd, server.database[i - 1], data, server);
                                server.upper(vector.vector[i].fd, data, server, server.database[i - 1]);
                                executables(i, data, vector.vector[i].fd, server.database[i - 1], Privmsg, server, Channel, kick, topic, invite);
                                server.quit(vector, vector.vector[i].fd, data, i, server, server.database[i - 1]);
                            }
                        }
                    }
                    i++;
                }
            }
            catch (std::exception &e)
            {
                std::cerr << e.what() << std::endl;
                if (i)
                {
                    close(vector.vector[i].fd);
                    vector.vector.erase(vector.vector.begin() + i);
                    server.database.erase(server.database.begin() + i - 1);
                }
            }
            catch (const char *str)
            {
                server.mysend(vector.vector[i].fd, str);
            }
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
