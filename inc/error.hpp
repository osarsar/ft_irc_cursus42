#ifndef ERROR_HPP
# define ERROR_HPP

#include "channel.hpp"

// clt -> client
// chnl -> channel
// cmd -> command
// tpc -> topic
// nik -> nick
// setat -> setat


// "<client> <command> :Not enough parameters"
// "<client> <channel> :No such channel"
// "<client> <channel> :You're not on that channel"
// "<client> <channel> :You're not channel operator"
// "<client> <channel> :No topic is set"
// "<client> <channel> :<topic>"
// "<client> <channel> <nick> <setat>"
#define ERR_NEEDMOREPARAMS(clt, cmd)                std::string(clt) + " " + std::string(cmd) + " :Not enough parameters"
#define ERR_NOSUCHCHANNEL(clt, chnl)                std::string(clt) + " " + std::string(chnl) + " :No such channel"
#define ERR_NOTONCHANNEL(clt, chnl)                 std::string(clt) + " " + std::string(chnl) + " :You're not on that channel"
#define ERR_CHANOPRIVSNEEDED(clt, chnl)             std::string(clt) + " " + std::string(chnl) + " :You're not channel operator"

//>>>>>>>> TOPIC --------------------------------------------------------->>>>>>>>>>>>>>>>>>
#define RPL_NOTOPIC(clt, chnl)                      std::string(clt) + " " + std::string(chnl) + " :No topic is set"
#define RPL_TOPIC(clt, chnl, tpc)                   std::string(clt) + " " + std::string(chnl) + " :" + std::string(tpc)
#define RPL_TOPICWHOTIME(clt, chnl, nik, setat)     std::string(clt) + " " + std::string(chnl) + " " + std::string(nik) + " " + std::string(setat)
//>>>>>>>> KICK --------------------------------------------------------->>>>>>>>>>>>>>>>>>
// "<client> <nick> <channel> :They aren't on that channel"
// "<channel> :Bad Channel Mask" 
#define ERR_USERNOTINCHANNEL(clt, nik, chnl)        std::string(clt) + " " + std::string(nik) + " " + std::string(chnl) + " :They aren't on that channel"
#define ERR_BADCHANMASK(chnl)                       std::string(chnl) + " :Bad Channel Mask"
//>>>>>>>> INVITE --------------------------------------------------------->>>>>>>>>>>>>>>>>>
// "<client> <nick> <channel>"
// "<client> <nick> <channel> :is already on channel"
// "<client> <channel>"
// "<client> :End of /INVITE list"
#define ERR_USERONCHANNEL(clt, nik, chnl)           std::string(clt) + " " + std::string(nik) + " " + std::string(chnl) + " :is already on channel"
#define RPL_INVITING(clt, nik, chnl)                std::string(clt) + " " + std::string(nik) + " " + std::string(chnl)
#define RPL_INVITELIST(clt, chnl)                   std::string(clt) + " " + std::string(chnl)
#define RPL_ENDOFINVITELIST(clt)                    std::string(clt) + " :End of /INVITE list"




class Exception : public std::exception
{
private:
    std::string msg;
public:
    Exception(std::string msg);
    virtual ~Exception() throw ();

    public:
        virtual const char* what() const throw ()
        {
            return (msg.c_str());
        }
};

Exception::Exception(std::string msgg):msg(msgg)
{
}

Exception::~Exception() throw ()
{
}


#endif

