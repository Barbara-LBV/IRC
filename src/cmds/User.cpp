/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:05:48 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/09 18:19:31 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

UserCommand::UserCommand(Server *server, bool auth) : Command(server, auth) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client *client, std::vector<std::string> arguments) 
{

	if (!client->getUsername().empty())
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_ALREADYREGISTERED(client->getPrefix()));
		return;
	}
	if (arguments.size() < 1) 
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
		return;
	}
	client->setUsername(arguments[0]);
	if (arguments.size() == 5)
		client->setRealName(arguments[3].substr(1) + " " + arguments[4]);
	else if (arguments.size() == 4)
		client->setRealName(arguments[3].substr(1) + " " + arguments[3]);
	if (!client->getNickname().empty())
		addToClientBuffer(client->getServer(), client->getFd(), RPL_NICK(client->getOldNick(), client->getUsername(), client->getNickname()));
	client->welcomeClient(client->getServer());
}