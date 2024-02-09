/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:46 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/09 15:08:37 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

PartCommand::PartCommand(Server *server) : Command(server) {}

PartCommand::~PartCommand() {}

void PartCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
		return;
	}

    std::string name = arguments[0];
	name[0] == '#' ? name : name.insert(0, 1, '#');
	
	std::string reason = "";
	if (arguments.size() > 1)
	{
		for (size_t i = 1; i < arguments.size(); i++)
		reason += " " + arguments[i];
	}
		
    Channel* 	channel = _server->getChannel(name);

    channel->partChannel(client, reason);
	
}
