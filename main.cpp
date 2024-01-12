#include "servsocket.hpp"
#define PORT 12337

int main(int ac, char** av) 
{
    try
    {
        (void)ac;
        SERVSOCKET server;
        std::string data;
        std::string port;
        
        port = av[1];
        server.servpass = av[2];
        int server_fd = server.mysocket(AF_INET, SOCK_STREAM);
        server.mybind("127.0.0.1", std::stoi(port));
        server.mylisten(5);

        std::cout << "Server listening on port " << port << " ..." << std::endl;

        // std::cout << "------- MY SERVER ------" << std::endl;

        POLLFD vector;
        int client_fd;

        vector.push(server_fd, POLLIN, 0);
        while (true)
        {
            poll(vector.vector.data(), vector.vector.size(), -1);
            for (size_t i = 0; i < vector.vector.size(); i++)
            {
                if (vector.vector[i].revents & POLLIN)
                {
                    if (vector.vector[i].fd == server_fd)
                    {
                        client_fd = server.myaccept();
                        vector.push(client_fd, POLLIN, 0);
                    }
                    else
                    {
                        data = server.myrecv(1024, vector.vector[i].fd);
                        std::cout << data << std::endl;
                    }
                }
            }
        }

    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        // close(clientSocket);
        // close(serverSocket);
    }
    return 0;
}
