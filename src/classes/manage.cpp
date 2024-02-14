#include "../../inc/manage.hpp"
#include "../../inc/error.hpp"

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

void  manage::addChannel(const std::string &name, client &client, std::string password) {
	if (Server.channel_map.find(name) == Server.channel_map.end()) {
        Server.channel_map[name] = channel(name);
		Server.channel_map[name].channel_pass = password;
    }
    client.adminOf.push_back(name);
	Server.channel_map[name].adminMap[client.nickname] = true;
    std::cout << GREEN << "An IRC channel " << name << " was created by User " << client.nickname << RESET << std::endl;
}

void manage::addClientoChannel(const std::string &name, client &client) {
	Server.channel_map[name].client_list.push_back(client);
	Server.mysend(client.fd, RPL_JOIN(client.nickname, client.username, name, Server.client_ip));
	Server.mysend(client.fd, RPL_NAMREPLY(std::string(Server.client_ip), client.nickname, name, client.nickname));
	Server.mysend(client.fd, RPL_ENDOFNAMES(std::string(Server.client_ip), client.nickname, name));
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