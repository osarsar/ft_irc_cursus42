#include "../../inc/manage.hpp"

manage::manage(SERVSOCKET &server) : Server(server) {
}

manage::~manage() {
}

void  manage::addChannel(const std::string &name) {
	Server.channel_map[name] = channel(name);
}
void manage::addClientoChannel(const std::string &name, client &client) {
	Server.channel_map[name].client_list.push_back(client);
}

bool manage::isClientInChannel(std::string ChannelName, SERVSOCKET &server, client &client) {
	std::map<std::string, channel>::const_iterator it = server.channel_map.find(ChannelName);
	for (;it != server.channel_map.end(); it++) {
		for (size_t i = 0; i < it->second.client_list.size(); i++) {
			if (it->second.client_list[i].nickname == client.nickname)
				return true;
		}
	}
	return false;
}