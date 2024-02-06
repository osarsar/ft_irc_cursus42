#include "../../inc/manage.hpp"

channel::channel(void) {
	max_clients = 0;
	flag = false;
}

channel::~channel(void) {}


int f_stoi(std::string numb)
{
	int ret;

	std::stringstream var(numb);
	for (int i = 0; (size_t)i < numb.length(); i++)
		if (isdigit(numb[i]) == 0)
			throw (RED"Error: is Not number\n"RESET);
	if (!(var >> ret))
		throw (RED"Error: Overflow\n"RESET);
	return ret;
}

void channel::join(std::string str, client &Client, SERVSOCKET &server)
{
	char *p;
	int argumentcount = 0;
	if (str.substr(0, std::string(JOIN).length()) == JOIN)
		str.erase(0, std::string(JOIN).length() + 1);
	else if (str.substr(0, std::string(Sjoin).length()) == Sjoin)
		str.erase(0, std::string(Sjoin).length() + 1);
	p = std::strtok(const_cast<char *>(str.c_str()), ", \r\n");
	while (p != NULL)
	{	
		argumentcount++;
		if (p[0] == CHANNEL)
			channelName = p;
		else
			channel_pass = p;
		p = std::strtok(NULL, ", \n");
	}
	if (argumentcount > 2 || argumentcount < 1)
		throw (RED"Join Invalid arguments\n"RESET);
	channelName = server.trim(channelName);
	channel_pass = server.trim(channel_pass);
	if (channelName[0] == '#' && std::isalpha(channelName[1]))
	{
		std::map<std::string, channel>::iterator iter = server.channel_map.find(channelName);
		manage manage(server);
		if (iter == server.channel_map.end())
			manage.addChannel(channelName, Client);
		if (manage.isClientInChannel(channelName, server, Client))
			throw(RED "client is already in channel\n" RESET);
		else if (!channel_pass.empty() && iter->second.channel_pass.empty())
			iter->second.channel_pass = channel_pass;
		if (!iter->second.channel_pass.empty() && argumentcount == 1) {
			if (!join_password(iter->second.channel_pass, Client, server))
				throw ("Password is incorrect\n");
		}
		if (max_clients < (int)iter->second.client_list.size() + 1 && flag)
			throw(RED"Channel has been limited\n"RESET);
		manage.addClientoChannel(channelName, Client);
	}
	else
		throw (RED"Join Command Error\n"RESET);
}

bool channel::join_password(std::string password, client &Client, SERVSOCKET &server)
{
	std::string passmsg = "Please provide the necessary password here : ";
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