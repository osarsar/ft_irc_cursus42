#include "../../inc/Kick.hpp"
#include "../../inc/privmsg.hpp"
#include "../../inc/error.hpp"
#include "../../inc/servsocket.hpp"


Kick::Kick(){}

Kick::~Kick(){}

void 	Kick::go_to_kick(std::string data, SERVSOCKET &server)
{
    std::cout << "(" << data << ") : --> KICK\n";
    
    size_t	pos1;
    size_t	pos2;
    std::string channel_name;
    std::string message;
    std::string client_nick;


    if ((pos1 = data.find(":")) != std::string::npos)
    {
        message = data.substr(pos1 + 1);
        message = server.trim(message);
        std::cout << "message:" << message << ".\n";
	}

    if ((pos2 = data.find(" ")) != std::string::npos)
    {
        channel_name = data.substr(pos2 + 1, pos1 - pos2 - 1);
        client_nick = channel_name.substr(channel_name.find(" ") + 1);
        channel_name = channel_name.substr(0, channel_name.find(" "));
        if (channel_name[0] != '#' && channel_name[0] != '&')
        {
            std::cout << "ERROR\n";
            return ;
        }
        channel_name = server.trim(channel_name);
        client_nick = server.trim(client_nick);
        
        std::cout << "channel_name:" << channel_name << ".\n";
        std::cout << "client_nick:" << client_nick << ".\n";
	}
    else
    {
        std::cout << "ERROR\n";
        return ;
    }

	std::map <std::string, channel>::iterator iter_chnl;
    std::vector<client>::iterator iter_clt;
    

    if (!channel_name.empty())
	{
        int check = 0;
        std::cout << "1-->>\n";

		for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
        {
			if (iter_chnl->first == channel_name)
            {
                std::cout << "Channel Existe\n";
                check = 1;
                int i = 0;
                for (iter_clt = iter_chnl->second.client_list.begin(); iter_clt != iter_chnl->second.client_list.end();iter_clt++)
                {
                    if (iter_clt->nickname == client_nick)
                    {
                        std::cout << "i-> " << i << ". Client Existe\n";
                        check = 2;
                        std::string cause;
                        if (message.empty()) {
                            cause = "You are Kicked.\n";
                        }
                        else
                            cause = "You are Kicked because: " + message + "\n";
                        int result = send(iter_clt->fd, cause.c_str(), cause.length(), 0);
                        if (result == -1) {
                            perror("send");
                        }
                        iter_chnl->second.client_list.erase(iter_chnl->second.client_list.begin() + i);
                        break;
				    }
                    i++;
				}
                break;
            }
		}
        if (check == 0)
            std::cout << "Channel N'Existe Paaass\n";
        else if (check == 1)
        {
            std::cout << "Client N'Existe Paaass\n";
            // throw (std::runtime_error(ERR_USERNOTINCHANNEL(iter_clt->username, client_nick, channel_name)));
        }
	}
}
