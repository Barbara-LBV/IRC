/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:24 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/23 15:53:03 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

NickCommand::NickCommand(Server *server) : Command(server) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty() || arguments[0].empty())
	{
		addToClientBuffer(client->getServer(), client->getCliFd(), ERR_NONICKNAMEGIVEN(client->getPrefix()));
		return;
	}

	std::string nickname = arguments[0];

	if (_server->getClientByNickname(nickname))
	{
		addToClientBuffer(client->getServer(), client->getCliFd(), ERR_NICKNAMEINUSE(client->getPrefix(), nickname));
		return;
	}
	client->setNickname(nickname);
	client->isRegistred();
}