#pragma once


#include "channel.hpp"
#include "client.hpp"
#include "servsocket.hpp"

class client;
class manage
{
	private:
		SERVSOCKET &Server;
	public:
	int	max_clients;
	manage(SERVSOCKET &server);
	manage();
	~manage();
	size_t getlimit();
	void addChannel(const std::string &name, client &client);
	bool give_privilege(std::string clientName, std::string name, bool flag);
	void addClientoChannel(const std::string &name, client &client);
	bool isClientInChannel(std::string ChannelName, SERVSOCKET &server, client &Client);
	void limit_clients(std::string str);
};