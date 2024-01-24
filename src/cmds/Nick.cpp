/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:24 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/24 16:02:17 by blefebvr         ###   ########.fr       */
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

	if (_server->getClientByNickname(nickname))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NICKNAMEINUSE(client->getPrefix(), nickname));
		return;
	}
	//std::cout << "In nick function, if nickname doesn't exist, nickname = " << nickname << std::endl;
	client->setNickname(nickname);
	client->welcomeClient(client->getServer());
}