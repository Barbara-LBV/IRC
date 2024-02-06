/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoIs.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:06:37 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/06 12:29:30 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

WhoCommand::WhoCommand(Server *server) : Command(server) {}

WhoCommand::~WhoCommand() {}

void WhoCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "WHOIS"));
	if (arguments[0] == client->getUsername())
		addToClientBuffer(client->getServer(), client->getFd(), "");
	return;
}