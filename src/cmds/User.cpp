/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:05:48 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/18 16:20:56 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcLib.hpp"
#include "Client.hpp"

UserCommand::UserCommand(Server *server) : Command(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client *client, std::vector<std::string> arguments) 
{

	if (!client->getUsername().empty())
	{
		client->reply(ERR_ALREADYREGISTERED(client->getPrefix()));
		return;
	}

	if (arguments.size() < 1) 
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getPrefix(), "USER"));
		return;
	}
	client->setUsername(arguments[0]);
	//client->setRealName(arguments[3].substr(1));
	client->isRegistred();
}