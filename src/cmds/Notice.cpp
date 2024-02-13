/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 18:11:32 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/13 16:43:50 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

NoticeCommand::NoticeCommand(Server *server) : Command(server) {}

NoticeCommand::~NoticeCommand() {}

void NoticeCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "NOTICE"));
		return;
	}
	
	const std::string  target = arguments[0];
	if (target[0] == '#')
	{
		if (_server->isValidChannelName(target))
		{
			addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), target));
			return ;
		}
	}
	else
	{
		if (_server->isValidNickname(target))
		{
			addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
			return ;
		}
	}
	if (arguments[1][0] != ':')
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NORECIPIENT(client->getNickname()));
		return ;
	}
	std::string message = "";
	for (size_t i = 2; i < arguments.size(); i++)
		message += arguments[i];
	
	if (target[0] == '#')
	{
		Channel* channel = _server->getChannel(target);
		channel->broadcastChannelPrimsg(client, message);
	}
	else
	{
		Client*		client_target = _server->getClientByNickname(target);
		addToClientBuffer(client->getServer(), client_target->getFd(), message);
	}
}
