#ifndef INVITE_HPP
# define INVITE_HPP


#include "channel.hpp"

//--------------------------------------------------->>> Temsa:

class Invite
{
private:

public:
    Invite();
    ~Invite();

    void 	go_to_invite(std::string data, SERVSOCKET &server, int fd);
};

std::vector<std::string> my_split(const std::string& input, char delimiter, SERVSOCKET server);


#endif