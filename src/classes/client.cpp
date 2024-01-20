#include "../../inc/client.hpp"

client::client() {
	registration_check = false;
	pass_bool = false;
	user_bool = false;
	nick_bool = false;
	fd = -1;
}

void	client::AddClientToChannel(channel &Channel) {
	Channel.client_list.push_back(*this);
}
