/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:04:09 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/29 11:11:19 by pmaimait         ###   ########.fr       */
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
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getPrefix(), "PASS"));
		return;
	}

	if (_server->getPwd() != arguments[0])
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_PASSWDMISMATCH(client->getPrefix()));
		return;
	}
    //std::cout << "in pass function PASS = " << arguments[0] << std::endl; 
    client->setPwd(arguments[0]);
	client->welcomeClient(client->getServer());
}