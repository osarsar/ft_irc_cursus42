#include "../../inc/privmsg.hpp"
#include <algorithm>
#include <cctype>
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

void    privmsg::parse_msg(std::string str, SERVSOCKET &server) {
	size_t	pos;

    if (str.substr(0, std::string(PRIVMSG).length()) == PRIVMSG)
        str.erase(0, std::string(PRIVMSG).length() + 1);
	if ((pos = str.find(":")) != std::string::npos) {
		std::string which(str.substr(0, pos));
		if (which[0] == '#')
			channel = which;
		else
			receiver = which;
		message = str.substr(pos + 1);
	}
	receiver = trim(receiver);
	std::vector<client>::iterator it = server.database.begin();
	for (;it != server.database.end();it++) {
		if (receiver == it->nickname)
			return ;
	}
	throw ("Client not found\n");
}

