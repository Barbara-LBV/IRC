/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:24 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/19 11:59:20 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcLib.hpp"
#include "Client.hpp"

NickCommand::NickCommand(Server *server, bool auth) : Command(server) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty() || arguments[0].empty())
	{
		client->reply(ERR_NONICKNAMEGIVEN(client->getPrefix()));
		return;
	}

	// std::string nickname = arguments[0];

	// if (_server->getClient(nickname))
	// {
	// 	client->reply(ERR_NICKNAMEINUSE(client->getPrefix(), nickname));
	// 	return;
	// }
	// client->setNickname(nickname);
	// client->welcome();
}