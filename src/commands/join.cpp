#include "../../inc/manage.hpp"

channel::channel(void) {
	passmsg = "Please provide the necessary password here : ";
}

channel::~channel(void) {}

int f_stoi(std::string numb)
{
	int ret;

	std::stringstream var(numb);
	var >> ret;
	return ret;
}

void channel::join(std::string str, client &Client, SERVSOCKET &server)
{
	char *p;
	std::string command = str.substr(0, str.find(" "));
	int i = 0;
	if (str.substr(0, std::string(JOIN).length()) == JOIN)
		str.erase(0, std::string(JOIN).length() + 1);
	p = std::strtok(const_cast<char *>(str.c_str()), ", \r\n");
	while (p != NULL)
	{
		if (p[0] == CHANNEL)
		{
			channelName = p;
			i++;
		}
		else
			channel_pass = p;
		p = std::strtok(NULL, ", \n");
	}
	channelName = server.trim(channelName);
	channel_pass = server.trim(channel_pass);
	if (channelName.empty())
		throw (RED"Join commmand can't function alone\n"RESET);
	std::map<std::string, channel>::iterator iter = server.channel_map.find(channelName);
	manage manage(server);
	if (iter == server.channel_map.end()) {
		manage.addChannel(channelName, Client);
	}
	if (manage.isClientInChannel(channelName, server, Client))
		throw(RED "client is already in channel\n" RESET);
 	if (!channel_pass.empty() && Client.adminOf != channelName) {
		if (!join_password(channel_pass, Client, server))
			throw ("Password is incorrect\n");
		channel_pass = "";
	}
	manage.addClientoChannel(channelName, Client);
	std::map<std::string, channel>::iterator it = server.channel_map.begin();
	for (; it != server.channel_map.end(); it++)
	{
		for (unsigned int i = 0; i < it->second.client_list.size(); i++)
			std::cout << "------+|" << it->second.client_list[i].nickname << std::endl;
		std::cout << it->second.channelName << std::endl;
	}
	std::cout << "---------------->>>>" << std::endl;
	std::cout << "End of channels" << std::endl;
	std::cout << "------------------" << std::endl;
}

bool channel::join_password(std::string password, client &Client, SERVSOCKET &server)
{
    send(Client.fd, passmsg.c_str(), passmsg.length(), 0);
    const int MAX_PASSWORD_LENGTH = 100;
    char userpass[MAX_PASSWORD_LENGTH];
    memset(userpass, 0, sizeof(userpass));
	std::string receivedPass;
    recv(Client.fd, userpass, sizeof(userpass) - 1, 0);
	receivedPass = userpass;
	receivedPass = server.trim(receivedPass);
    if (receivedPass == password)
        return true;
    return false;
}