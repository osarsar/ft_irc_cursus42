#include "../../inc/manage.hpp"
#include "../../inc/error.hpp"

manage::manage(SERVSOCKET &server) : Server(server) {
}

manage::~manage() {
}

bool manage::give_privilege(std::string clientName, std::string name, bool flag) {
	std::map<std::string, channel>::iterator it = Server.channel_map.find(name);
	if (it != Server.channel_map.end()) {
		for (size_t index = 0; index < Server.database.size(); index++) {
			if (Server.database[index].nickname == clientName) {
				for (size_t i = 0; i < it->second.client_list.size(); i++) {
					if (it->second.client_list[i].nickname == clientName && !flag) {
						for (size_t k = 0; k < Server.database[index].adminOf.size(); k++) {
							if (Server.database[index].adminOf[k] == name)
								std::cout << "Client " << clientName << " already an admin" << RESET << std::endl;
						}
						Server.database[index].adminOf.push_back(name);
						std::cout << BLUE << "Hey Wake up, " << clientName << " You have been granted the Admin status" << RESET << std::endl;
						return true;
					}
					else if (flag) {
						for (size_t j = 0; j < Server.database[index].adminOf.size(); j++) {
							if (Server.database[index].adminOf[j] != name)
								std::cout << RED << "Client " << clientName << " already lost his privileges" << RESET << std::endl;
							if (Server.database[index].adminOf[j] == name) {
								Server.database[index].adminOf[j].erase();
								std::cout << RED << "It is with great sorrow to inform " << clientName << " that your admin privileges has been lost" << RESET << std::endl;
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

void manage::addClientoChannel(const std::string &name, client &newClient) {
	bool flag = false;
    channel& channel = Server.channel_map[name];
    
    channel.client_list.push_back(newClient);
 
    std::string namesReply;
    for (size_t i = 0; i < channel.client_list.size(); ++i) {
        if (i != 0) {
            namesReply += " ";
        }
		for (int k = 0; k < (int)channel.client_list[i].adminOf.size(); k++) {
			if (channel.client_list[i].adminOf[k] == name)
				flag = true;
		}
		if (flag) {
   	    	namesReply += "@" + channel.client_list[i].nickname;
			flag = false;
		}
        else 
			namesReply += channel.client_list[i].nickname;
    } 

    Server.mysend(newClient.fd, RPL_JOIN(newClient.nickname, newClient.username, name, Server.client_ip));
    for (size_t i = 0; i < channel.client_list.size(); ++i) {
        Server.mysend(newClient.fd, RPL_NAMREPLY(std::string(Server.client_ip), namesReply, name, newClient.nickname));
		break ;
    }

    Server.mysend(newClient.fd, RPL_ENDOFNAMES(std::string(Server.client_ip), newClient.nickname, name));
	for (size_t i = 0; i < channel.client_list.size(); ++i) {
		if (channel.client_list[i].nickname != newClient.nickname)
    		Server.mysend(channel.client_list[i].fd, RPL_JOIN(newClient.nickname, newClient.username, name, Server.client_ip));
    }
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