#include "../../inc/channel.hpp"

channel::channel(void) {}

channel::~channel(void) {}

int	f_stoi( std::string numb ) {
	int		ret;

	std::stringstream var(numb);
	var >> ret;
	return ret;
}

void	channel::join(std::string str, client &Client, 	SERVSOCKET &server) {
	// channel Channel;
	char *p;
	std::string command = str.substr(0, str.find(" "));
	if (str.substr(0, std::string(JOIN).length()) == JOIN)
		str.erase(0, std::string(JOIN).length() + 1);
	// else if (str.substr(0, std::string(MODE).length()) == MODE) {
	// 	str.erase(0, std::string(MODE).length() + 1);
	// 	mode_parse(str);
	// 	return ;
	// }
	// else
		// throw ("Unknown command\n");
	int i = 0;
	p = std::strtok(const_cast<char *>(str.c_str()), ", \r\n");
	while (p != NULL) {
		if (p[0] == '#') {
			channelName = p;
			i++;
		}
		else
			channel_pass = p;
		p = std::strtok(NULL, ", \n");
	}
	if (!channelName.empty()) {
		server.add_channel(channelName, *this);
		Client.AddClientToChannel(*this, server);
	}
	std::map<std::string, channel>::iterator it = server.channel_map.begin();
	for (; it != server.channel_map.end();it++) {
		std::cout << "------> hahua dkhal" << std::endl;
		std::vector<client>::iterator iter = it->second.client_list.begin();
		for (int i = 0; i < this->client_list.size(); i++)
		{
		std::cout << "------+|" << this->client_list[i].nickname << std::endl;

		}
		// for (; iter != it->second.client_list.end(); iter++) {
			// std::cout << "------> hahua dkhal 2" << std::endl;
			// std::cout << (iter)->nickname << std::endl;
		// }
		std::cout << it->second.channelName << std::endl;
	}
	// server.show();
}