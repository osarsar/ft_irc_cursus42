#ifndef ERROR_HPP
# define ERROR_HPP

#include "channel.hpp"

// clt -> client
// chnl -> channel
// cmd -> command
// tpc -> topic
// nik -> nick
// setat -> setat

#define IP "127.0.0.1"

// "<client> <command> :Not enough parameters"
// "<client> <channel> :No such channel"
// "<client> <channel> :You're not on that channel"
// "<client> <channel> :You're not channel operator"
// "<client> <channel> :No topic is set"
// "<client> <channel> :<topic>"
// "<client> <channel> <nick> <setat>"

#define ERR_NEEDMOREPARAMS(nick, cmd)               ":1337.Temsa.ma 461 " + std::string(nick) + " " + std::string(cmd) + " :Not enough parameters\r\t\n"
#define ERR_NOSUCHCHANNEL(nick, chnl)               ":1337.Temsa.ma 403 " + std::string(nick) + " " + std::string(chnl) + " :No such channel\r\t\n"
#define ERR_NOTONCHANNEL(nick, chnl)                ":1337.Temsa.ma 442 " + std::string(nick) + " " + std::string(chnl) + " :You're not on that channel\r\t\n"
#define ERR_CHANOPRIVSNEEDED(nick, chnl)            ":1337.Temsa.ma 482 " + std::string(nick) + " " + std::string(chnl) + " :You're not a channel operator\r\t\n"
#define ERR_NOTREGISTERED(nick)                      std::string(nick) + " :You have not registered"

//>>>>>>>> TOPIC --------------------------------------------------------->>>>>>>>>>>>>>>>>>
#define RPL_NOTOPIC(clt, chnl)                      ":1337.Temsa.ma 331 " + std::string(clt) + " " + std::string(chnl) + " :No topic is set\r\t\n"
#define RPL_TOPIC(hN, hU, ip, new_tpc,chnl)         ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " TOPIC " + std::string(chnl) + " : " + std::string(new_tpc) + "\r\t\n"
#define RPL_TOPIC_CLEAR(hN, hU, ip, chnl)           ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " TOPIC " + std::string(chnl) + " :" + "\r\t\n"
#define RPL_TOPIC_DISPLAY(hN, hU, ip, new_tpc,chnl) ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " TOPIC " + std::string(chnl) + " : " + std::string(new_tpc) + " TOPIC " + std::string(chnl) + "\r\t\n"
#define RPL_TOPICWHOTIME(clt, chnl, nik, setat)     std::string(clt) + " " + std::string(chnl) + " " + std::string(nik) + " " + std::string(setat)

//>>>>>>>> KICK --------------------------------------------------------->>>>>>>>>>>>>>>>>>
#define ERR_USERNOTINCHANNEL(clt, nik, chnl)        std::string(clt) + " " + std::string(nik) + " " + std::string(chnl) + " :They aren't on that channel"
#define ERR_BADCHANMASK(chnl)                       ":1337.Temsa.ma 476 " + std::string(chnl) + " :Bad Channel Mask\r\t\n"
#define RPL_KICK(hN, hU, ip, gN,chnl)               ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " KICK " + std::string(chnl) + " " + std::string(gN)  + " :" + std::string(gN) + "\r\t\n"

//>>>>>>>> INVITE --------------------------------------------------------->>>>>>>>>>>>>>>>>>
#define ERR_NOSUCHNICK(nick, nameToDo)              ":1337.Temsa.ma 401 " + std::string(nick) + " " + std::string(nameToDo) + " :No such nick/channel\r\t\n"
#define ERR_USERONCHANNEL(nik1, nik2, chnl)         ":1337.Temsa.ma 443 " + std::string(nik1) + " " + std::string(nik2) + " " + std::string(chnl) + " :is already on channel\r\t\n"
#define RPL_INVITING(nik, nik2, chnl)               ":1337.Temsa.ma 341 " + std::string(nik) + " " + std::string(nik2) + " " + std::string(chnl) + "\r\t\n"
#define RPL_INVITELIST(hN, hU, ip, gN,chnl)         ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " INVITE " + std::string(gN) + " :" + std::string(chnl) + "\r\t\n"
#define RPL_ENDOFINVITELIST(nik)                    std::string(nik) + " :End of /INVITE list"


#endif

