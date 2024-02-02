#include "../../inc/client.hpp"


client::client() {
	registration_check = false;
	pass_bool = false;
	user_bool = false;
	nick_bool = false;
	fd = -1;
}
