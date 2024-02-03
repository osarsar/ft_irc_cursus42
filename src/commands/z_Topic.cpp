// TOPIC #test :New topic
// TOPIC #test :
// TOPIC #test
#include "../../inc/topic.hpp"
#include "../../inc/privmsg.hpp"

Topic::Topic()
{
}

Topic::~Topic()
{
}

void Topic::go_to_topic(std::string data, SERVSOCKET server)
{
    size_t	pos1;
    size_t	pos2;
    std::string newTopic;
    std::string channel_name;
    // std::string client_user;

    std::cout << "(" << data << ") : --> TOPIC\n";
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
            std::cout << "ERROR\n";
            return ;
        }
        channel_name = server.trim(channel_name);
        std::cout << "channel_name:" << channel_name << ".\n";
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
            std::cout << "newTopic:" << newTopic << ".\n";
            std::map <std::string, channel>::iterator iter_chnl;
            int check = 0;
            std::cout << "Checking the topic for \"" << channel_name << "\".\n";
            if (!channel_name.empty())
            {
                for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
                {
                    if (iter_chnl->first == channel_name)
                    {
                        std::cout << "Channel Existe\n";
                        check = 1;
                        std::cout << "1- Old Topic Is: \"" << iter_chnl->second.topic << "\".\n";
                        iter_chnl->second.topic = newTopic;
                        std::cout << "2- New Topic Is: \"" << iter_chnl->second.topic << "\".\n";
                        break;
                    }
                }
                if (check == 0)
                    std::cout << "Channel N'Existe Paaass\n";
            }
        }
        else
        {
            std::cout << "Clearing the topic on \"" << channel_name << "\".\n";
            //--> HERE (add clear for channel )
        }
	}
    else
    {
	    std::map <std::string, channel>::iterator iter_chnl;
        int check = 0;
        std::cout << "Checking the topic for \"" << channel_name << "\".\n";
        if (!channel_name.empty())
        {
            for (iter_chnl = server.channel_map.begin(); iter_chnl != server.channel_map.end(); iter_chnl++)
            {
                if (iter_chnl->first == channel_name)
                {
                    std::cout << "Channel Existe\n";
                    check = 1;
                    std::cout << "Topic is: \"" << iter_chnl->second.topic << "\".\n";
                    break;
                }
            }
            if (check == 0)
                std::cout << "Channel N'Existe Paaass\n";
        }
    }
}

// void Topic::TopicCommand(client* setter, const std::string& topicCommand)
// {
//     // Split the topicCommand into components
//     // Assume you have a utility function to split the string, e.g., splitString
//     std::vector<std::string> commandParts = splitString(topicCommand, ' ');

//     if (commandParts.size() >= 2) {
//         const std::string& channelName = commandParts[1];
//         if (channelName == name)
//         {
//             // The channel name in the command matches the current channel

//             if (commandParts.size() == 4 && commandParts[2] == ":") {
//     // Case 1: Setting the topic
//                 std::string newTopic = topicCommand.substr(topicCommand.find(':') + 1);
//                 setTopic(newTopic, setter);
//             } else if (commandParts.size() == 3 && commandParts[2] == ":") {
//     // Case 2: Clearing the topic
//                 setTopic("", setter);
//             } else if (commandParts.size() == 2) {
//     // Case 3: Checking the topic
//                 // Respond to the client with the current topic
//                 std::string currentTopic = getTopic();
//                 // Implement this based on your communication mechanism
//                 // It could be sending a message through a socket, writing to a file descriptor, etc.
//                 sender->send("RPL_TOPIC " + name + " :" + currentTopic);
//             }
//         } else {
//             // Channel name in the command doesn't match the current channel
//             // Respond with an error message
//             // Implement this based on your communication mechanism
//             // It could be sending a message through a socket, writing to a file descriptor, etc.
//             sender->send("ERR_NOSUCHCHANNEL " + channelName);
//         }
//     } else {
//         // Insufficient parameters in the command
//         // Respond with an error message
//         // Implement this based on your communication mechanism
//         // It could be sending a message through a socket, writing to a file descriptor, etc.
//         sender->send("ERR_NEEDMOREPARAMS TOPIC");
//     }
// }
