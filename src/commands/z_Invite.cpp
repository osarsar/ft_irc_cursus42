#include "../../inc/invite.hpp"
#include "../../inc/privmsg.hpp"
#include "../../inc/servsocket.hpp"

std::vector<std::string> my_split(const std::string& input, char delimiter, SERVSOCKET server) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        token = server.trim(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// ... (other includes and definitions)
// INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar
Invite::Invite()
{
}

Invite::~Invite()
{
}

void Invite::go_to_invite(std::string data, SERVSOCKET &server)
{
    (void)data;
    (void)server;
    // // Split the inviteCommand into components
    // // Assume you have a utility function to split the string, e.g., splitString
    // std::vector<std::string> commandParts = my_split(data, ' ', server);
    // if (commandParts.size() != 3) {
    //     // inviter->send("ERR_NEEDMOREPARAMS INVITE");
    //     std::cout << "ERR_NEEDMOREPARAMS INVITE\n";
    //     return ;
    // }
    // const std::string& channelName = commandParts[2];
    // if (channelName == name) {
    //     // The channel name in the command matches the current channel

    //     // Extract the nickname to invite
    //     const std::string& targetNickname = commandParts[3];

    //     // Check if the target is not already a member
    //     auto targetIt = std::find_if(members.begin(), members.end(),
    //         [targetNickname](client* client) {
    //             return client->getNickname() == targetNickname;
    //         });

    //     if (targetIt == members.end()) {
    //         // Case: Inviting a client not already in the channel
    //         // Add the target to the channel members
    //         client* targetClient = findClientByNickname(targetNickname);
    //         if (targetClient != nullptr) {
    //             members.push_back(targetClient);

    //             // Notify others about the invitation
    //             // Implement this based on your communication mechanism
    //             sendMessage("INVITE " + targetNickname + " :" + name, inviter);

    //             // Notify the inviter about the successful invitation
    //             // Implement this based on your communication mechanism
    //             inviter->send("RPL_INVITING " + targetNickname + " " + name);
    //         } else {
    //             // Target client not found
    //             // Respond with an error message
    //             // Implement this based on your communication mechanism
    //             inviter->send("ERR_NOSUCHNICK " + targetNickname);
    //         }
    //     } else {
    //         // Case: Target is already a member
    //         // Respond with an error message
    //         // Implement this based on your communication mechanism
    //         inviter->send("ERR_USERONCHANNEL " + targetNickname + " " + name);
    //     }
    // } else {
    //     // Channel name in the command doesn't match the current channel
    //     // Respond with an error message
    //     // Implement this based on your communication mechanism
    //     inviter->send("ERR_NOSUCHCHANNEL " + channelName);
    // }
    
}
