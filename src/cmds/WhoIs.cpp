/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoIs.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:06:37 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/09 18:19:38 by blefebvr         ###   ########.fr       */
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
