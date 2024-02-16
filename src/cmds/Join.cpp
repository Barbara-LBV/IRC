/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:55:54 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/15 18:49:18 by blefebvr         ###   ########.fr       */
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

//    JOIN &foo fubar                 ; Command to join channel &foo using
//                                    key "fubar".



//    JOIN #foo,&bar fubar            ; Command to join channel #foo using
//                                    key "fubar" and &bar using no key.

//    JOIN #foo,#bar fubar,foobar     ; Command to join channel #foo using
//                                    key "fubar", and channel #bar using
//                                    key "foobar".

//    JOIN #foo,#bar                  ; Command to join channels #foo and
//                                    #bar.

//    JOIN 0                          ; Leave all currently joined
//                                    channels.

//    :WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone ; JOIN message from WiZ
//                                    on channel #Twilight_zone


JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

// format : JOIN channel pw
void JoinCommand::execute(Client *client, std::vector<std::string> arguments)
{
    if (arguments.empty())
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN"));
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

	Channel* channel = _server->getChannel(name);
	if (channel == NULL)
	{
        Channel* channel = new Channel(name, password, _server);
		addToClientBufferExtended(client->getServer(), client->getFd(), RPL_JOIN(client->getPrefix(), name));
		_server->addChannel(name, channel);
		channel->addClient(client);
		channel->addOperator(client);
		client->addChannel(channel);
		channel->replyList(client);
	}
    else 
	{
		if(channel->getI() == true && channel->isInvited(client) == false)
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_INVITONLYCHAN(client->getPrefix(), name));
			return ;
		}
		else if (((channel->getL() - channel->getClients().size()) > 0))
		{
			if (password == channel->getPassword())
			{
				channel->addClient(client);
				client->addChannel(channel);
				addToClientBuffer(client->getServer(), client->getFd(), RPL_JOIN(client->getPrefix(), name));
				channel->replyList(client);
				_server->broadcastChannel(client, RPL_JOIN(client->getPrefix(), name), channel);
				if (!channel->getTopic().empty())
					addToClientBufferExtended(client->getServer(), client->getFd(), RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()));
				return;
			}
			else
				addToClientBufferExtended(client->getServer(), client->getFd(), ERR_BADCHANNELKEY(client->getNickname(), name));
			
		}
		else
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_CHANNELISFULL(client->getNickname(), name));
	}
}