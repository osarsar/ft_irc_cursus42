#include "../../inc/manage.hpp"
#include "../../inc/error.hpp"

channel::channel(void) {
	max_clients = 0;
	flag = false;
	Iflag = false;
	Tflag = false;
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

int channel::fill(std::string str) {
	char *p;
	int argumentcount = 0;
	if (str.substr(0, std::string(JOIN).length()) == JOIN)
		str.erase(0, std::string(JOIN).length() + 1);
	else if (str.substr(0, std::string(Sjoin).length()) == Sjoin)
		str.erase(0, std::string(Sjoin).length() + 1);
	p = std::strtok(const_cast<char *>(str.c_str()), ", \t\r\n");
	while (p != NULL)
	{	
		argumentcount++;
		if (p[0] == CHANNEL)
			NameVec.push_back(p);
		else
			PassVec.push_back(p);
		p = std::strtok(NULL, ", \t\r\n");
	}
	return (argumentcount);
}

void channel::join(std::string Name, std::string Password, client &Client, SERVSOCKET &server)
{
	channelName = Name;
	manage manage(server);
	std::map<std::string, channel>::iterator iter = server.channel_map.find(channelName);
	//Client is already in channel
	if (manage.isClientInChannel(channelName, server, Client)) {
		server.mysend(Client.fd, ERR_USERONCHANNEL(std::string(server.client_ip), Name, Client.nickname));
		return ;
	}
	//Check if the channel is already created
	if (iter == server.channel_map.end())
		manage.addChannel(channelName, Client, Password);
	// Ask for Password if it is necessary
	else if (iter != server.channel_map.end() && !iter->second.channel_pass.empty())
		if (!join_password(iter->second.channel_pass, Client, server)) {
			server.mysend(Client.fd, ERR_BADCHANNELKEY(Client.nickname, std::string(server.client_ip), channelName));
			return ;
		}
	//Check if the channel is limited
	if (max_clients < (int)iter->second.client_list.size() + 1 && flag) {
		server.mysend(Client.fd, ERR_CHANNELISFULL(Client.nickname, std::string(server.client_ip)));
		return ;
	}
	//Check invited users
	if (iter->second.invited_users.size() == 0 && Iflag) {
		server.mysend(Client.fd, ERR_INVITEONLY(Client.nickname, std::string(server.client_ip)));
		return ;
	}
	for (int k = 0; k != (int)iter->second.invited_users.size() && Iflag; k++) {
		if (Client.nickname != iter->second.invited_users[k] && k + 1 == (int)iter->second.invited_users.size()) {
			server.mysend(Client.fd, ERR_INVITEONLY(Client.nickname, std::string(server.client_ip)));
			return ;
		}
	}
	//Add Client to Channel
	manage.addClientoChannel(channelName, Client);
}

bool channel::join_password(std::string password, client &Client, SERVSOCKET &server)
{
	adminMap[Client.nickname] = false;
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