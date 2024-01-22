/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:04:09 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/22 11:17:14 by pmaimait         ###   ########.fr       */
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
		client->reply(ERR_NEEDMOREPARAMS(client->getPrefix(), "PASS"));
		return;
	}

	if (_server->getPwd() != arguments[0])
	{
		client->reply(ERR_PASSWDMISMATCH(client->getPrefix()));
		return;
	}
    
    client->setPwd(arguments[0]);
	client->isRegistred();
}