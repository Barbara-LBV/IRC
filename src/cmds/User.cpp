/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:05:48 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/23 17:36:00 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

UserCommand::UserCommand(Server *server) : Command(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client *client, std::vector<std::string> arguments) 
{

	if (!client->getUsername().empty())
	{
		addToClientBuffer(client->getServer(), client->getCliFd(), ERR_ALREADYREGISTERED(client->getPrefix()));
		return;
	}

	if (arguments.size() < 1) 
	{
		addToClientBuffer(client->getServer(), client->getCliFd(), ERR_NEEDMOREPARAMS(client->getPrefix(), "USER"));
		return;
	}
	client->setUsername(arguments[0]);
	//client->setRealName(arguments[3].substr(1));
	client->isRegistred();
}