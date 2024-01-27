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
	manage(SERVSOCKET &server);
	manage();
	~manage();
	void addChannel(const std::string &name, client &client);
	void give_privilege(std::string clientName, std::string name);
	void addClientoChannel(const std::string &name, client &client);
	bool isClientInChannel(std::string ChannelName, SERVSOCKET &server, client &Client);
};