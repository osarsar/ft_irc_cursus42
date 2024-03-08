#include "../../inc/invite.hpp"

bool is_numeric(const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> my_split(const std::string& input, char delimiter, SERVSOCKET server)
{
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        token = server.trim(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

Invite::Invite()
{
}

Invite::~Invite()
{
}

client* get_client(int fd, SERVSOCKET &server)
{
    std::vector<client>::iterator iter_clt;
    for (iter_clt = server.database.begin(); iter_clt != server.database.end();iter_clt++)
    {
        if (iter_clt->fd == fd)
            return &(*iter_clt);
    }
    return NULL;
}

client* get_client(std::string nickname, SERVSOCKET &server)
{
    std::vector<client>::iterator iter_clt;
    for (iter_clt = server.database.begin(); iter_clt != server.database.end();iter_clt++)
    {
        if (iter_clt->nickname == nickname)
            return &(*iter_clt);
    }
    return NULL;
}

channel* get_channel(std::string channelName, SERVSOCKET &server)
{
    std::map <std::string, channel>::iterator iter_chnl;

    for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
    {
        if (iter_chnl->first == channelName)
            return &(iter_chnl->second);
    }
    return NULL;
}

int check_client_in_channel(client guest, SERVSOCKET &server, std::string channel_name)
{
	std::map <std::string, channel>::iterator iter_chnl;
    std::vector<client>::iterator iter_clt;
		
    for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
    {
        if (iter_chnl->first == channel_name)
        {
            for (iter_clt = iter_chnl->second.client_list.begin(); iter_clt != iter_chnl->second.client_list.end();iter_clt++)
            {
                if (iter_clt->nickname == guest.nickname)
                    return 1;
            }
            break;
        }
    }
    return 0;
}

bool isInAdminOf(std::string str, std::vector<std::string> _adminOf)
{
    std::vector<std::string>::iterator it;
    for (it = _adminOf.begin(); it != _adminOf.end(); ++it) {
        if (*it == str) {
            return true;
        }
    }
    return false;
}

void Invite::go_to_invite(std::string data, SERVSOCKET &server, int fd, channel &Channel3)
{
    client *host;
    host = get_client(fd, server);
    std::string host_ni = host->nickname;
    std::string host_us = host->username;

    std::vector<std::string> commands0 = my_split(data, ' ', server);
    if ((commands0.size() == 2) && commands0[1] == ":")
    {
        data.erase(data.size() - 1);
        server.trim(data);
    }

    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 3)
    {
        server.mysend(fd, ERR_NEEDMOREPARAMS(host_ni, "INVITE", host->ip));
        return ;
    }

    std::string channelName = commands[2];
    channel *channel = get_channel(channelName, server);
    if (!channel)
    {
        server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channelName, host->ip));
        return;
    }

    client *guest;
    guest = get_client(commands[1], server);
    if (!guest)
    {
        server.mysend(fd, ERR_NOSUCHNICK(host_ni, commands[1], channelName));
        return;
    }
    std::string guestd = guest->username;

    if (check_client_in_channel(*host, server, channelName) == 0)
    {
        server.mysend(fd, ERR_NOTONCHANNEL(host_ni, channelName, host->ip));
        return;
    }

    if (check_client_in_channel(*guest, server, channelName) == 1)
    {
       server.mysend(fd, ERR_USERONCHANNEL(host_ni, guest->nickname ,channelName));
        return;
    }
    if (Channel3.Iflag == true)
    {
        if (!isInAdminOf(channelName, host->adminOf))
        {
            server.mysend(fd, ERR_CHANOPRIVSNEEDED(host_ni, channelName, host->ip));
            return;
        }
    }
    channel->invited_users.push_back(guest->nickname);

    server.mysend(fd, RPL_INVITING(host_ni, guest->nickname, channelName, host->ip));
    server.mysend(guest->fd, RPL_INVITELIST(host_ni, host_us, host->ip, guest->nickname ,channelName));

    return;
}
