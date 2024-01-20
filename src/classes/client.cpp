#include "../../inc/client.hpp"

client::client() {
	registration_check = false;
	pass_bool = false;
	user_bool = false;
	nick_bool = false;
	fd = -1;
}

void	client::AddClientToChannel(channel &Channel, SERVSOCKET &server) {
	Channel.client_list.push_back(*this);
	std::cout << this << std::endl;
	std::cout << Channel.client_list.size() << std::endl;
	// std::cout << Channel.client_list[Channel.client_list.size() - 1].nickname << " has joined this channel " << Channel.channelName << std::endl; 
	// std::cout << nickname << " has joined this channel " << Channel.channelName << std::endl; 
}
