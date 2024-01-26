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


std::string& SERVSOCKET::ltrim(std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start != std::string::npos) {
        str.erase(0, start);
    } else {
        str.clear();
    }
    return str;
}

std::string& SERVSOCKET::rtrim(std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r");
    if (end != std::string::npos) {
        str.erase(end + 1);
    } else {
        str.clear();
    }
    return str;
}

std::string& SERVSOCKET::trim(std::string& str) {
    return ltrim(rtrim(str));
}

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
        throw ErrorOnMyBind();//exit()
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
    std::string password;

    socklen_t clientAddressSize = sizeof(client_addr);
    socket_client = accept(socket_server, (struct sockaddr*)&client_addr, &clientAddressSize);
    if (socket_client == -1)
    {
        close(socket_server);
        throw ErrorOnMyAccept();
    }
    mysend(socket_client, GREEN"------- WELCOME TO THE SERVER ------\n"RESET);
    return socket_client;
}

std::string SERVSOCKET::myrecv(unsigned int size, int fd)
{
    char buffer[size];
    ssize_t num_read;

    num_read = recv(fd, buffer, sizeof(buffer), 0);
    buffer[num_read - 1] = '\0';
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


void POLLFD::push(int fd, short events, short revents)
{
    struct pollfd sock_fd;

    sock_fd.fd = fd;
    sock_fd.events = events;
    sock_fd.revents = revents;
    vector.push_back(sock_fd);
}

channel    &SERVSOCKET::add_channel(std::string name, channel &Channel) {
    Channel.channelName = name;
    channel_map[name] = Channel;
    return (Channel);
}


std::vector<int> POLLFD::getFds() const {
    std::vector<int> fds;
    for (std::vector<struct pollfd>::const_iterator it = this->vector.begin(); it != vector.end(); ++it) {
        fds.push_back(it->fd);
    }
    return fds;
}

void SERVSOCKET::registration(int client_fd, client &client, std::string data)
{
    std::string str;
    if (data.substr(0, 5) == "PASS ")
    {
        if (client.pass_bool == true)
        {
            mysend(client_fd, ORANGE"YOU ALREADY ENTER THE PASSWORD\n"RESET);
            return;
        }
        str = data.erase(0, 5);
        if (str.empty())
            return;
        const char *ptr = str.c_str();
        while (std::isspace(*ptr))
            ptr++;
        if (!*ptr)
            return;
        str = std::strtok(const_cast<char *>(str.c_str()), " ");
        if (str != servpass)
        {
            mysend(client_fd, RED"INCORRECT PASSWORD\n"RESET);
            return;
        }
        client.pass_bool = true;
        mysend(client_fd, GREEN"PASSWORD REGISTRED SUCCESSFULLY\n"RESET);
    }
}

void SERVSOCKET::nickname(int client_fd, client &client, std::string data)
{
    std::string str;
    if (data.substr(0, 5) == "NICK ")
    {
        if (client.pass_bool != 1)
        {
            mysend(client_fd, ORANGE"PLEASE ENTER THE PASSWORD FIRST\n"RESET);
            return;
        }
        if (client.nick_bool == true)
        {
            mysend(client_fd, ORANGE"YOU ALREADY ENTER THE NICKNAME\n"RESET);
            return;
        }
        str = data.erase(0, 5);
        if (str.empty())
            return;
        const char *ptr = str.c_str();
        while (std::isspace(*ptr))
            ptr++;
        if (!*ptr)
            return;
        str = std::strtok(const_cast<char *>(str.c_str()), " ");
        for (it = database.begin(); it != database.end() ; it++)
        {
            if (it->nickname == str)
            {
                mysend(client_fd, RED"NICKNAME ALREADY USED\n"RESET);
                return;
            }
        }
        client.nick_bool = true;
        client.nickname = str;
        mysend(client_fd, GREEN"NICKNAME REGISTRED SUCCESSFULLY\n"RESET);
        if (client.user_bool == 1)
        {
            mysend(client_fd, PURPLE"---- YOU HAVE BEEN REGISTRED SUCCESSFULLY ----\n"RESET);
            client.registration_check = true;
            client.fd = client_fd;
        }

    }
}

void SERVSOCKET::username(int client_fd, client &client, std::string data)
{
    std::string str;
    if (data.substr(0, 5) == "USER ")
    {
        if (client.pass_bool != 1)
        {
            mysend(client_fd, ORANGE"PLEASE ENTER THE PASSWORD FIRST\n"RESET);
            return;
        }
        if (client.user_bool == true)
        {
            mysend(client_fd, ORANGE"YOU ALREADY ENTER THE USERNAME\n"RESET);
            return;
        }
        str = data.erase(0, 5);
        if (str.empty())
            return;
        const char *ptr = str.c_str();
        while (std::isspace(*ptr))
            ptr++;
        if (!*ptr)
            return;
        str = std::strtok(const_cast<char *>(str.c_str()), " ");
        client.user_bool = true;
        client.username = str;
        mysend(client_fd, GREEN"USERNAME REGISTRED SUCCESSFULLY\n"RESET);
        if (client.nick_bool == 1)
        {
            mysend(client_fd, PURPLE"---- YOU HAVE BEEN REGISTRED SUCCESSFULLY ----\n"RESET);
            client.registration_check = true;
            client.fd = client_fd;
        }
    }
}

void SERVSOCKET::push()
{
    client client;
    database.push_back(client);
}

void SERVSOCKET::show()
{
    std::vector<client>::iterator it;

    for (it = database.begin(); it != database.end(); it++)
    {
        std::cout << "---DATABASE--- "<< std::endl;
        std::cout << "username: " << it->username << std::endl;
        std::cout << "nickname: " << it->nickname << std::endl;
        std::cout << "client_fd: " << it->fd << std::endl;
    }
}