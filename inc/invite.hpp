#ifndef INVITE_HPP
# define INVITE_HPP


#include "channel.hpp"
#include "privmsg.hpp"
#include "servsocket.hpp"
#include "client.hpp"
#include "error.hpp"

class Invite
{
private:

public:
    Invite();
    ~Invite();

    void 	go_to_invite(std::string data, SERVSOCKET &server, int fd);
};

std::vector<std::string>    my_split(const std::string& input, char delimiter, SERVSOCKET server);
int                         is_operator(const std::vector<std::string>& operators, const std::string& client_name) ;
int                         check_client_in_channel(client guest, SERVSOCKET &server, std::string channel_name);
channel*                    get_channel(std::string channelName, SERVSOCKET &server);
client*                     get_client(std::string nickname, SERVSOCKET &server);
client*                     get_client(int fd, SERVSOCKET &server);
bool                        is_numeric(const std::string& str);

bool                        isInAdminOf(std::string str, std::vector<std::string> _adminOf);

#endif