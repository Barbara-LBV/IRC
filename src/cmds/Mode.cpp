/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:08 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/16 11:43:53 by pmaimait         ###   ########.fr       */
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
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		return;
	}
	std::string&  nick = arguments[0];
	if (arguments[0] == client->getNickname())
	{
		std::string& mode = arguments[1];
		if (arguments[1] == "+i")
			addToClientBuffer(client->getServer(), client->getFd(), MODE_USERMSG(nick, mode));
		//if (arguments[1] == "-o")
		//	addToClientBufferExtended(client->getServer(), client->getFd(), MODE_USERMSG(client->getNickname(), "-o" ));
		return;
	}
	
	std::string& chan_name = arguments[0];
	chan_name[0] == '#' ? chan_name : chan_name.insert(0, 1, '#');
    Channel* 	channel = _server->getChannel(chan_name);

	if (!channel)
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), chan_name));
		return ;
	}
    if (!channel->isInChannel(client))
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOTONCHANNEL(client->getNickname(), chan_name));
		return ;
	}
	else if (!channel->is_oper(client))
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNickname(), chan_name));
		return ;
	}
	// MODE #abc i/-i
	if (arguments[1] == "i" || arguments[1] == "+i" )
	{
		channel->setI(true);
		_server->broadcastChannel(NULL, RPL_MODE(client->getNickname(), chan_name, "+i", "channel is Invite only now"), channel);
		return;
	}
	if (arguments[1] == "-i" )
	{
		channel->setI(false);
		_server->broadcastChannel(NULL, RPL_MODE(client->getNickname(), chan_name, "-i", "channel no need to invite"), channel);
		return ;
	}

	// MODE #abc t/-t
	if (arguments[1] == "t" || arguments[1] == "+t" )
	{
		channel->setT(true);
		_server->broadcastChannel(NULL, RPL_MODE(client->getNickname(), chan_name, "+t", "only operator can set or modify TOPIC of channel"), channel);
		return;
	}
	if (arguments[1] == "-t" )
	{
		channel->setT(false);
		_server->broadcastChannel(NULL, RPL_MODE(client->getNickname(), chan_name, "-t", "every user can set or modify TOPIC of channel"), channel);
		return;
	}

	// MODE #abc k/-k
	if (arguments[1] == "k" || arguments[1] == "+k" )
	{
		if (arguments.size() < 3)
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
			return ;
		}
		else
		{
			channel->setPassword(arguments[2]);
			_server->broadcastChannel(NULL, MODE_CHANNELMSGWITHPARAM(chan_name, "+k", channel->getPassword()), channel);
			//_server->broadcastChannel(NULL, RPL_MODE(client->getNickname(), chan_name, "+k", ("password has changed for " + channel->getPassword())), channel);
			return;
		}
	}
	if (arguments[1] == "-k" )
	{
		channel->setPassword("");
		_server->broadcastChannel(NULL, MODE_CHANNELMSGWITHPARAM(chan_name, "-k", "no password needed to enter"), channel);
		return ;
	}
	
	// MODE #abc o/-o
	if (arguments[1] == "o" || arguments[1] == "+o" || arguments[1] == "-o")
	{
		if (arguments.size() < 3)
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
			return;
		}
		else 
		{
			std::string target = arguments[2];
			if (_server->isValidNickname(target))
			{
				addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
				return ;
			}
			Client*		client_target = _server->getClientByNickname(target); 
			if (!channel->isInChannel(client_target))
			{
				addToClientBufferExtended(client->getServer(), client->getFd(), ERR_USERNOTINCHANNEL(client->getNickname(), target, chan_name));
				return ;
			}
			bool isOperator = false;
			if (channel->is_oper(client_target))
				isOperator = true;
			if (arguments[1] == "o" || arguments[1] == "+o")
			{
				if (isOperator == false)
				{
					
					addToClientBufferExtended(client->getServer(), client->getFd(), MODE_USERMSG(client->getNickname(), "+o"));
					_server->broadcastChannel(NULL, RPL_MODE(client->getPrefix(),chan_name, "+o", target + " got operator privilege now"), channel);
					channel->addOperator(client_target);
				}
				else
					addToClientBufferExtended(client->getServer(), client->getFd(), target + "  already a operator in this channel");
				return;
			}
			else if (arguments[1] == "-o")
			{
				if (isOperator == true && client->getNickname() != client_target->getNickname())
				{
					std::cout << "what will be happen when operator left the channel? \n";
					addToClientBufferExtended(client->getServer(), client->getFd(), MODE_USERMSG(client->getNickname(), "-o"));
					_server->broadcastChannel(NULL, RPL_MODE(client->getPrefix(),chan_name, "-o", target + " is no more operator of channel" ), channel);
					channel->removeOpe(client_target);
					return;
				}
				if (isOperator == false)
					addToClientBufferExtended(client->getServer(), client->getFd(), target + "  is not operator of this channel");
				return ;
			}
		}
	}
	// MODE #abc l/-l
	if (arguments.size() == 3 && isAllDigits(arguments[2])) 
	{
        // Convert the string to size_t
        std::istringstream iss(arguments[2]);
        size_t sizeValue;

        if ((iss >> sizeValue) && iss.eof())
		{
			if ((arguments[1] == "l" || arguments[1] == "+l") && (sizeValue != 0))
			{
				channel->setL(sizeValue);
				_server->broadcastChannel(NULL, RPL_MODE(client->getPrefix(), chan_name, "+l ", ("Limit for channel user is " + arguments[2])), channel);
				//_server->broadcastChannel(NULL, MODE_CHANNELMSGWITHPARAM(client-> getNickname(), chan_name, "+l", ("Limit for channel user is " + arguments[2])), channel);
				return;
			}
		}    
	}
	if (arguments.size() == 2 && arguments[1] == "-l")
	{
		channel->setL(0);
		_server->broadcastChannel(NULL, RPL_MODE(client->getPrefix(), chan_name, "-l ", "channel is unlimit user channel now"), channel);
		//_server->broadcastChannel(NULL, MODE_CHANNELMSGWITHPARAM(client-> getNickname(), chan_name, "-l", "channel is unlimit user channel now"), channel);
		return;
	}
	_server->broadcastChannel(NULL, ERR_UMODEUNKNOWNFLAG(client->getNickname()), channel);
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