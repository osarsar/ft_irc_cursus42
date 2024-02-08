// TOPIC #test :New topic
// TOPIC #test :
// TOPIC #test
#include "../../inc/topic.hpp"


Topic::Topic()
{
}

Topic::~Topic()
{
}

void Topic::go_to_topic(std::string data, SERVSOCKET &server, int fd)
{
    // std::cout << "Topic 1.\n";

    size_t	pos1;
    size_t	pos2;
    std::string newTopic;
    std::string channel_name;

    // std::cout << "(" << data << ") : --> TOPIC\n";
    client *host;
    host = get_client(fd, server);

    std::string host_ni = host->nickname;
    std::string host_us = host->username;

    std::vector<std::string> commands = my_split(data, ' ', server);
    if (commands.size() < 2)
    {
        server.mysend(fd, ERR_NEEDMOREPARAMS(host_ni, "TOPIC"));
        return ;
    }

    data = server.trim(data);
    pos1 = data.find(":");
    pos2 = data.find(" ");
    //get channel_name;
    if (pos2 != std::string::npos)
    {
        channel_name = data.substr(pos2 + 1, pos1 - pos2 - 1);
        channel_name = channel_name.substr(0, channel_name.find(" "));
        if (channel_name[0] != '#' && channel_name[0] != '&')
        {
            server.mysend(fd, ERR_BADCHANMASK(channel_name));
            return ;
        }
        channel_name = server.trim(channel_name);
        // std::cout << "channel_name:" << channel_name << ".\n";
	}
    //check (:);
    if (pos1 != std::string::npos)
    {
        data = server.trim(data);
        //check ( :newTopic );
        if (data[pos1 + 1] != '\0')
        {
            newTopic = data.substr(pos1 + 1);
            newTopic = server.trim(newTopic);
            // std::cout << "newTopic:" << newTopic << ".\n";
            std::map <std::string, channel>::iterator iter_chnl;
            int check = 0;
            // std::cout << "Checking the topic for \"" << channel_name << "\".\n";
            if (!channel_name.empty())
            {
                for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
                {
                    if (iter_chnl->first == channel_name)
                    {
                        // std::cout << "Channel Existe\n";
                        check = 1;
                        // std::cout << "1- Old Topic Is: \"" << iter_chnl->second.topic << "\".\n";
                        iter_chnl->second.topic = newTopic;
                        // std::cout << "2- New Topic Is: \"" << iter_chnl->second.topic << "\".\n";
                        // HERE send for all client in this channel
                        server.mysend(fd, RPL_TOPIC(host_ni, host_us, IP, newTopic, channel_name));

                        break;
                    }
                }
                if (check == 0)
                {
                    server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channel_name));
                    return;
                }
            }
        }
        else
        {
            //--> HERE (Check is operator) pour toute action (ATTENTION)
            std::map <std::string, channel>::iterator iter_chnl;
            if (!channel_name.empty())
            {
                for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
                {
                    if (iter_chnl->first == channel_name)
                    {
                        iter_chnl->second.topic = "";
                        server.mysend(fd, RPL_TOPIC_CLEAR(host_ni, host_us, IP, channel_name));
                        return;
                    }
                }
                server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channel_name));
                return;
            }
            //--> HERE (add clear for channel )
        }
	}
    else
    {
	    std::map <std::string, channel>::iterator iter_chnl;
        int check = 0;
        // std::cout << "Checking the topic for \"" << channel_name << "\".\n";
        if (!channel_name.empty())
        {
            for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
            {
                if (iter_chnl->first == channel_name)
                {
                    // std::cout << "Channel Existe\n";
                    if (iter_chnl->second.topic.empty())
                    {
                        server.mysend(fd, RPL_NOTOPIC(host_ni, channel_name));
                        return;
                    }
                    server.mysend(fd, RPL_TOPIC_DISPLAY(host_ni, host_us, IP, iter_chnl->second.topic, channel_name));
                    check = 1;
                    break;
                }
            }
            if (check == 0)
            {
                server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channel_name));
                return;
            }
        }
    }
}

// mode #general +o benito

//--> HERE (Check is operator) pour toute action (ATTENTION)
// HERE send for all client in this channel
