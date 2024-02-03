#include "../../inc/invite.hpp"
#include "../../inc/privmsg.hpp"
#include "../../inc/servsocket.hpp"
#include "../../inc/client.hpp"
#include "../../inc/error.hpp"

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

client* get_client(int fd, SERVSOCKET server)
{
    std::vector<client>::iterator iter_clt;
    for (iter_clt = server.database.begin(); iter_clt != server.database.end();iter_clt++)
    {
        if (iter_clt->fd == fd)
            return &(*iter_clt);// HERE
    }
    return NULL;
}

client* get_client(std::string nickname, SERVSOCKET server)
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
    (void) fd;

    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 3)
    {
       server.mysend(fd, "ERR_NEEDMOREPARAMS INVITE\n");
        return ;
    }
    client *host;
    client *guest;

    host = get_client(fd, server);
    if (!host)
    {
        server.mysend(fd, "ERR_NOTREGISTERED\n");
        return;
    }

    guest = get_client(commands[1], server);
    if (!guest)
    {
        server.mysend(fd, "ERR_NOSUCHNICK (client n'existe pas) HERE ---> i dkn what the message\n");
        return;
    }

    std::string channelName = commands[2];
    channel *channel = get_channel(channelName, server);
    if (!channel)
    {
        throw (std::runtime_error(ERR_NOSUCHCHANNEL(host->nickname, channelName)));
        server.mysend(fd, "ERR_NOSUCHCHANNEL\n");
        return;
    }

    std::cout << "host: " << host->nickname << ".\n";
    if (check_client_in_channel(*host, server, channelName) == 0)
    {
        server.mysend(fd, "ERR_NOTONCHANNEL\n");
        return;
    }
    std::cout << "host on channel, " << host->nickname << ".\n";

    if (check_client_in_channel(*guest, server, channelName) == 1)
    {
       server.mysend(fd, "ERR_USERONCHANNEL\n");
        return;
    }
    
    // if (!is_operator(channel->operators, host->nickname))
    // {
    //    server.mysend(fd, "ERR_CHANOPRIVSNEEDED\n");
    //     return;
    // }

    channel->client_list.push_back(*guest);

    if (check_client_in_channel(*guest, server, channelName) == 1)
    {
        server.mysend(fd, "RPL_INVITING\n");
        server.mysend(fd, "Added successfully\n");
        return;
    }
}
