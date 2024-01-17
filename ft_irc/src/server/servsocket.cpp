#include "../../inc/servsocket.hpp"
#include "../../inc/channel.hpp"

//--------------------------HANDEL ERRORS--------------------------------//

const char* SERVSOCKET::ErrorOnMySocket::what() const throw()
{
    return RED"Failed To Create"RESET;
}
const char* SERVSOCKET::ErrorOnMyBind::what() const throw()
{
    return RED"Failed To Bind"RESET;
}

const char* SERVSOCKET::ErrorOnMyListen::what() const throw()
{
    return RED"Failed To Listening"RESET;
}

const char* SERVSOCKET::ErrorOnMyRecv::what() const throw()
{
    return RED"Client Is Disconnected"RESET;
}

const char* SERVSOCKET::ErrorOnMyAccept::what() const throw()
{
    return RED"Failed To Accept"RESET;
}

const char* SERVSOCKET::ErrorOnMySend::what() const throw()
{
    return RED"Failed To Send"RESET;
}
const char* SERVSOCKET::ErrorOnPassword::what() const throw()
{
    return "";
}

//----------------------------------------------------------------------//

int SERVSOCKET::mysocket(int ipvs, const int type)
{
    socket_server = socket(ipvs, type, 0);
    if (socket_server == -1)
        throw ErrorOnMySocket();
    return socket_server;
}

void SERVSOCKET::mybind(std::string ip, int port)
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    int op = 1;
    setsockopt(socket_server,SOL_SOCKET,SO_REUSEADDR,(void *)&op, sizeof(op));
    if (bind(socket_server, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        close(socket_server);
        throw ErrorOnMyBind();
    }
}

void SERVSOCKET::mylisten(int nb_client)
{
    if (listen(socket_server, nb_client) == -1)
    {
        close(socket_server);
        throw ErrorOnMyListen();
    }
}

int SERVSOCKET::myaccept()
{
    std::string username;
    std::string password;
    std::string nickname;

    socklen_t clientAddressSize = sizeof(client_addr);
    socket_client = accept(socket_server, (struct sockaddr*)&client_addr, &clientAddressSize);
    if (socket_client == -1)
    {
        close(socket_server);
        throw ErrorOnMyAccept();
    }
    mysend(socket_client, "USERNAME: ");
    username = myrecv(1024, socket_client);
    mysend(socket_client, "NICKNAME: ");
    nickname = myrecv(1024, socket_client);
    mysend(socket_client, "PASSWORD: ");
    password = myrecv(1024, socket_client);
    database[username] = password;
    // if (password != servpass)
    // {
    //     mysend(socket_client, RED"INCORRECT PASSWORD"RESET);
    //     close(socket_client);
    //     throw ErrorOnPassword();
    // }

    mysend(socket_client, GREEN"------- WELCOME TO THE SERVER ------\n"RESET);
    return socket_client;
}

std::string SERVSOCKET::myrecv(unsigned int size, int fd)
{
    char buffer[size];
    ssize_t num_read;

    num_read = recv(fd, buffer, sizeof(buffer), 0);
    buffer[num_read] = '\0';
    if (num_read <= 0)
        throw ErrorOnMyRecv();
    std::string str(buffer);
    return str;
}

void SERVSOCKET::mysend(int fd, std::string data)
{
    if (send(fd, data.c_str(), data.length(), 0) == -1)
        throw ErrorOnMySend();
}

void SERVSOCKET::show()
{
    std::map<std::string, std::string>::iterator it;

    for (it = database.begin(); it != database.end() ;it++)
    {
        std::cout << "username = " << it->first;
        std::cout << "password = " << it->second;
    }
}


void POLLFD::push(int fd, short events, short revents)
{
    struct pollfd sock_fd;

    sock_fd.fd = fd;
    sock_fd.events = events;
    sock_fd.revents = revents;
    vector.push_back(sock_fd);
}

channel *SERVSOCKET::add_channel(std::string name, channel *Channel) {
    Channel = new channel(name);
        channel_vec.push_back(Channel);
    return Channel;
}

std::vector<int> POLLFD::getFds() const {
    std::vector<int> fds;
    for (std::vector<struct pollfd>::const_iterator it = this->vector.begin(); it != vector.end(); ++it) {
        fds.push_back(it->fd);
    }
    return fds;
}