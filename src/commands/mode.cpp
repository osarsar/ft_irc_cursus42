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
	if (key == "+k" && Client.adminOf == channel_name)
		channel_pass = mode_pass;
	else if (!channel_pass.empty() && key == "-k" && Client.adminOf == channel_name)
		channel_pass = "";
	else if (Client.adminOf == channel_name && key == "+o") {
		manage manage(server);
		manage.give_privilege(server, mode_pass, channel_name);
		std::cout << "->>>> " << Client.adminOf << std::endl;
	}
}
