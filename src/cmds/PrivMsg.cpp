/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:05:32 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/09 18:18:13 by blefebvr         ###   ########.fr       */
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
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
		return;
	}
	
	const std::string  target = arguments[0];
	std::cout << "is this a channel name with # ? " << arguments[0] << std::endl;
	if (target[0] == '#')
	{
		if (_server->isValidChannelName(target))
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getPrefix(), target));
			return ;
		}
	}
	else
	{
		if (_server->isValidNickname(target))
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getPrefix(), target));
			return ;
		}
	}
	if (arguments[1][0] != ':')
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NORECIPIENT(client->getPrefix()));
		return ;
	}
	std::string message = arguments[1].substr(1);
	for (size_t i = 2; i < arguments.size(); i++)
		message += " " + arguments[i];
	
	if (target[0] == '#')
	{
		Channel* channel = _server->getChannel(target);
		channel->broadcastChannelPrimsg(client, message);
	}
	else
	{
		Client*		client_target = _server->getClientByNickname(target);
		addToClientBuffer(client->getServer(), client_target->getFd(), RPL_PRIVMSG(client->getPrefix(), target, message));
	}
}