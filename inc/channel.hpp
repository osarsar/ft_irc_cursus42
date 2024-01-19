#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "servsocket.hpp"
#include "client.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#define JOIN "join"
#define MODE "mode"
#define PASS "PASS"

#define BLUE      "\x1b[34m"
#define ORANGE    "\x1b[33m"
#define RESET     "\x1b[0m"
#define PURPLE    "\x1b[96m"
#define GREEN     "\x1b[32m"
#define RED       "\x1b[1;31m"
#define CHANNEL '#'
#define POS_KEY '+'
#define NEG_KEY '-'
class client;
class channel
{
	public :
		std::string channelName;
		std::string topic;
		std::vector < std::string > channel_name;
		std::vector < std::string > channel_pass;
		std::vector < std::string > kicked_users;
		std::vector < std::string > banned_users;
		std::vector < std::string > operators;
		std::vector < client *> client_list;
		channel(void);
		channel(const std::string& name) : channelName(name) {}
		std::string getName() const { return channelName;}
		~channel();
		void	join_parse(std::string str);
		void	add_client(client *client);
		void	mode_parse(std::string str);
};
int	f_stoi(std::string numb);
std::ostream& operator<<(std::ostream& os, const channel& c);
#endif