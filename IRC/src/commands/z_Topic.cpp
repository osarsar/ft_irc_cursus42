#include "../../inc/topic.hpp"

Topic::Topic()
{
}

Topic::~Topic()
{
}

void Topic::go_to_topic(std::string data, SERVSOCKET &server, int fd)
{
    size_t	pos1;
    size_t	pos2;
    std::string newTopic;
    std::string channel_name;

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
    if (commands.size() < 2)
    {            
        server.mysend(fd, ERR_NEEDMOREPARAMS(host_ni, "TOPIC", host->ip));
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
            if (data == "TOPIC :")
                channel_name = ".";
            server.mysend(fd, ERR_BADCHANMASK(channel_name, host->ip));
            return ;
        }
        channel_name = server.trim(channel_name);
	}
    //check (:);
    if (pos1 != std::string::npos || commands.size() >= 3)
    {
        data = server.trim(data);
        if (data[pos1 + 1] != '\0')
        {
            std::string newTopic1;
            std::string newTopic2;
            newTopic = data.substr(pos1 + 1);
            newTopic = server.trim(newTopic);
            if (commands.size() >= 3)
            {
                newTopic1 = commands[commands.size() - 1];
            }
            int a = 1;
            newTopic = data.substr(pos1 + 1);
            size_t pos11 = newTopic.find(":");
            if (pos11 != std::string::npos)
            {
                a = 2;
                newTopic = newTopic.substr(pos11 + 1);
                newTopic2 = server.trim(newTopic);
            }
            if (newTopic1[0] == ':')
                newTopic1 = newTopic1.substr(1);
            if (newTopic2[0] == ':')
                newTopic2 = newTopic2.substr(1);
            if (a == 2)
                newTopic = newTopic2;
            else
                newTopic = newTopic1;
            if (!channel_name.empty())
            {
                std::map <std::string, channel>::iterator iter_chnl;
                for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
                {
                    if (iter_chnl->first == channel_name)
                    {
                        std::vector<client>::iterator iter_clt;
                        for (iter_clt = iter_chnl->second.client_list.begin(); iter_clt != iter_chnl->second.client_list.end();iter_clt++)
                        {
                            if (iter_clt->nickname == host_ni)
                            {
                                if (iter_chnl->second.Tflag == true)
                                {
                                    if (!isInAdminOf(channel_name, host->adminOf))
                                    {
                                        server.mysend(fd, ERR_CHANOPRIVSNEEDED(host_ni, channel_name, host->ip));
                                        return;
                                    }
                                }
                                iter_chnl->second.topic = newTopic;
                                privmsg obj;
                                obj.msg_to_channel(server, RPL_TOPIC(host_ni, host_us, host->ip, newTopic, channel_name), channel_name, *host, true);
                                server.mysend(fd, RPL_TOPIC(host_ni, host_us, host->ip, newTopic, channel_name));
                                return;
                            }
                        }
                        server.mysend(fd, ERR_NOTONCHANNEL(host_ni, channel_name, host->ip));
                        return;
                    }
                }
                server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channel_name, host->ip));
                return;
            }
        }
        else
        {
            std::map <std::string, channel>::iterator iter_chnl;
            if (!channel_name.empty())
            {
                for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
                {
                    if (iter_chnl->first == channel_name)
                    {
                        std::vector<client>::iterator iter_clt;
                        for (iter_clt = iter_chnl->second.client_list.begin(); iter_clt != iter_chnl->second.client_list.end();iter_clt++)
                        {
                            if (iter_clt->nickname == host_ni)
                            {
                                if (iter_chnl->second.Tflag == true)
                                {
                                    if (!isInAdminOf(channel_name, host->adminOf))
                                    {
                                        server.mysend(fd, ERR_CHANOPRIVSNEEDED(host_ni, channel_name, host->ip));
                                        return;
                                    }
                                }
                                iter_chnl->second.topic = "";
                                privmsg obj;
                                obj.msg_to_channel(server, RPL_TOPIC_CLEAR(host_ni, host_us, host->ip, channel_name), channel_name, *host, true);
                                server.mysend(fd, RPL_TOPIC_CLEAR(host_ni, host_us, host->ip, channel_name));
                                return;
                            }
                        }
                        server.mysend(fd, ERR_NOTONCHANNEL(host_ni, channel_name, host->ip));
                        return;
                    }
                }
                server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channel_name, host->ip));
                return;
            }
        }
	}
    else
    {
	    std::map <std::string, channel>::iterator iter_chnl;
        if (!channel_name.empty())
        {
            for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
            {
                if (iter_chnl->first == channel_name)
                {
                    std::vector<client>::iterator iter_clt;
                    for (iter_clt = iter_chnl->second.client_list.begin(); iter_clt != iter_chnl->second.client_list.end();iter_clt++)
                    {
                        if (iter_clt->nickname == host_ni)
                        {
                            if (iter_chnl->second.topic.empty())
                            {
                                server.mysend(fd, RPL_NOTOPIC(host_ni, channel_name, host->ip));
                                return;
                            }
                            server.mysend(fd, RPL_TOPIC_DISPLAY(host_ni, host_us, host->ip, iter_chnl->second.topic, channel_name));
                            return;
                        }
                    }
                    server.mysend(fd, ERR_NOTONCHANNEL(host_ni, channel_name, host->ip));
                    return;
                }
            }
            server.mysend(fd, ERR_NOSUCHCHANNEL(host_ni, channel_name, host->ip));
            return;
        }
    }
}
