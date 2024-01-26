#include "../../inc/manage.hpp"

manage::manage(SERVSOCKET &server) : Server(server) {
}

manage::~manage() {
}

void  manage::give_privilege(SERVSOCKET &server, std::string clientName, std::string name) {
	std::map<std::string, channel>::iterator it = server.channel_map.find(name);
	std::vector<client>::iterator cli;
	if (it != server.channel_map.end())
	{
		for (cli = it->second.client_list.begin(); cli != it->second.client_list.end(); cli++) {
			if (cli->nickname == clientName) {
				cli->adminOf = name;
				break ;
			}
		}
		if (cli != it->second.client_list.end())
			throw (RED"Client is already an admin\n"RESET);
	}
}

void  manage::addChannel(const std::string &name, client &client) {
	std::map<std::string, channel>::iterator it = Server.channel_map.find(name);
	if (it == Server.channel_map.end())
		client.adminOf = name;
	Server.channel_map[name] = channel(name);
}
void manage::addClientoChannel(const std::string &name, client &client) {
	Server.channel_map[name].client_list.push_back(client);
}

bool manage::isClientInChannel(std::string ChannelName, SERVSOCKET &server, client &client) {
	std::map<std::string, channel>::iterator it = server.channel_map.find(ChannelName);;
	if (it != server.channel_map.end()) {
		for (size_t i = 0; i < it->second.client_list.size(); i++) {
			if (it->second.client_list[i].nickname == client.nickname)
				return true;
		}
	}
	return false;
}