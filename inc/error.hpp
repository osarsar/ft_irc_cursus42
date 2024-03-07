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

#define ERR_NEEDMOREPARAMS(cid, cmd, hostname)                ":" + std::string(hostname) + " 461 " + std::string(cid) + " " + std::string(cmd) + " :Not enough parameters\r\t\n"
#define ERR_NOSUCHCHANNEL(nick, chnl, hostname)               ":" + std::string(hostname) + " 403 " + std::string(nick) + " " + std::string(chnl) + " :No such channel\r\t\n"
#define ERR_NOTONCHANNEL(nick, chnl, hostname)                ":" + std::string(hostname) + " 442 " + std::string(nick) + " " + std::string(chnl) + " :You're not on that channel\r\t\n"
#define ERR_CHANOPRIVSNEEDED(nick, chnl, hostname)            ":" + std::string(hostname) + " 482 " + std::string(nick) + " " + std::string(chnl) + " :You're not a channel operator\r\t\n"
#define ERR_NOTREGISTERED(nick, hostname)                     ":" + std::string(hostname) + " 451 " + std::string(nick) + " :You have not registered\r\t\n"

//>>>>>>>> TOPIC --------------------------------------------------------->>>>>>>>>>>>>>>>>>
#define RPL_NOTOPIC(clt, chnl, hostname)                      ":" + std::string(hostname) + " 332 " + std::string(clt) + " " + std::string(chnl) + " :No topic is set\r\t\n"
#define RPL_TOPIC(hN, hU, ip, new_tpc,chnl)         ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " TOPIC " + std::string(chnl) + " : " + std::string(new_tpc) + "\r\t\n"
#define RPL_TOPIC_CLEAR(hN, hU, ip, chnl)           ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " TOPIC " + std::string(chnl) + " :" + "\r\t\n"
#define RPL_TOPIC_DISPLAY(hN, hU, ip, new_tpc,chnl) ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " TOPIC " + std::string(chnl) + " : " + std::string(new_tpc) + " TOPIC " + std::string(chnl) + "\r\t\n"
#define RPL_TOPICWHOTIME(clt, chnl, nik, setat)     std::string(clt) + " " + std::string(chnl) + " " + std::string(nik) + " " + std::string(setat)

//>>>>>>>> KICK --------------------------------------------------------->>>>>>>>>>>>>>>>>>
#define ERR_USERNOTINCHANNEL(clt, nik, chnl)        std::string(clt) + " " + std::string(nik) + " " + std::string(chnl) + " :They aren't on that channel"
#define ERR_BADCHANMASK(chnl, hostname)                       ":" + std::string(hostname) + " 476 " + std::string(chnl) + " :Bad Channel Mask\r\t\n"
#define RPL_KICK(hN, hU, ip, chnl, gN, reason)               ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " KICK " + std::string(chnl) + " " + std::string(gN)  + " :" + std::string(reason) + "\r\t\n"

//>>>>>>>> INVITE --------------------------------------------------------->>>>>>>>>>>>>>>>>>
// #define ERR_NOSUCHNICK(nick, nameToDo, hostname)              ":" + std::string(hostname) + " 401 " + std::string(nick) + " " + std::string(nameToDo) + " :No such nick/channel\r\t\n"
// #define ERR_USERONCHANNEL(nik1, nik2, chnl, hostname)         ":" + std::string(hostname) + " 443 " + std::string(nik1) + " " + std::string(nik2) + " " + std::string(chnl) + " :is already on channel\r\t\n"
#define RPL_INVITING(nik, nik2, chnl, hostname)               ":" + std::string(hostname) + " 341 " + std::string(nik) + " " + std::string(nik2) + " " + std::string(chnl) + "\r\t\n"
#define RPL_INVITELIST(hN, hU, ip, gN,chnl)         ":" + std::string(hN) + "!~" + std::string(hU) + "@" + std::string(ip) + " INVITE " + std::string(gN) + " :" + std::string(chnl) + "\r\t\n"
#define RPL_ENDOFINVITELIST(nik)                    std::string(nik) + " :End of /INVITE list"

