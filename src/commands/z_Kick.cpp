#include "../../inc/Kick.hpp"

Kick::Kick(){}

Kick::~Kick(){}

void toUpper(std::string& str)
{
    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        *it = std::toupper(*it);
    }
}

std::vector<std::string> get_client_kick(std::string &client_nick, SERVSOCKET &server)
{
    std::vector<std::string> list_client_kick = my_split(client_nick, ',', server);
    return (list_client_kick);
}

int kill_him_or_no(std::string &iter_clt_nickname, std::vector<std::string> &list_client_kick)
{
    std::vector<std::string>::iterator it;
    for (it = list_client_kick.begin(); it != list_client_kick.end(); it++)
    {
        if (*it == iter_clt_nickname)
            return 1;        
    }
    return 0;
}

void 	Kick::go_to_kick(std::string data, SERVSOCKET &server, int fd)
{
    client *host;
    host = get_client(fd, server);

    std::string host_ni = host->nickname;
    std::string host_us = host->username;

    std::vector<std::string> commands0 = my_split(data, ' ', server);
    if ((commands0.size() <= 3) && commands0[commands0.size() - 1] == ":")
    {
        data.erase(data.size() - 1);
        server.trim(data);
    }

    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 3)
    {
        server.mysend(fd, ERR_NEEDMOREPARAMS(host_ni, "KICK", host->ip));
        return ;
    }

    size_t	pos1;
    size_t	pos2;
    std::string channel_name;
    std::string message = "";
    std::string client_nick;
    std::vector<std::string> list_client_kick;

    if ((pos1 = data.find(":")) != std::string::npos)
    {
        message = data.substr(pos1 + 1);
        message = server.trim(message);
	}

    if ((pos2 = data.find(" ")) != std::string::npos)
    {
        channel_name = data.substr(pos2 + 1, pos1 - pos2 - 1);
        client_nick = channel_name.substr(channel_name.find(" ") + 1);
        channel_name = channel_name.substr(0, channel_name.find(" "));
        if (channel_name[0] != '#' && channel_name[0] != '&')
        {
            server.mysend(fd, ERR_BADCHANMASK(channel_name, host->ip));
            return ;
        }
        channel_name = server.trim(channel_name);
        client_nick = server.trim(client_nick);
	}
    else
    {
        server.mysend(fd, ERR_NEEDMOREPARAMS(host_ni, "KICK", host->ip));
        return ;
    }
    
    list_client_kick = get_client_kick(client_nick, server);

    if (check_client_in_channel(*host, server, channel_name) == 0)
    {
        server.mysend(fd, ERR_NOTONCHANNEL(host_ni, channel_name, host->ip));
        return;
    }

    if (!isInAdminOf(channel_name, host->adminOf))
    {
        server.mysend(fd, ERR_CHANOPRIVSNEEDED(host_ni, channel_name, host->ip));
        return;
    }

	std::map <std::string, channel>::iterator iter_chnl;
    std::vector<client>::iterator iter_clt;
    std::vector<int> vect_fd;

    if (!channel_name.empty())
	{
		for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
        {
			if (iter_chnl->first == channel_name)
            {
                int i = 0;
                int check = 0;
                for (iter_clt = iter_chnl->second.client_list.begin(); iter_clt != iter_chnl->second.client_list.end();iter_clt++)
                {
                    vect_fd.push_back(iter_clt->fd);
                    if (kill_him_or_no(iter_clt->nickname, list_client_kick) == 1)
                    {
                        int j = 0;
                        check = 1;
                        std::vector<std::string>::iterator invtd_usrs2;
                        for (invtd_usrs2 = iter_chnl->second.invited_users.begin(); invtd_usrs2 != iter_chnl->second.invited_users.end(); invtd_usrs2++)
                        {
                            if (*invtd_usrs2 == client_nick)
                            {
                                iter_chnl->second.invited_users.erase(iter_chnl->second.invited_users.begin() + j);
                                break;
                            }
                            j++;
                        }
                        iter_chnl->second.kicked_users.push_back(iter_clt->nickname);
                        iter_clt = iter_chnl->second.client_list.erase(iter_clt);
                        --iter_clt;
				    }
                    i++;
				}
                if (check == 1)
                {
                    std::vector<int>::iterator iti_fd;
                    for (iti_fd = vect_fd.begin(); iti_fd != vect_fd.end(); iti_fd++)
                    {
                        server.mysend(*iti_fd, RPL_KICK(host_ni, host_us, host->ip, channel_name, client_nick, message));
                    }
                    server.mysend(fd, RPL_KICK(host_ni, host_us, host->ip, channel_name, client_nick, message));
                    return;
                }
                server.mysend(fd, ERR_NOSUCHNICK(host_ni, channel_name, client_nick));
                return;
            }
		}
        server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channel_name, host->ip));
        return;
	}
}
