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
	else if (str.substr(0, std::string(Smode).length()) == Smode)
		str.erase(0, std::string(Smode).length() + 1);
	std::cout << str << std::endl;
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
		throw (RED"Channel not found\n"RESET);
	if (i == 0)
		throw (RED"mode key is necessary\n"RESET);
	if ((key == "+k" || key == "+o" || key == "-o" || key == "+l") && mode_pass.empty())
		throw (RED"Invalid mode argument\n"RESET);
	execute_mode(key, server, Client, channel_name, mode_pass);
}


void	channel::execute_mode(std::string key, SERVSOCKET &server, client &Client, std::string channel_name, std::string mode_pass) {
	privmsg 	priv;
	std::map<std::string, channel>::iterator it = server.channel_map.find(channel_name);
	std::cout << Client.adminOf.size() << std::endl;
	for (size_t k = 0; k < Client.adminOf.size(); k++) {
		//check if a client requested it
		if (Client.adminOf[k] == channel_name) {
			if (key == "+k") {
				if (it != server.channel_map.end())
					it->second.channel_pass = mode_pass;
				priv.msg_to_channel(server, GREEN"This channel is now in Password Mode\n"RESET, channel_name, Client);
			}
			else if (!channel_pass.empty() && key == "-k") {
				channel_pass = "";
				priv.msg_to_channel(server, RED"This channel is no longer in Password Mode\n"RESET, channel_name, Client);
			}
			else if (key == "+o") {
				manage manage(server);
				std::cout << mode_pass << std::endl;
				if (!manage.give_privilege(mode_pass, channel_name, false))
					throw(RED"Client not found\n"RESET);
			}
			else if (key == "-o") {
				manage manage(server);
				if (!manage.give_privilege(mode_pass, channel_name, true))
					throw(RED"Client not found\n"RESET);
			}
			else if (key == "+l") {
				if (f_stoi(mode_pass) < (int)it->second.client_list.size())
					throw (RED"Can't limit channel with current value\n"RESET);
				else {
					max_clients = f_stoi(mode_pass);
					flag = true;
					throw (GREEN"This Channel is now limited\n"RESET);
				}
			}
			else if (key == "-l") {
				flag = false;
				throw (GREEN"Channel users limit has been sealed off"RESET);
			}
			else if (key == "+i") {
				Iflag = true;
				throw (GREEN"Channel in invite only mode\n"RESET);
			}
			else if (key == "-i") {
				Iflag = false;
				throw (GREEN"Invite only mode has been Removed\n"RESET);
			}
			// else if (key == "+t") {
			// 	throw (GREEN"Invite only mode has been Removed\n"RESET);
			// }
			// else if (key == "-t") {
			// 	throw (GREEN"Invite only mode has been Removed\n"RESET);
			// }
			return ;
		}
	}
	throw (RED"You are not an admin\n"RESET);
}