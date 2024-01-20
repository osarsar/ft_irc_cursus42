#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include "servsocket.hpp"
#include "channel.hpp"

class client
{
	public :
		std::string username;
		std::string nickname;
        // std::vector<client> database;

		int  fd;
		bool registration_check;
		bool pass_bool;
		bool user_bool;
		bool nick_bool;
		// void push(std::string nickname);
		client() {registration_check = false, pass_bool = false, user_bool = false, nick_bool = false, fd = -1;}
};

#endif

// void push(std::string password)
// {
// 	database.push_back();
// }