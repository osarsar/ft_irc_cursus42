#include "../../inc/channel.hpp"

void	channel::mode(std::string str) {
	char *p;
	int i = 0;
	std::string mode_pass;
	std::string key;
	std::string channel;

	if (str.substr(0, std::string(MODE).length()) == MODE)
		str.erase(0, std::string(MODE).length() + 1);
	p = std::strtok(const_cast<char *>(str.c_str()), ", \r\n");
	while (p != NULL) {
		if (p[0] == CHANNEL && channel.empty())
			channel = p;
		else if ((p[0] == POS_KEY || p[0] == NEG_KEY) && key.empty()) {
			key = p;
			i++;
		}
		else if (p[0] != NEG_KEY && p[0] != POS_KEY && p[0] != CHANNEL && mode_pass.empty())
			mode_pass = p;
		p = std::strtok(NULL, ", \n");
	}
	// std::vector < std::string >::iterator pos = std::find(channels.begin(), channels.end(), channel);
	// if (pos == channels.end())
		// throw ("Channel not found");
	// if (i == 0)
		// throw ("mode key is necessary");
}