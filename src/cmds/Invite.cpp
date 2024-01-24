/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:55:34 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/24 17:48:07 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Invite message

//       Command: INVITE
//    Parameters: <nickname> <channel>

//    The INVITE command is used to invite a user to a channel.  The
//    parameter <nickname> is the nickname of the person to be invited to
//    the target channel <channel>.  There is no requirement that the
//    channel the target user is being invited to must exist or be a valid
//    channel.  However, if the channel exists, only members of the channel
//    are allowed to invite other users.  When the channel has invite-only
//    flag set, only channel operators may issue INVITE command.





// Kalt                         Informational                     [Page 21]

// RFC 2812          Internet Relay Chat: Client Protocol        April 2000


//    Only the user inviting and the user being invited will receive
//    notification of the invitation.  Other channel members are not
//    notified.  (This is unlike the MODE changes, and is occasionally the
//    source of trouble for users.)

//    Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_USERONCHANNEL
//            ERR_CHANOPRIVSNEEDED
//            RPL_INVITING                    RPL_AWAY

//    Examples:

//    :Angel!wings@irc.org INVITE Wiz #Dust

//                                    ; Message to WiZ when he has been
//                                    invited by user Angel to channel
//                                    #Dust

//    INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
//                                    #Twilight_zone



#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

InvitCommand::InvitCommand(Server *server) : Command(server) {}

InvitCommand::~InvitCommand() {}

void InvitCommand::execute(Client *client, std::vector<std::string> arguments)
{
    if (arguments.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "INVITE"));
		return;
	}

    const std::string  target = arguments[0];
	const std::string&  chan_name = arguments[1];
	
	chan_name[0] == '#' ? chan_name : "#" + chan_name;


	if (_server->isValidNickname(target))
	{
		client->reply(ERR_NOSUCHNICK(client->getNickName(), target));
		return ;
	}
    
    if (_server->isValidChannelName(chan_name))
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickName(), target));
		return ;
    }
    
    Channel* 	channel = _server->getChannel(chan_name);
	Client*		client_target = _server->getClientByNickname(target); 
	
	if (!channel->is_oper(client))
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickName(), chan_name));
	else if (channel->isInChannel(client_target))
		client->reply(ERR_USERONCHANNEL(client->getNickName(), target, chan_name));
	else 
	{
		channel->joinChannel(client_target);
		client_target->setChannelName(chan_name);
	}
}