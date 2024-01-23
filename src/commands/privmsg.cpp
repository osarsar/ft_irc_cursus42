#include "../../inc/privmsg.hpp"

privmsg::privmsg() {}

privmsg::~privmsg() {}

std::string& ltrim(std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start != std::string::npos) {
        str.erase(0, start);
    } else {
        str.clear();
    }
    return str;
}

std::string& rtrim(std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r");
    if (end != std::string::npos) {
        str.erase(end + 1);
    } else {
        str.clear();
    }
    return str;
}

std::string& trim(std::string& str) {
    return ltrim(rtrim(str));
}

void    privmsg::parse_msg(std::string str, SERVSOCKET &server, client &Client, channel &Channel) {
	(void)Channel;
	size_t	pos;
	int 	fd;
	std::vector<int> fds_vector;
	std::vector<client>::iterator vec_it;
	std::vector<int>::iterator iti;
	std::map<std::string, channel>::iterator iter;
    if (str.substr(0, std::string(PRIVMSG).length()) == PRIVMSG)
        str.erase(0, std::string(PRIVMSG).length() + 1);
	if ((pos = str.find(":")) != std::string::npos) {
		std::string which(str.substr(0, pos));
		if (which[0] == '#')
			channel_receive = which;
		else
			receiver = which;
		message = str.substr(pos + 1);
	}
	receiver = trim(receiver);
	channel_receive = trim(channel_receive);
	message += '\n';
 	std::vector<client>::iterator it = server.database.begin();
	for (;it != server.database.end();it++) {
		if (receiver == it->nickname) {
			fd = client_fd(receiver, server);
			break ;
		}
	}
	for (iter = server.channel_map.begin(); iter != server.channel_map.end(); iter++) {
		if (iter->first == channel_receive)
			for (vec_it = iter->second.client_list.begin(); vec_it != iter->second.client_list.end();vec_it++) {
				fds_vector.push_back(vec_it->fd);
			}
	}
	unsigned long i = 0;
	while (i++ < fds_vector.size()) {
		for (iti = fds_vector.begin(); iti != fds_vector.end(); iti++) {
			send(*iti, message.c_str(), message.length(), 0);
		}
	}
	// if (it == server.database.end())
	// 	throw ("Client not found\n");
	if (!receiver.empty()) {
		std::cout << "hh" << std::endl;
		msg_to_client(fd, message, Client);
	}
}

int	privmsg::client_fd(std::string str, SERVSOCKET &server) {
	std::vector<client>::iterator it = server.database.begin();
	for (;it != server.database.end();it++) {
		if (it->nickname == str)
			return (it->fd);
	}
	return (0);
}

void	privmsg::msg_to_client(int fd, std::string message, client &Client) {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::string timeString = std::ctime(&currentTime);
    timeString.pop_back();
	std::ofstream file("./logs/logs.spyware", std::ios::app);
	send(fd, message.c_str(), message.length(), 0);
	file << timeString << " [" << Client.nickname << "] sent"<<" to [" << receiver << "] ~~ " << message;
}