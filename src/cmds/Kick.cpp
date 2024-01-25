/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:56:07 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/25 11:29:10 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

KickCommand::KickCommand(Server *server) : Command(server) {}

KickCommand::~KickCommand() {}

void KickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
		return;
	}

    std::string chan_name = arguments[0];
    chan_name[0] == '#' ? chan_name : "#" + chan_name;
	std::string target = arguments[1];

    if (_server->isValidNickname(target))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
		return ;
	}
    
    if (_server->isValidChannelName(chan_name))
    {
        addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), target));
		return ;
    }
    
    Channel* 	channel = _server->getChannel(chan_name);
	Client*		client_target = _server->getClientByNickname(target); 
    
    if (!channel->is_oper(client))
		addToClientBuffer(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNickname(), chan_name));
    else if (!channel->isInChannel(client_target))
		addToClientBuffer(client->getServer(), client->getFd(), ERR_USERNOTINCHANNEL(client->getNickname(), target, chan_name));
    else
        channel->partChannel(client_target);
}