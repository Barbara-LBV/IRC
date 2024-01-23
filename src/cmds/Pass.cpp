/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:04:09 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/23 17:35:49 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

PassCommand::PassCommand(Server *server) : Command(server) {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		addToClientBuffer(client->getServer(), client->getCliFd(), ERR_NEEDMOREPARAMS(client->getPrefix(), "PASS"));
		return;
	}

	if (_server->getPwd() != arguments[0])
	{
		addToClientBuffer(client->getServer(), client->getCliFd(), ERR_PASSWDMISMATCH(client->getPrefix()));
		return;
	}
    
    client->setPwd(arguments[0]);
	client->isRegistred();
}