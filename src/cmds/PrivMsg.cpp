/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:05:32 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/02 15:24:43 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

// PRIVMSG parida :hello !     send message "hello !" to parida 
// PRIVMSG #abc :hello !     send message "hello !" to channel #abc   

PrivMsgCommand::PrivMsgCommand(Server *server) : Command(server) {}

PrivMsgCommand::~PrivMsgCommand() {}

void PrivMsgCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
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
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NORECIPIENT(client->getNickname()));
		return ;
	}
	std::string message = "";
	for (size_t i = 2; i < arguments.size(); i++)
		message += arguments[i];
	
	if (target[0] == '#')
	{
		Channel* channel = _server->getChannel(target);
		channel->broadcastChannel(message);
	}
	else
	{
		Client*		client_target = _server->getClientByNickname(target);
		addToClientBuffer(client->getServer(), client_target->getFd(), message);
	}
}