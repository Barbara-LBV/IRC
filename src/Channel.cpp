/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:06:20 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/29 15:29:07 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password, Client* ops, Server *server)
					: _name(name), _topic(""),_password(password), _server(server), \
                     _l(1000),_i(FALSE), _t(TRUE){_ops.push_back(ops);}
Channel::~Channel() {}

std::vector<std::string> Channel::getNicknames()
{
    std::vector<std::string> nicknames;
    std::vector<Client*>::iterator it = _clients.begin();

    while (it != _clients.end())
    {
        Client* client = *it;

        // Check if client is in the _ops vector
        if (std::find(_ops.begin(), _ops.end(), client) != _ops.end()) {
            nicknames.push_back("@");  // Client is in _ops
        } else {
            nicknames.push_back("");   // Client is not in _ops
        }

        nicknames.back() += client->getNickname();
        it++;
    }
    return nicknames;
}

Client  *Channel::getClient(const std::string &nickname)
{
	std::vector<Client *>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		if ((*it)->getNickname() == nickname)
			return *it;
		it++;
	}
	return NULL;
}

void 	 Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}

bool    Channel::is_oper(Client *client)
{
	std::vector<Client *> opers_chan = this->getOperator();
	std::vector<Client *>:: iterator it_oper = opers_chan.begin();

	while (it_oper != opers_chan.end())
	{
		Client *oper = *it_oper;
		if (oper == client)
			return TRUE;
		++it_oper;
	}
	if (it_oper == opers_chan.end())
		return FALSE;
	return FALSE;
}

void    Channel::partChannel(Client* cli)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == cli)
        {
            _clients.erase(it);			// delete from list of client in this channel
            if (is_oper(cli))
                removeOpe(cli);
			cli->deleteChannelName(getName());   // remove the name of channel in  the client info
            if ((it + 1) == _clients.end())
                delete this;
            break; 
        }
        if (it == _ops.end())
            std::cout << cli->getNickname() + " is not user of this channel\n";
    }
}

void    Channel::removeOpe(Client *client)
{
    for (std::vector<Client*>::iterator it = _ops.begin(); it != _ops.end(); ++it)
    {
        if (*it == client)
        {
            _ops.erase(it);   // delete from list of operator in this channel
            break;
        }
        else if (it == _ops.end())
            std::cout << client->getNickname() + " is not operator of this channel\n";
    }
}

bool	Channel::isInChannel(Client *client)
{
	for  (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}

void 	Channel::broadcastChannel(std::string message)
{
	std::vector<Client*>::iterator it = getClients().begin();
	
   for (; it != getClients().end(); ++it)
   {
		if (getName() == (*it)->getActiveChannel())
		{
			addToClientBuffer(getServer(), (*it)->getFd(), message);
            // (*it)->getFd()->sendReply((*it)->getFd());
		}
	}
}