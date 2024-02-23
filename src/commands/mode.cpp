#include "../../inc/channel.hpp"
#include "../../inc/manage.hpp"
#include "../../inc/error.hpp"

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
	if (it == server.channel_map.end()) {
		server.mysend(Client.fd, ERR_MODENOSUCHCHANNEL(std::string(server.client_ip), channel_name, Client.nickname));
		return ;
	}
	if (i == 0) {
		server.mysend(Client.fd, ERR_INVALIDMODEPARAM(channel_name, std::string(server.client_ip), key));
		return ;
	}	
	if ((key == "+k" || key == "+o" || key == "-o" || key == "+l") && mode_pass.empty()) {
		server.mysend(Client.fd, ERR_INVALIDMODEPARAM(channel_name, std::string(server.client_ip), key));
		return ;
	}
	execute_mode(key, server, Client, channel_name, mode_pass);
}


void	channel::execute_mode(std::string key, SERVSOCKET &server, client &Client, std::string channel_name, std::string mode_pass) {
	privmsg 	priv;
	std::map<std::string, channel>::iterator it = server.channel_map.find(channel_name);
	for (size_t k = 0; k < Client.adminOf.size(); k++) {
		//check if a client requested it
		if (Client.adminOf[k] == channel_name) {
			// add a Password
			if (key == "+k") {
				if (it != server.channel_map.end())
					it->second.channel_pass = mode_pass;
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key + " " + mode_pass));
			}
			// Remove a Password
			else if (!channel_pass.empty() && key == "-k") {
				channel_pass = "";
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key));
			}
			// ADD an Admin
			else if (key == "+o") {
				manage manage(server);
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key + " " + mode_pass));
			}
			// Remove an Admin
			else if (key == "-o") {
				manage manage(server);
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key + " " + mode_pass));
			}
			// Mode to Limit Channel users
			else if (key == "+l") {
				if (f_stoi(mode_pass) < (int)it->second.client_list.size())
					throw (RED"Can't limit channel with current value\n"RESET);
				else {
					max_clients = f_stoi(mode_pass);
					flag = true;
					server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key + " " + mode_pass));
				}
			}
			// Remove The Channel limit
			else if (key == "-l") {
				flag = false;
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key));
			}
			// Make the Channel invite-only
			else if (key == "+i") {
				Iflag = true;
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key));
			}
			// Remove the invite-only mode
			else if (key == "-i") {
				Iflag = false;
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key));
			}
			// Remove the ability to user Topic command on regular users
			else if (key == "+t") {
				Tflag = true;
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key));
			}
			// Change it back to normal
			else if (key == "-t") {
				Tflag = false;
				server.mysend(Client.fd, RPL_MODEIS(channel_name, std::string(server.client_ip), key));
			}
			return ;
		}
	}
	server.mysend(Client.fd, ERR_NOTOP(std::string(server.client_ip), channel_name));
}