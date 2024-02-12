#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <vector>
#include "servsocket.hpp"
#include "channel.hpp"
class channel;
class SERVSOCKET;
class client
{
	public :
	// HERE Temsa
		std::string buff_data;
		std::string username;
		std::vector<std::string> adminOf;
		std::string nickname;
		int  fd;
		bool registration_check;
		bool pass_bool;
		bool user_bool;
		bool nick_bool;
		client();
};

#endif