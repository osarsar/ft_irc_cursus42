#include "../../inc/Kick.hpp"

Kick::Kick(){}

Kick::~Kick(){}

void toUpper(std::string& str)
{
    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        *it = std::toupper(*it);
    }
}

void 	Kick::go_to_kick(std::string data, SERVSOCKET &server, int fd)
{
    client *host;
    host = get_client(fd, server);

    std::string host_ni = host->nickname;
    std::string host_us = host->username;

    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 3)
    {
        server.mysend(fd, ERR_NEEDMOREPARAMS(host_ni, "KICK"));
        return ;
    }
    
    size_t	pos1;
    size_t	pos2;
    std::string channel_name;
    std::string message;
    std::string client_nick;


    if ((pos1 = data.find(":")) != std::string::npos)
    {
        message = data.substr(pos1 + 1);
        message = server.trim(message);
        // std::cout << "message:" << message << ".\n";
	}

    if ((pos2 = data.find(" ")) != std::string::npos)
    {
        channel_name = data.substr(pos2 + 1, pos1 - pos2 - 1);
        client_nick = channel_name.substr(channel_name.find(" ") + 1);
        channel_name = channel_name.substr(0, channel_name.find(" "));
        if (channel_name[0] != '#' && channel_name[0] != '&')
        {
            server.mysend(fd, ERR_BADCHANMASK(channel_name));
            return ;
        }
        channel_name = server.trim(channel_name);
        client_nick = server.trim(client_nick);
        // std::cout << "channel_name:" << channel_name << ".\n";
        // std::cout << "client_nick:" << client_nick << ".\n";
	}
    else
    {
        server.mysend(fd, ERR_NEEDMOREPARAMS(host_ni, "KICK"));
        return ;
    }

    if (check_client_in_channel(*host, server, channel_name) == 0)
    {
        server.mysend(fd, ERR_NOTONCHANNEL(host_ni, channel_name));
        return;
    }

	std::map <std::string, channel>::iterator iter_chnl;
    std::vector<client>::iterator iter_clt;
    

    if (!channel_name.empty())
	{
        int check = 0;
        // std::cout << "1-->>\n";

		for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
        {
			if (iter_chnl->first == channel_name)
            {
                // std::cout << "Channel Existe\n";
                check = 1;
                int i = 0;
                for (iter_clt = iter_chnl->second.client_list.begin(); iter_clt != iter_chnl->second.client_list.end();iter_clt++)
                {
                    if (iter_clt->nickname == client_nick)
                    {
                        // std::cout << "i-> " << i << ". Client Existe\n";
                        check = 2;
                        std::string cause;
                        if (message.empty()) {
                            cause = "You are Kicked.\n";
                        }
                        else
                            cause = "You are Kicked because: " + message + "\n";
                        
                        std::vector<std::string>::iterator invtd_usrs2;
                        int j = 0;
                        for (invtd_usrs2 = iter_chnl->second.invited_users.begin(); invtd_usrs2 != iter_chnl->second.invited_users.end(); invtd_usrs2++)
                        {
                            std::cout << "1 ->." << *invtd_usrs2 << ".\n";
                            if (*invtd_usrs2 == client_nick)
                            {
                                iter_chnl->second.invited_users.erase(iter_chnl->second.invited_users.begin() + j);
                                break;
                            }
                            j++;
                        }

                        iter_chnl->second.client_list.erase(iter_chnl->second.client_list.begin() + i);

                        iter_chnl->second.kicked_users.push_back(client_nick);
                        // --> HERE kick from all inv, oper, normal ; + kik
                        break;
				    }
                    i++;
				}
                break;
            }
		}
        if (check == 0)
        {
            server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channel_name));
            return;
        }
        else if (check == 1)
        {
            server.mysend(fd, ERR_NOSUCHNICK(host_ni, client_nick));
            // std::cout << "No such nick/channel\n";
            return;
        }
	}

    server.mysend(fd, RPL_KICK(host_ni, host_us, IP, iter_clt->nickname, channel_name));

    server.mysend(iter_clt->fd, RPL_KICK(host_ni, host_us, IP, iter_clt->nickname, channel_name));
}
// kick from all inv, oper, normal ; + kik