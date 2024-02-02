/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:05:32 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/02 11:49:17 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

// PRIVMSG parida :hello !     send message "hello !" to parida   

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
	if (_server->isValidNickname(target))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
		return ;
	}
	if (arguments[1][0] != ':')
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NORECIPIENT(client->getNickname()));
	Client*		client_target = _server->getClientByNickname(target);
	std::string message = NULL;
	for (int i = 2; i < arguments.size(); i++)
		message += arguments[i];
	addToClientBuffer(client->getServer(), client_target->getFd(), message);
}