#ifndef TOPIC_HPP
# define TOPIC_HPP


#include "channel.hpp"

//--------------------------------------------------->>> Temsa:

class Topic
{
private:
    
public:
    Topic();
    ~Topic();

    void    go_to_topic(std::string data, SERVSOCKET server);
    void    TopicCommand(client* setter, std::string& topicCommand);
};


#endif