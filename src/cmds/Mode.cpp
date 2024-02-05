/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:08 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/05 13:56:06 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// MODE  #abc   +i/i				channel #abc is Invite-only channel now
// MODE  #abc	-i					channel #abc is not need to invite for join in now
// MODE  #abc   +t/t				only operator can set or modify TOPIC of channel #abc
// MODE  #abc   -t					every user can set or modify TOPIC of channel #abc
// MODE  #abc   k/+k	hello 		set channel #abc password is "hello"
// MODE  #abc   -k					remove password of channel #abc
// MODE  #abc   o/+o	parida		give operator privilege to user "parida" in channel #abc 
// MODE  #abc   -o      parida		takeoff operator privilege to user "parida" in channel #abc
// MODE  #abc   l/+l  	15			set limit of number of user in channel #abc  is 15	
// MODE  #abc   -l  				takeoff limit of number of user in channel #abc
		

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

void ModeCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2 || arguments[0].empty() || arguments[1].empty()) 
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		return;
	}

	const std::string&  chan_name = arguments[0];
	chan_name[0] == '#' ? chan_name : "#" + chan_name;
    Channel* 	channel = _server->getChannel(chan_name);

	if (!channel)
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), chan_name));
		return ;
	}
    if (!channel->isInChannel(client))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NOTONCHANNEL(client->getNickname(), chan_name));
		return ;
	}
	else if (!channel->is_oper(client))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNickname(), chan_name));
		return ;
	}
	// MODE #abc i/-i
	if (arguments[1] == "i" || arguments[1] == "+i" )
	{
		channel->setI(true);
		channel->broadcastChannel("channel" + chan_name + " is invite-only now");
	}
	if (arguments[1] == "-i" )
	{
		channel->setI(false);
		channel->broadcastChannel("channel" + chan_name + " is invite-only now");
	}

	// MODE #abc t/-t
	if (arguments[1] == "t" || arguments[1] == "+t" )
	{
		channel->setT(true);
		channel->broadcastChannel("only operator can set or modify TOPIC of channel" + chan_name);
	}
	if (arguments[1] == "-t" )
	{
		channel->setT(false);
		channel->broadcastChannel("every user can set or modify TOPIC of channel" + chan_name);
	}

	// MODE #abc k/-k
	if (arguments[1] == "k" || arguments[1] == "+k" )
	{
		if (arguments[2].empty())
			addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		else
		{
			channel->setPassword(arguments[2]);
			channel->broadcastChannel("channel's Password has changed");
		}
	}
	if (arguments[1] == "-k" )
	{
		channel->setPassword("");
		channel->broadcastChannel("channel's Passwod has removed");
	}
	
	// MODE #abc o/-o
	if (arguments[1] == "o" || arguments[1] == "+o" || "-o")
	{
		if (arguments[2].empty())
			addToClientBuffer(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		else 
		{
			std::string target = arguments[2];
			if (_server->isValidNickname(target))
			{
				addToClientBuffer(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
				return ;
			}
			Client*		client_target = _server->getClientByNickname(target); 
			if (!channel->isInChannel(client_target))
			{
				addToClientBuffer(client->getServer(), client->getFd(), ERR_USERNOTINCHANNEL(client->getNickname(), target, chan_name));
				return ;
			}
			else if (!channel->is_oper(client_target))
			{
				if (arguments[1] == "o" || arguments[1] == "+o")
				{
					channel->addOperator(client_target);
					client_target->addChannel(channel);
					addToClientBuffer(client->getServer(), client->getFd(), target + " is got operator privilege now");
					return ;
				}
				else 
				{
					if (arguments[1] == "-o")
					{
						channel->removeOpe(client_target);
						client_target->deleteChannel(channel);
						addToClientBuffer(client->getServer(), client->getFd(), target + " is no more operator of channel " + chan_name);
						return ;
					}
				}
			}
		}
	}
	// MODE #abc l/-l
	if (arguments.size() > 2 || isAllDigits(arguments[2])) 
	{
        // Convert the string to size_t
        std::istringstream iss(arguments[2]);
        size_t sizeValue;

        if ((iss >> sizeValue) && iss.eof())
		{
			if ((arguments[1] == "l" || arguments[1] == "+l") && (sizeValue != 0))
				channel->setL(sizeValue);
		}    
	}
	if (arguments.size() == 2 && arguments[1] == "-l")
		channel->setL(0);
}

bool	isAllDigits(const std::string& str) 
{
    for (std::size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}