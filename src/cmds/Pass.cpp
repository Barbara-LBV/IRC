/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:04:09 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/09 18:18:04 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

PassCommand::PassCommand(Server *server, bool auth) : Command(server, auth) {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
		return;
	}

	if (_server->getPwd() != arguments[0])
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_PASSWDMISMATCH(client->getPrefix()));
		return;
	}
    client->setPwd(arguments[0]);
	client->welcomeClient(client->getServer());
}