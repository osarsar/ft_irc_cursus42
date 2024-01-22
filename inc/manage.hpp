#pragma once

#include "channel.hpp"
#include "client.hpp"
#include "servsocket.hpp"


class manage
{
	private:
		SERVSOCKET &Server;

public:
	manage(SERVSOCKET &server);

	manage();
	~manage();
	void addChannel(const std::string &name);
	void addClientoChannel(const std::string &name, client &client);

};