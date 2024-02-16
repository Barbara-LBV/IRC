/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:46 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/15 18:59:28 by blefebvr         ###   ########.fr       */
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
		addToClientBufferExtended(client->getServer(), client->getFd(), \
			ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
		return;
	}

    std::string name = arguments[0];
	name[0] == '#' ? name : name.insert(0, 1, '#');
	
	std::string reason = "";
	if (arguments.size() > 1)
	{
		reason = arguments[1];
		for (size_t i = 2; i < arguments.size(); i++)
		reason += " " + arguments[i];
	}
		
    Channel* 	channel = _server->getChannel(name);
	if (!channel)
	{
		addToClientBufferExtended(_server, client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), name));
		return ;
	}
	if (channel->getClient(client->getNickname()) == NULL)
	{
		addToClientBufferExtended(_server, client->getFd(), ERR_USERNOTINCHANNEL(client->getNickname(), " ", channel->getName()));
        return ;
	}
    channel->partChannel(client, reason);
}
