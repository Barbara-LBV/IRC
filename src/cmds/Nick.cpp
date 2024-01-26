/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:24 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/26 14:16:23 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

NickCommand::NickCommand(Server *server, bool auth) : Command(server, auth) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty() || arguments[0].empty())
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NONICKNAMEGIVEN(client->getPrefix()));
		return;
	}

	std::string nickname = arguments[0];

	if (_server->getClientByNickname(nickname) && client->getNickname().empty())
	{
		while (_server->getClientByNickname(nickname))
		{
			std::cout << "in nick function bp#2 \n";
			nickname = nickname + "_";
			std::cout << "auth = " << getAuthRequired() << std::endl;
			std::cout << "auth = " << client->getUsername() << std::endl;
		}
		if (!client->getUsername().empty())
		{
			addToClientBuffer(client->getServer(), client->getFd(), NICK(nickname, client->getUsername(), nickname));
			client->setNickname(nickname);
			send(client->getFd(), client->getMsgRecvd().c_str(), MAXBUF, 0);
			_authRequired = TRUE;
			std::cout << "in nick function bp#3 \n";
			client->welcomeClient(client->getServer());
			std::cout << "in nick function bp#4 \n";
			client->setRecvMsg("");
			return;
		}	
	}
	else if (_server->getClientByNickname(nickname) && client->getNickname() == nickname)
	{
		std::cout << "in nick function bp#0 \n";
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NICKNAMEINUSE(client->getPrefix(), nickname));
		return;
	}
	client->setNickname(nickname);
	if (!client->getUsername().empty() && getAuthRequired() == FALSE)
	{
		std::cout << "in nick function bp#5 \n";
		addToClientBuffer(client->getServer(), client->getFd(), NICK(nickname, client->getUsername(), nickname));
		send(client->getFd(), client->getMsgRecvd().c_str(), MAXBUF, 0);
		_authRequired = TRUE;
		client->setRecvMsg("");
	}
	std::cout << "in nick function bp#6 \n";
	client->welcomeClient(client->getServer());
}