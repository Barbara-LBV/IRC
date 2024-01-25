/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:55:34 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/25 11:28:19 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"
#include "../../lib/Command.hpp"

InvitCommand::InvitCommand(Server *server) : Command(server) {}

InvitCommand::~InvitCommand() {}

void InvitCommand::execute(Client *client, std::vector<std::string> arguments)
{
    if (arguments.size() < 2)
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
		return;
	}

    const std::string  target = arguments[0];
	const std::string&  chan_name = arguments[1];
	
	chan_name[0] == '#' ? chan_name : "#" + chan_name;


	if (_server->isValidNickname(target))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
		return ;
	}
    
    if (_server->isValidChannelName(chan_name))
    {
        addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), target));
		return ;
    }
    
    Channel* 	channel = _server->getChannel(chan_name);
	Client*		client_target = _server->getClientByNickname(target); 
	
	if (!channel->is_oper(client))
		addToClientBuffer(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNickname(), chan_name));
	else if (channel->isInChannel(client_target))
		addToClientBuffer(client->getServer(), client->getFd(), ERR_USERONCHANNEL(client->getNickname(), target, chan_name));
	else 
	{
		channel->joinChannel(client_target);
		client_target->setChannelName(chan_name);
	}
}