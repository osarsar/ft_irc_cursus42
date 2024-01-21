#include "../../inc/manage.hpp"

manage::manage(SERVSOCKET &server) : Server(server) {
}

manage::~manage() {
}

void  manage::addChannel(const std::string &name) {
	Server.channel_map[name] = channel(name);
}
void manage::addClienttoChannel(const std::string &name, client &client) {
	Server.channel_map[name].client_list.push_back(client);
}