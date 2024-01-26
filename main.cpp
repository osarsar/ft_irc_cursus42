#include "./inc/servsocket.hpp"
#include "./inc/channel.hpp"
#include "./inc/privmsg.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << RED "Invalid arguments" RESET << std::endl;
        exit(1);
    }
    client client;
    privmsg Privmsg;
    SERVSOCKET server;
    channel Channel;
    std::string data;
    std::string port;

    port = av[1];
    server.servpass = av[2];
    int server_fd = server.mysocket(AF_INET, SOCK_STREAM);
    int value = f_stoi(port);
    server.mybind("10.12.6.7", value);
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
            poll(vector.vector.data(), vector.vector.size(), -1);
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
                        data = server.myrecv(1024, vector.vector[i].fd);
                        // server.mysend(vector.vector[i % 2 + 1].fd,data);
                        server.registration(vector.vector[i].fd, server.database[i - 1], data);
                        server.nickname(vector.vector[i].fd, server.database[i - 1], data);
                        server.username(vector.vector[i].fd, server.database[i - 1], data);
                        std::string command = data.substr(0, data.find(" "));
                        if (!server.database[i - 1].registration_check && (command == JOIN && command == PRIVMSG && command == MODE))
                            throw(RED "Khasek lwra9 a m3alem sir tal gheda oji\n" RESET);
                        else if (command == JOIN && server.database[i - 1].registration_check)
                            Channel.join(data, server.database[i - 1], server);
                        else if (command == MODE && server.database[i - 1].registration_check)
                            Channel.mode(data, server, server.database[i - 1]);
                        else if (command == PRIVMSG && server.database[i - 1].registration_check)
                            Privmsg.parse_msg(data, server, server.database[i - 1]);
                        // client.getclient_fd(vector.vector);
                        // std::cout << client.client_fd << std::endl;
                        // std::cout << data << std::endl;
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
    return 0;
}
