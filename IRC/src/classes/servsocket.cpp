#include "../../inc/servsocket.hpp"
#include "../../inc/channel.hpp"
#include "../../inc/Kick.hpp"
#include "../../inc/invite.hpp"

//--------------------------HANDEL ERRORS--------------------------------//

const char* SERVSOCKET::ErrorOnMySocket::what() const throw()
{
    return RED"Failed To Create"RESET;
}
const char* SERVSOCKET::ErrorOnMyBind::what() const throw()
{
    return (RED"Failed To Bind"RESET);
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
    (void)ip;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

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
    std::string password;

    socklen_t clientAddressSize = sizeof(client_addr);
    socket_client = accept(socket_server, (struct sockaddr*)&client_addr, &clientAddressSize);
    if (socket_client == -1)
    {
        close(socket_server);
        throw ErrorOnMyAccept();
    }

    const char* client_ip_str = inet_ntoa(client_addr.sin_addr);
    if (client_ip_str == NULL)
    {
        throw ErrorOnMyAccept();
    }
    strncpy(client_ip, client_ip_str, INET_ADDRSTRLEN);

    return socket_client;
}

client* SERVSOCKET::get_client_Tem(int fd)
{
    std::vector<client>::iterator iter_clt;
    for (iter_clt = database.begin(); iter_clt != database.end();iter_clt++)
    {
        if (iter_clt->fd == fd)
            return &(*iter_clt);
    }
    return NULL;
}

std::string SERVSOCKET::Temsa_recv(unsigned int size, int fd, int &check, SERVSOCKET &serv)
{
    char buffer[size];
    std::string mybuffer;
    ssize_t num_read;
    int _else = 0;
    (void) serv;

    num_read = recv(fd, buffer, sizeof(buffer), 0);
    if (num_read <= 0)
        throw ErrorOnMyRecv();

    mybuffer.append(buffer, num_read);
    
    if (mybuffer.find_first_of("\r\n") != std::string::npos)
    {
        if(fd_buff[fd] != "")
        {
            fd_buff[fd].erase(fd_buff[fd].size() - 1);
            fd_buff[fd].append(buffer, num_read - 1);
            std::string str(fd_buff[fd]);
            fd_buff[fd] = "";
            check = 1;
            return str;
        }
        check = 1;
    }
    else
    {
        num_read++;
        _else = 1;
    }
    
    buffer[num_read - 1] = '\0';

    if (_else == 1)
    {
        if (!fd_buff[fd].empty())
            fd_buff[fd].erase(fd_buff[fd].size() - 1);
        fd_buff[fd].append(buffer, num_read);
    }

    if(fd_buff[fd] != "")
    {
        std::string str(fd_buff[fd]);
        return str;
    }

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

void SERVSOCKET::registration(int client_fd, client &client, std::string data, SERVSOCKET server)
{
    std::string str;
    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 1)
        return;

    toUpper(commands[0]);
    trim(commands[0]);

    if (commands[0] != "PASS")
        return;

    if (commands.size() == 1)
        commands = my_split(commands[0], '\t', server);

    if (commands.size() < 2)
    {
        server.mysend(client_fd, ERR_NEEDMOREPARAMS(client.nickname, "PASS", client.ip));//-> HERE
        return ;
    }
    if (commands.size() > 2)
    {
        server.mysend(client_fd, ERR_NEEDMOREPARAMS(client.nickname, "PASS", client.ip));
        return ;
    }
    trim(commands[1]);

    if (commands[0] == "PASS")
    {
        if (client.pass_bool == true)
        {
            mysend(client_fd, ERR_ALREADYREGISTRED(client.ip, client.nickname));
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
        trim(str);
        if (str != servpass)
        {
            mysend(client_fd, ERR_PASSWDMISMATCH(client.ip, client.nickname));
            return;
        }
        client.pass_bool = true;
        client.time = clock();
    }
    
}

void SERVSOCKET::nickname(int client_fd, client &client, std::string data, SERVSOCKET server)
{
    std::string str;
    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 1)
        return;

    toUpper(commands[0]);
    trim(commands[0]);
    if (commands[0] != "NICK")
        return;

    if (commands.size() == 1)
        commands = my_split(commands[0], '\t', server);

    if (commands.size() < 2)
    {
        mysend(client_fd, ERR_NEEDMOREPARAMS(client.nickname, "NICK", client.ip));
        return ;
    }
    if (commands.size() > 2)
    {
        mysend(client_fd, ERR_NEEDMOREPARAMS(client.nickname, "NICK", client.ip));
        return ;
    }

    trim(commands[1]);

    if (commands[0] == "NICK")
    {
        if (client.pass_bool != 1)
        {
            mysend(client_fd, ORANGE"PLEASE ENTER THE PASSWORD FIRST\n"RESET);
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
        trim(str);
        for (it = database.begin(); it != database.end() ; it++)
        {
            if (it->nickname == str)
            {
                mysend(client_fd, ERR_NICKNAMEINUSE(client.ip , it->nickname, it->nickname));
                return;
            }
        }
        client.nick_bool = true;
        client.nickname = str;
        mysend(client_fd, GREEN"NICKNAME REGISTRED SUCCESSFULLY\n"RESET);
        if (client.user_bool == 1)
        {
            client.registration_check = true;
            client.fd = client_fd;
            mysend(client.fd, RPL_WELCOME(client.nickname, client.ip));
            mysend(client.fd, RPL_WELCOME(client.nickname, client.ip));
            mysend(client.fd, RPL_YOURHOST(client.nickname, client.ip));
            mysend(client.fd, RPL_CREATED(client.nickname, client.ip));
            mysend(client.fd, RPL_MYINFO(client.nickname, client.ip));
        }
    }
}

void SERVSOCKET::username(int client_fd, client &client, std::string data, SERVSOCKET server)
{
    std::string str;
    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 1)
        return;

    toUpper(commands[0]);
    trim(commands[0]);
    if (commands[0] != "USER")
        return;

    if (commands.size() == 1)
        commands = my_split(commands[0], '\t', server);

    if (commands.size() < 5)
    {
        mysend(client_fd, ERR_NEEDMOREPARAMS(client.nickname, "USER", client.ip));
        return ;
    }
    if (commands.size() > 5)
    {
        mysend(client_fd, ERR_NEEDMOREPARAMS(client.nickname, "USER", client.ip));
        return ;
    }
    
    trim(commands[1]);
    trim(commands[4]);

    if (commands[0] == "USER")
    {
        if (client.pass_bool != 1)
        {
            mysend(client_fd, ORANGE"PLEASE ENTER THE PASSWORD FIRST\n"RESET);
            return;
        }
        str = commands[1];
        if (str.empty())
            return;
        const char *ptr = str.c_str();
        while (std::isspace(*ptr))
            ptr++;
        if (!*ptr)
            return;
        str = std::strtok(const_cast<char *>(str.c_str()), " ");
        client.user_bool = true;
        client.username = commands[4];
        mysend(client_fd, GREEN"USERNAME REGISTRED SUCCESSFULLY\n"RESET);
        if (client.nick_bool == 1)
        {
            client.registration_check = true;
            client.fd = client_fd;
            mysend(client.fd, RPL_WELCOME(client.nickname, client.ip));
            mysend(client.fd, RPL_WELCOME(client.nickname, client.ip));
            mysend(client.fd, RPL_YOURHOST(client.nickname, client.ip));
            mysend(client.fd, RPL_CREATED(client.nickname, client.ip));
            mysend(client.fd, RPL_MYINFO(client.nickname, client.ip));
        }
    }
}

