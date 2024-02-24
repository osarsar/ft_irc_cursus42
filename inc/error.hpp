#ifndef ERROR_HPP
# define ERROR_HPP

#include "channel.hpp"

// clt -> client
// chnl -> channel
// cmd -> command
// tpc -> topic
// nik -> nick
// setat -> setat

#define IP "10.11.4.10"

// "<client> <command> :Not enough parameters"
// "<client> <channel> :No such channel"
// "<client> <channel> :You're not on that channel"
// "<client> <channel> :You're not channel operator"
// "<client> <channel> :No topic is set"
// "<client> <channel> :<topic>"
// "<client> <channel> <nick> <setat>"

#define ERR_NEEDMOREPARAMS(cid, cmd)               ":1337.Temsa.ma 461 " + std::string(cid) + " " + std::string(cmd) + " :Not enough parameters\r\t\n"
#define ERR_NOSUCHCHANNEL(nick, chnl)               ":1337.Temsa.ma 403 " + std::string(nick) + " " + std::string(chnl) + " :No such channel\r\t\n"
#define ERR_NOTONCHANNEL(nick, chnl)                ":1337.Temsa.ma 442 " + std::string(nick) + " " + std::string(chnl) + " :You're not on that channel\r\t\n"
#define ERR_CHANOPRIVSNEEDED(nick, chnl)            ":1337.Temsa.ma 482 " + std::string(nick) + " " + std::string(chnl) + " :You're not a channel operator\r\t\n"
#define ERR_NOTREGISTERED(nick)                     ":1337.Temsa.ma 451 " + std::string(nick) + " :You have not registered"

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
// #define ERR_NOSUCHNICK(nick, nameToDo)              ":1337.Temsa.ma 401 " + std::string(nick) + " " + std::string(nameToDo) + " :No such nick/channel\r\t\n"
// #define ERR_USERONCHANNEL(nik1, nik2, chnl)         ":1337.Temsa.ma 443 " + std::string(nik1) + " " + std::string(nik2) + " " + std::string(chnl) + " :is already on channel\r\t\n"
#define RPL_INVITING(nik, nik2, chnl)               ":1337.Temsa.ma 341 " + std::string(nik) + " " + std::string(nik2) + " " + std::string(chnl) + "\r\t\n"
#define RPL_INVITELIST(hN, hU, ip, gN,chnl)         ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " INVITE " + std::string(gN) + " :" + std::string(chnl) + "\r\t\n"
#define RPL_ENDOFINVITELIST(nik)                    std::string(nik) + " :End of /INVITE list"

//-----------ADDED------------------//
#define ERR_NICKCOLLISION(nick, cmd)                                        ":1337.Temsa.ma 436 " + std::string(nick) + " " + std::string(cmd) + " :Nickname collision KILL\r\t\n"
#define RPL_WELCOME(cid, name, nick, user, host)                            ":1337.Temsa.ma 001 " + std::string(nick) + " :Welcome to the " + name + " Network, " + std::string(nick) + "!" + std::string(user) + "@" + std::string(host) + "\r\t\n"
#define	RPL_YOURHOST(cid, servername, version)		                        ":1337.Temsa.ma 002 " + std::string(cid) + " :Your host is " + std::string(servername) + ", running version " + std::string(version) + "\r\t\n"
#define	RPL_CREATED(cid, date)						                        ":1337.Temsa.ma 003 " + std::string(cid) + " :This server was created " + std::string(date) + "\r\t\n"
#define RPL_MYINFO(cid, username, version, avmode, cmode, modepara)         ":1337.Temsa.ma 004 " + std::string(cid) + " " + std::string(username) + " " + std::string(version) + " " + std::string(avmode) + " " + std::string(cmode) + " [" + std::string(modepara) + "]\r\t\n"
#define ERR_PASSWDMISMATCH(cid)						                        ":1337.Temsa.ma 464 " + std::string(cid) + " :Password incorrect\r\t\n"
#define ERR_ALREADYREGISTRED(cid)						                    ":1337.Temsa.ma 462 " + std::string(cid) + " :Unauthorized command (already registered)\r\t\n"
#define	ERR_UNKNOWNCOMMAND(cid, cmd)						                ":1337.Temsa.ma 421 " + std::string(cid) + " " + std::string(cmd) + " :Unknown command\r\t\n" 
#define	ERR_NICKNAMEINUSE(cid, nick)						                ":1337.Temsa.ma 433 " + std::string(cid) + " " + std::string(nick) + " :Nickname is already in use\r\t\n"
//>>>>>>>> JOIN --------------------------------------------------------->>>>>>>>>>>>>>>>>>
# define RPL_JOIN(nick, username, channelname, ipaddress)                   ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
# define RPL_NAMREPLY(hostname, clients, channelname,nick)                  ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
# define RPL_ENDOFNAMES(hostname, nick, channelname)                        ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"
# define ERR_USERONCHANNEL(hostname, channel, nick)                         ":" + hostname + " 443 " + nick + " " + channel + "  :is already on channel\r\n"
# define ERR_NOSUCHNICK(hostname, channel, argument)                        ":" + hostname + " 401 " + channel + " " +  argument + " :No such nick/channel\r\n"
# define ERR_CHANNELISFULL(nick, hostname)                                  ":" + hostname + " 471 " + channelName + " :Cannot join channel (+l)\r\n"
# define ERR_INVITEONLY(nick, hostname)                                     ":" + hostname + " 473 " + channelName + " :Cannot join channel (+i)\r\n"
# define ERR_JOINPASSWDMISMATCH(nick, hostname)	                        ":" + hostname + " 464 " + nick + " :Password incorrect !\r\n"
# define ERR_MODENOSUCHCHANNEL(hostname, channel, nick)                         ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"
# define ERR_MODEUSERNOTINCHANNEL(hostname, channel)                            ":" + hostname + " 441 " + channel + " "  + ":they aren't on that channel\r\n"
# define ERR_INVALIDMODEPARAM(channel, hostname, flag)                      ":" + hostname +  " 696 " + channel + " " + flag + " * you must specifiy a parameter for the op mode\r\n"
# define RPL_MODEIS(channel, hostname, mode)                                ":" + hostname + " MODE " + channel + " " + mode + "\r\n"
# define ERR_BADCHANNELKEY(nick, hostname, channelName)                     ":" + hostname + " 475 " + nick + " " + channelName + " :Cannot join channel (+K) - bad key\r\n"
# define ERR_NOTOP(hostname, channel)                                       ":" + hostname + " 482 " + channel + " "  + ":You're not a channel operator\r\n"
#endif
