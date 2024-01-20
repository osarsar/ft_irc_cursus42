#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include "servsocket.hpp"
#include "channel.hpp"
class channel;
class SERVSOCKET;
class client
{
	public :
		std::string username;
		std::string nickname;
		int  fd;
		bool registration_check;
		bool pass_bool;
		bool user_bool;
		bool nick_bool;
		client();
		void	AddClientToChannel(channel &Channel, SERVSOCKET &server);
};

#endif