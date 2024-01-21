#include "../../inc/channel.hpp"
#include "../../inc/manage.hpp"

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
	// server.add_channel(channelName, *this);

	manage manage(server);
	manage.addChannel(channelName);
	manage.addClienttoChannel(channelName, Client);

	// std::map<std::string, channel>::iterator iti = server.channel_map.begin();
	// std::cout << iti->first << std::endl;
	// exit(0);
	// server.channel_map[channelName] = channel(channelName);
	// Client.AddClientToChannel(*this);
	std::map<std::string, channel>::iterator it = server.channel_map.begin();
	for (; it != server.channel_map.end();it++) {
		for (unsigned int i = 0; i < it->second.client_list.size(); i++)
			std::cout << "------+|" << it->second.client_list[i].nickname << std::endl;
		std::cout << it->second.channelName << std::endl;
	}
	std::cout << "---------------->>>>" << std::endl;
	std::cout << "End of channels" << std::endl;
	std::cout << "------------------" << std::endl;
}