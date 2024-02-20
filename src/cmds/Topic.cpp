/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:06:14 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/19 13:39:10 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//    Topic message

//       Command: TOPIC
//    Parameters: <channel> [ <topic> ]

//    The TOPIC command is used to change or view the topic of a channel.
//    The topic for channel <channel> is returned if there is no <topic>
//    given.  If the <topic> parameter is present, the topic for that
//    channel will be changed, if this action is allowed for the user
//    requesting it.  If the <topic> parameter is an empty string, the
//    topic for that channel will be removed.


//    Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//            RPL_NOTOPIC                     RPL_TOPIC
//            ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES

//    Examples:

//    :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the
//                                    topic.

//    TOPIC #test :another topic      ; Command to set the topic on #test
//                                    to "another topic".

//    TOPIC #test :                   ; Command to clear the topic on
//                                    #test.

//    TOPIC #test                     ; Command to check the topic for
//                                    #test.

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

TopicCommand::TopicCommand(Server *server) : Command(server) {}

TopicCommand::~TopicCommand() {}

void TopicCommand::execute(Client *client, std::vector<std::string> arguments)
{
	 if (arguments.empty() || arguments.size() < 1)
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
		return;
	}
	std::string&  chan_name = arguments[0];
	chan_name[0] == '#' ? chan_name : chan_name.insert(0, 1, '#');
	
    if (_server->isValidChannelName(chan_name))
    {
        addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), chan_name));
		return ;
    }
    
    Channel* 	channel = _server->getChannel(chan_name);
	if (!channel->isInChannel(client))
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOTONCHANNEL(client->getNickname(), chan_name));
		return;
	}
	else if (channel->getT() && !channel->isOper(client))
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNickname(), chan_name));
		return ;
	}
	
	if (arguments.size() == 1)
	{
		if (channel->getTopic().size() > 0)
			addToClientBufferExtended(client->getServer(), client->getFd(), RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()));
		else
			addToClientBufferExtended(client->getServer(), client->getFd(), RPL_NOTOPIC(client->getNickname(), channel->getName()));
		return;
	}
	
	std::string  topic = "";
	if (arguments.size() >= 2)
	{
		if (!channel->getTopic().empty())
			channel->setTopic(topic);
		if (arguments[1][0] != ':')
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NORECIPIENT(client->getNickname()));
			return ;
		}
		if (arguments[1][0] == ':' && arguments[1].size() > 1)
			topic = arguments[1].substr(1);
		for(size_t i = 2; i < arguments.size(); i++)
			topic += " " + arguments[i];
	}
	channel->setTopic(topic);
	channel->broadcastChannelmessage(NULL, RPL_TOPIC(client->getNickname(), channel->getName(), topic));;
}
