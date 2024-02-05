#include "../../inc/invite.hpp"

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
            return &(*iter_clt);// HERE
    }
    return NULL;
}

client* get_client(std::string nickname, SERVSOCKET &server)
{
    std::vector<client>::iterator iter_clt;
    for (iter_clt = server.database.begin(); iter_clt != server.database.end();iter_clt++)
    {
        if (iter_clt->nickname == nickname)
            return &(*iter_clt);// HERE
    }
    return NULL;
}

channel* get_channel(std::string channelName, SERVSOCKET &server)
{
    std::map <std::string, channel>::iterator iter_chnl;

    for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
    {
        if (iter_chnl->first == channelName)
            return &(iter_chnl->second);// HERE
    }
    return NULL;
}

int check_client_in_channel(client guest, SERVSOCKET server, std::string channel_name)
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

int is_operator(const std::vector<std::string>& operators, const std::string& client_name) {
for (std::vector<std::string>::const_iterator it = operators.begin(); it != operators.end(); ++it) {
        if (*it == client_name) {
            return 1;
        }
    }
    return 0;
}

void Invite::go_to_invite(std::string data, SERVSOCKET &server, int fd)
{
    client *host;
    host = get_client(fd, server);

    std::string host_ni = host->nickname;
    std::string host_us = host->username;

    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 3)
    {
        server.mysend(fd, ERR_NEEDMOREPARAMS(host_ni, "INVITE"));
        return ;
    }

    client *guest;
    guest = get_client(commands[1], server);
    if (!guest)
    {
        server.mysend(fd, ERR_NOSUCHNICK(host_ni, commands[1]));
        return;
    }
    std::string guestd = guest->username;

    std::string channelName = commands[2];
    channel *channel = get_channel(channelName, server);
    if (!channel)
    {
        server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channelName));
        return;
    }

    if (check_client_in_channel(*host, server, channelName) == 0)
    {
        server.mysend(fd, ERR_NOTONCHANNEL(host_ni, channelName));
        return;
    }

    if (check_client_in_channel(*guest, server, channelName) == 1)
    {
       server.mysend(fd, ERR_USERONCHANNEL(host_ni, guest->nickname ,channelName));
        return;
    }
    
    // if (!is_operator(channel->operators, host_ni))
    // {
    //    server.mysend(fd, ERR_CHANOPRIVSNEEDED((host_ni, channelName)));
    //     return;
    // }

    channel->invited_users.push_back(guest->nickname);

    server.mysend(fd, RPL_INVITING(host_ni, guest->nickname, channelName));

    // HERE send for all client in this channel
    server.mysend(guest->fd, RPL_INVITELIST(host_ni, host_us, IP, guest->nickname ,channelName));

    return;
}
