#include "../../inc/privmsg.hpp"
#include "../../inc/error.hpp"

privmsg::privmsg() {}

privmsg::~privmsg() {}


void    privmsg::parse_msg(std::string str, SERVSOCKET &server, client &Client) {
	std::string channel_receive;
	std::string receiver;
	size_t	pos;
	int 	fd;

    if (str.substr(0, std::string(PRIVMSG).length()) == PRIVMSG)
        str.erase(0, std::string(PRIVMSG).length() + 1);
	if (str.substr(0, std::string(LPRIVMSG).length()) == LPRIVMSG)
        str.erase(0, std::string(LPRIVMSG).length() + 1);
	if ((pos = str.find(":")) != std::string::npos) {
		std::string which(str.substr(0, pos));
		if (which[0] == '#')
			channel_receive = which;
		else
			receiver = which;
		message = str.substr(pos);
	}
	else
	{
		if ((pos = str.find(" ")) != std::string::npos) {
			if (str[0] == '#')
				channel_receive = str.substr(0, pos);
			else
				receiver = str.substr(0, pos);
			message = str.substr(pos + 1, str.find(" "));
		}
	}
	receiver = server.trim(receiver);
	channel_receive = server.trim(channel_receive);
	message += '\n';
 	std::vector<client>::iterator it = server.database.begin();
	for (;it != server.database.end();it++) {
		if (receiver == it->nickname) {
			fd = client_fd(receiver, server);
			break ;
		}
	}
	if (it == server.database.end() && channel_receive.empty())
		server.mysend(Client.fd, ERR_NOSUCHNICK(std::string(server.client_ip), channel_receive, receiver));
	if (!channel_receive.empty())
		msg_to_channel(server, message, channel_receive, Client, false);
	if (!receiver.empty())
		msg_to_client(fd, message, receiver, Client, server);
}

int	privmsg::client_fd(std::string str, SERVSOCKET &server) {
	std::vector<client>::iterator it = server.database.begin();
	for (;it != server.database.end();it++) {
		if (it->nickname == str)
			return (it->fd);
	}
	return (0);
}

void	privmsg::msg_to_channel(SERVSOCKET &server, std::string message, std::string receiver, client &Client, bool flag)
{
	std::vector<int> fds_vector;
	std::vector<client>::iterator vec_it;
	std::vector<int>::iterator iti;
	std::map<std::string, channel>::iterator iter = server.channel_map.find(receiver);
	if (iter != server.channel_map.end()) 
	{
		for (vec_it = iter->second.client_list.begin(); vec_it != iter->second.client_list.end();vec_it++) {
			fds_vector.push_back(vec_it->fd);
		}
	}
	else
	{
		server.mysend(Client.fd, ERR_MODENOSUCHCHANNEL(std::string(server.client_ip), receiver, Client.nickname));
		return ;
	}
	unsigned long i = 0;
	if (!flag)
		message = ":" + Client.nickname + "!" + Client.username + "@" + server.client_ip + " PRIVMSG " + receiver + " " + message;
	for (iti = fds_vector.begin(); i++ < fds_vector.size() && iti != fds_vector.end(); iti++) {
			if (*iti != Client.fd)
				send(*iti, message.c_str(), message.length(), 0);
		}

}

void	privmsg::msg_to_client(int fd, std::string message, std::string receiver, client &Client, SERVSOCKET &server) {
	message = ":" + Client.nickname + "!" + Client.username + "@" + server.client_ip + " PRIVMSG " + receiver + " " + message;
	send(fd, message.c_str(), message.length(), 0);
}