#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include "servsocket.hpp"
#include "channel.hpp"

class client
{
	public :
		std::string username;
		std::string hostname;
		std::string nickname;
		int client_fd;
		bool registration_check;
		bool has_allinfo;
		client() {registration_check = false, has_allinfo = false;}
};

#endif