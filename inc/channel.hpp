#ifndef CHANNEL_HPP
# define CHANNEL_HPP


#include "servsocket.hpp"
#include "privmsg.hpp"
#include "client.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#define JOIN "JOIN"
#define MODE "MODE"
#define PASS "PASS"

#define Sjoin "join"
#define Smode "mode"
#define Spass "pass"

#define PART "PART"
#define SPART "part"

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
class SERVSOCKET;
// Here --> majiscule
class channel
{
	public :
		bool flag;
		bool Iflag;
		bool Tflag;
		std::string channelName;
		std::string channel_pass;
		std::map <std::string, bool> adminMap;
		std::vector < std::string > NameVec;
		std::vector < std::string > PassVec;
		std::string topic;
		int			max_clients;
		std::vector < std::string > kicked_users;
		std::vector < std::string > invited_users;
		std::vector <client> client_list;
		channel();
		channel(const std::string& name) : channelName(name) {}
		~channel();
		bool	get_bool();
		void	join(std::string Name, std::string Password, client &Client, SERVSOCKET &server);
		void	mode(std::string str, SERVSOCKET &server, client &Client);
		void	part(std::string Name, client &Client, SERVSOCKET &server);
		void	execute_mode(std::string key, SERVSOCKET &server, client &Client, std::string channel_name, std::string channel_pass);
		bool	join_password(std::string password, client &Client, SERVSOCKET &server);
		int		fill(std::string str);
};
int	f_stoi(std::string numb);
std::ostream& operator<<(std::ostream& os, const channel& c);
#endif