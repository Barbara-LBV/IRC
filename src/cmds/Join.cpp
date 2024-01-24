/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:55:54 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/24 17:36:35 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

//    Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
//                / "0"

//    The JOIN command is used by a user to request to start listening to
//    the specific channel.  Servers MUST be able to parse arguments in the
//    form of a list of target, but SHOULD NOT use lists when sending JOIN
//    messages to clients.

//    Once a user has joined a channel, he receives information about
//    all commands his server receives affecting the channel.  This
//    includes JOIN, MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.
//    This allows channel members to keep track of the other channel
//    members, as well as channel modes.

//    If a JOIN is successful, the user receives a JOIN message as
//    confirmation and is then sent the channel's topic (using RPL_TOPIC) and
//    the list of users who are on the channel (using RPL_NAMREPLY), which
//    MUST include the user joining.

//    Note that this message accepts a special argument ("0"), which is
//    a special request to leave all channels the user is currently a member
//    of.  The server will process this message as if the user had sent
//    a PART command (See Section 3.2.2) for each channel he is a member
//    of.

//    Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//            ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//            ERR_CHANNELISFULL               ERR_BADCHANMASK
//            ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//            ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
//            RPL_TOPIC

//    Examples:

//    JOIN #foobar                    ; Command to join channel #foobar.

//    JOIN 0                          ; Leave all currently joined
//                                    channels.

//    :WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone ; JOIN message from WiZ
//                                    on channel #Twilight_zone

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

// format : JOIN channel pw
void JoinCommand::execute(Client *client, std::vector<std::string> arguments)
{
    if (arguments.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getPrefix(), "JOIN"));
		return;
	}
    
	std::string name = arguments[0];

	if (name == "0")
	{
		client->partAllChannel();
		return;
	}
		
	
	name[0] == '#' ? name : "#" + name;
	std::string password = arguments.size() > 1 ? arguments[1] : "";

	if (_server->isValidChannelName(name))
	{
        Channel* channel = new Channel(name, password, client, _server);
		client->setChannelName(name);
	}
    else 
	{
		Channel* channel = _server->getChannel(name);
		if(channel->getI() == true)
		{
			client->reply(ERR_INVITONLYCHAN(client->getPrefix(), name));
			return ;
		}
		else if ((channel->getL() - channel->getClients().size()) > 0)
		{
			channel->joinChannel(client);
			client->setChannelName(name);
		}
		else
			client->reply(ERR_CHANNELISFULL(client->getPrefix(), name));
	}
}