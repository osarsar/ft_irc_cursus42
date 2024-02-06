#include "../../inc/manage.hpp"


manage::manage(SERVSOCKET &server) : Server(server) {
}

manage::~manage() {
}

bool manage::give_privilege(std::string clientName, std::string name, bool flag) {
	std::cout << clientName << std::endl;
	std::map<std::string, channel>::iterator it = Server.channel_map.find(name);
	const char *grant = BLUE"Hey Wake up, You have been granted the Admin status\n"RESET;
	const char *seize = RED"It is with great sorrow to inform you that your admin privileges has been lost\n"RESET;
	if (it != Server.channel_map.end()) {
		for (size_t index = 0; index < Server.database.size(); index++) {
			if (Server.database[index].nickname == clientName) {
				std::cout << Server.database[index].nickname << std::endl;
				for (size_t i = 0; i < it->second.client_list.size(); i++) {
					if (it->second.client_list[i].nickname == clientName && !flag) {
						for (size_t k = 0; k < Server.database[index].adminOf.size(); k++) {
							if (Server.database[index].adminOf[k] == name)
								throw(RED"Client already an admin\n"RESET);
						}
						Server.database[index].adminOf.push_back(name);
						send(Server.database[index].fd, grant, std::string(grant).length(), 0);
						return true;
					}
					else if (flag) {
						for (size_t j = 0; j < Server.database[index].adminOf.size(); j++) {
							if (Server.database[index].adminOf[j] != name)
								throw (RED"Client already lost his privileges\n"RESET);
							if (Server.database[index].adminOf[j] == name) {
								Server.database[index].adminOf[j].erase();
								send(Server.database[index].fd, seize, std::string(seize).length(), 0);
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

void  manage::addChannel(const std::string &name, client &client) {
	if (Server.channel_map.find(name) == Server.channel_map.end()) {
        Server.channel_map[name] = channel(name);
    }
    client.adminOf.push_back(name);
    std::cout << GREEN << "An IRC channel " << name << " was created by User " << client.nickname << RESET << std::endl;
}

void manage::addClientoChannel(const std::string &name, client &client) {
	Server.channel_map[name].client_list.push_back(client);
	throw (ORANGE"You have joined an IRC channel\n"RESET);
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