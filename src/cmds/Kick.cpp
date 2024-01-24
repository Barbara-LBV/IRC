/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:56:07 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/24 17:49:55 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/Client.hpp"


KickCommand::KickCommand(Server *server) : Command(server) {}

KickCommand::~KickCommand() {}

// format: KICK <channel> <user> 

void KickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "KICK"));
		return;
	}

    std::string chan_name = arguments[0];
    chan_name[0] == '#' ? chan_name : "#" + chan_name;
	std::string target = arguments[1];

    if (_server->isValidNickname(target))
	{
		client->reply(ERR_NOSUCHNICK(client->getNickName(), target));
		return ;
	}
    
    if (_server->isValidChannelName(chan_name))
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickName(), target));
		return ;
    }
    
    Channel* 	channel = _server->getChannel(chan_name);
	Client*		client_target = _server->getClientByNickname(target); 
    
    if (!channel->is_oper(client))
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickName(), chan_name));
    else if (!channel->isInChannel(client_target))
		client->reply(ERR_USERNOTINCHANNEL(client->getNickName(), target, chan_name));
    else
        channel->partChannel(client_target);
}