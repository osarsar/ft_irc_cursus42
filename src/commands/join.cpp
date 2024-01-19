#include "../../inc/channel.hpp"

channel::channel(void) {}

channel::~channel(void) {}

int	f_stoi( std::string numb ) {
	int		ret;

	std::stringstream var(numb);
	var >> ret;
	return ret;
}

void	channel::add_client(client *client) {
	client_list.push_back(client);
}

void	channel::join_parse(std::string str) {
	SERVSOCKET *server = new SERVSOCKET();
	channel *Channel = NULL;
	char *p;
	std::string command = str.substr(0, str.find(" "));
	if (str.substr(0, std::string(JOIN).length()) == JOIN)
		str.erase(0, std::string(JOIN).length() + 1);
	// else if (str.substr(0, std::string(MODE).length()) == MODE) {
	// 	str.erase(0, std::string(MODE).length() + 1);
	// 	mode_parse(str);
	// 	return ;
	// }
	else
		throw ("Unknown command");
	int i = 0;
	p = std::strtok(const_cast<char *>(str.c_str()), ", \r\n");
	while (p != NULL) {
		if (p[0] == '#') {
			std::vector < std::string >::iterator pos = std::find(channel_name.begin(), channel_name.end(), p);
			if (pos != channel_name.end())
				throw ("Duplicate channel");
			channel_name.push_back(p);
			i++;
		}
		else
			channel_pass.push_back(p);
		p = std::strtok(NULL, ", \n");
	}
	if (channel_name.empty() || i == 0)
		throw ("command alone can't function");
	std::vector<std::string>::iterator it = channel_name.begin();
	for (;it != channel_name.end();it++) {
		Channel = server->add_channel(*it, Channel);
	}
}