void SERVSOCKET::upper(int client_fd, std::string data, SERVSOCKET server, client &client)
{
    std::string str;
    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 1)
        return;

    toUpper(commands[0]);
    trim(commands[0]);

    if (commands[0] != "BOT")
        return;
    
    if (commands.size() == 1)
        commands = my_split(commands[0], '\t', server);

    if (commands.size() > 2 || commands.size() == 1)
    {
        mysend(client_fd, ERR_BOT(client_ip ,client.nickname));
        return ;
    }
    trim(commands[1]);
    toUpper(commands[1]);
    if (commands[0] == "BOT")
    {
        std::string message = ":" + client.nickname + "!" + client.username + "@" + server.client_ip + " PRIVMSG " + " " + commands[1];
        mysend(client_fd, message);
    }
}

void SERVSOCKET::push()
{
    client client;
    client.ip = client_ip;
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

void SERVSOCKET::quit(POLLFD &vector, int client_fd, std::string data, int i, SERVSOCKET server, client &client)
{
    std::string str;
    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 1)
        return;

    toUpper(commands[0]);
    trim(commands[0]);

    if (commands[0] != "QUIT")
        return;

    if (commands.size() >= 2)
        return ;

    if (commands.size() == 1)
        commands = my_split(commands[0], '\t', server);

    if (commands[0] == "QUIT")
    {
        std::cout << client_ip << RED" has disconnected"RESET << std::endl;
        mysend(client_fd, RPL_QUIT(client.nickname, client.username, client.ip));
        close(client_fd);
        vector.vector.erase(vector.vector.begin() + i);
        server.database.erase(server.database.begin() + i - 1);
    }
        
}
