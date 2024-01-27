#include "../../inc/manage.hpp"

manage::manage(SERVSOCKET &server) : Server(server) {
}

manage::~manage() {
}

bool manage::give_privilege(std::string clientName, std::string name, bool flag) {
	std::map<std::string, channel>::iterator it = Server.channel_map.find(name);
	const char *grant = BLUE"Hey Wake up, You have been granted the Admin status\n"RESET;
	const char *seize = RED"It is with great sorrow to inform you that your admin privileges has been lost\n"RESET;
	if (it != Server.channel_map.end()) {
		for (size_t index = 0; index < Server.database.size(); index++) {
			if (Server.database[index].nickname == clientName) {
				for (size_t i = 0; i < it->second.client_list.size(); i++) {
					if (it->second.client_list[i].nickname == clientName && !flag) {
						if (Server.database[index].adminOf == name)
							throw(RED"Client already an admin\n"RESET);
						Server.database[index].adminOf = name;
						send(Server.database[index].fd, grant, std::string(grant).length(), 0);
						return true;
					}
					else if (flag) {
						Server.database[index].adminOf = "";
						send(Server.database[index].fd, seize, std::string(seize).length(), 0);
						return true;
					}
				}
			}
		}
	}
	return false;
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
	std::map<std::string, channel>::iterator it = server.channel_map.find(ChannelName);
	if (it != server.channel_map.end()) {
		for (size_t i = 0; i < it->second.client_list.size(); i++) {
			if (it->second.client_list[i].nickname == client.nickname)
				return true;
		}
	}
	return false;
}