/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:55:34 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/05 13:06:56 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"
#include "../../lib/Command.hpp"

InvitCommand::InvitCommand(Server *server) : Command(server) {}

InvitCommand::~InvitCommand() {}

void InvitCommand::execute(Client *client, std::vector<std::string> arguments)
{
	// for (size_t i = 0 ; i < arguments.size(); i++)
	// {
	// 	std::cout << "################################arguments = " + arguments[i] << std::endl;
	// }
    if (arguments.size() < 2)
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
		return;
	}
	
    const std::string  target = arguments[0];
	std::string&  chan_name = arguments[1];
	chan_name[0] == '#' ? chan_name : chan_name.insert(0, 1, '#');
	
	Client*		client_target = _server->getClientByNickname(target); 
	if (!client_target)
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
		return ;
	}

	Channel* 	channel = _server->getChannel(chan_name);
    if (channel == NULL)
    {
        addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), chan_name));
		return ;
    }
    
	if (!channel->isInChannel(client))
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NOTONCHANNEL(client->getNickname(), chan_name));
	else if (!channel->is_oper(client))
		addToClientBuffer(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNickname(), chan_name));
	else if (channel->isInChannel(client_target))
		addToClientBuffer(client->getServer(), client->getFd(), ERR_USERONCHANNEL(client->getNickname(), target, chan_name));
	else 
	{
		if ((channel->getL() - channel->getClients().size()) > 0)
		{
			channel->joinChannel(client_target);
			client_target->addChannel(channel);
			addToClientBuffer(client->getServer(), client->getFd(), RPL_INVITE(client->getNickname(), target, chan_name));
		}
		else 
			addToClientBuffer(client->getServer(), client->getFd(), ERR_CHANNELISFULL(client->getPrefix(), chan_name));
	}
}