//-----------ADDED------------------//
#define ERR_NICKCOLLISION(hostname, nick, cmd)                                        ":" + std::string(hostname) + " 436 " + std::string(nick) + " " + std::string(cmd) + " :Nickname collision KILL\r\t\n"
#define RPL_WELCOME(nick, hostname)                                         ":" + std::string(hostname) + " 001 " + nick + " :Welcome " + nick + " to the ft_irc network !\r\n"
#define RPL_YOURHOST(nick, hostname)                                        ":" + std::string(hostname) + " 002 " + nick + " :Your host is " + std::string(hostname) + " running version 1.0 !\r\n"
#define RPL_CREATED(nick, hostname)                                         ":" + std::string(hostname) + " 003 " + nick + " :This server was created 2024-03-02 !\r\n"
#define RPL_MYINFO(nick, hostname)                                          ":" + std::string(hostname) + " 004 " + nick + " :Host: " + std::string(hostname) + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i !\r\n"

#define ERR_PASSWDMISMATCH(hostname, cid)						                        ":" + std::string(hostname) + " 464 " + std::string(cid) + " :Password incorrect\r\t\n"
#define ERR_ALREADYREGISTRED(hostname, cid)						                    ":" + std::string(hostname) + " 462 " + std::string(cid) + " :Unauthorized command (already registered)\r\t\n"
#define	ERR_UNKNOWNCOMMAND(hostname, cid, cmd)						                ":" + std::string(hostname) + " 421 " + std::string(cid) + " " + std::string(cmd) + " :Unknown command\r\t\n" 
#define	ERR_NICKNAMEINUSE(hostname, cid, nick)						                ":" + std::string(hostname) + " 433 " + std::string(cid) + " " + std::string(nick) + " :Nickname is already in use\r\t\n"
# define ERR_BOT(hostname, nick)                                                    ":" + hostname + " 450 " + nick + "enter [BOT generate] to generate a joke" + "\r\n"
//>>>>>>>> JOIN --------------------------------------------------------->>>>>>>>>>>>>>>>>>
# define RPL_JOIN(nick, username, channelname, ipaddress)                   ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
# define RPL_NAMREPLY(hostname, clients, channelname,nick)                  ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
# define RPL_ENDOFNAMES(hostname, nick, channelname)                        ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"
# define ERR_USERONCHANNEL(hostname, channel, nick)                         ":" + hostname + " 443 " + nick + " " + channel + "  :is already on channel\r\n"
# define ERR_NOSUCHNICK(hostname, channel, argument)                        ":" + hostname + " 401 " + channel + " " +  argument + " :No such nick/channel\r\n"
# define ERR_CHANNELISFULL(nick, hostname)                                  ":" + hostname + " 471 " + channelName + " :Cannot join channel (+l)\r\n"
# define ERR_INVITEONLY(nick, hostname)                                     ":" + hostname + " 473 " + channelName + " :Cannot join channel (+i)\r\n"
# define ERR_JOINPASSWDMISMATCH(nick, hostname)	                            ":" + hostname + " 464 " + nick + " :Password incorrect !\r\n"
# define ERR_MODENOSUCHCHANNEL(hostname, channel, nick)                         ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"
# define ERR_MODEUSERNOTINCHANNEL(hostname, channel)                            ":" + hostname + " 441 " + channel + " "  + ":they aren't on that channel\r\n"
# define ERR_INVALIDMODEPARAM(channel, hostname, flag)                      ":" + hostname +  " 696 " + channel + " " + flag + " * you must specifiy a parameter for the op mode\r\n"
# define RPL_MODEIS(channel, hostname, mode)                                ":" + hostname + " MODE " + channel + " " + mode + "\r\n"
# define ERR_BADCHANNELKEY(nick, hostname, channelName)                     ":" + hostname + " 475 " + nick + " " + channelName + " :Cannot join channel (+K) - bad key\r\n"
# define ERR_NOTOP(hostname, channel)                                       ":" + hostname + " 482 " + channel + " "  + ":You're not a channel operator\r\n"
#endif
