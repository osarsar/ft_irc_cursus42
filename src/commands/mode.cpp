#include "../../inc/channel.hpp"
#include "../../inc/manage.hpp"

void	channel::mode(std::string str, SERVSOCKET &server, client &Client) {
	char *p;
	int i = 0;
	std::string mode_pass;
	std::string key;
	std::string channel_name;
	
	if (str.substr(0, std::string(MODE).length()) == MODE)
		str.erase(0, std::string(MODE).length() + 1);
	p = std::strtok(const_cast<char *>(str.c_str()), ", \r\n");
	while (p != NULL) {
		if (p[0] == CHANNEL && channel_name.empty())
			channel_name = p;
		else if ((p[0] == POS_KEY || p[0] == NEG_KEY) && key.empty()) {
			key = p;
			i++;
		}
		else if (p[0] != NEG_KEY && p[0] != POS_KEY && p[0] != CHANNEL && mode_pass.empty())
			mode_pass = p;
		p = std::strtok(NULL, ", \n");
	}
	key = server.trim(key);
	mode_pass = server.trim(mode_pass);
	channel_name = server.trim(channel_name);
	std::map<std::string, channel>::iterator it = server.channel_map.find(channel_name);
	if (it == server.channel_map.end())
		throw ("Channel not found\n");
	if (i == 0)
		throw ("mode key is necessary\n");
	execute_mode(key, server, Client, channel_name, mode_pass);
}

void	channel::execute_mode(std::string key, SERVSOCKET &server, client &Client, std::string channel_name, std::string mode_pass) {
	privmsg 	priv;
	if (Client.adminOf == channel_name) {
		if (key == "+k") {
			channel_pass = mode_pass;
			priv.msg_to_channel(server, GREEN"This channel is now in Password Mode\n"RESET, channel_name, Client);
		}
		else if (!channel_pass.empty() && key == "-k") {
			channel_pass = "";
			priv.msg_to_channel(server, RED"This channel is no longer in Password Mode\n"RESET, channel_name, Client);
		}
		else if (key == "+o") {
			manage manage(server);
			if (!manage.give_privilege(mode_pass, channel_name, false))
				throw(RED"Client not found\n"RESET);
		}
		else if (key == "-o") {
			manage manage(server);
			if (!manage.give_privilege(mode_pass, channel_name, true))
				throw(RED"Client not found\n"RESET);
		}
	}
	else
		throw (RED"You are not an admin\n"RESET);
}