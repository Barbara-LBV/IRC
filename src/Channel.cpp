/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:06:20 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/23 15:55:41 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password, Client* ops, Server *server)
					: _name(name) , _l(1000), _i(false), _password(password), _server(server), _topic(NULL){_ops.push_back(ops);}
Channel::~Channel() {}

const std::string 	Channel::getName(void) const {return _name;};

std::vector<std::string> Channel::getNickNames()
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


Client *Channel::getClient(const std::string &nickname)
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

void Channel::partChannel(Client* cli)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->getNickname() == cli->getNickname())
        {
            _clients.erase(it);			// delete from list of client in this channel
			if ()
			cli->getChannelName().pop();   // remove the name of channel in  the client
            break; 
        }
    }
	
	 

	for (std::vector<Client*>::iterator it = _ops.begin(); it != _ops.end(); ++it)
    {
        if ((*it)->getNickname() == cli->getNickname())
        {
            _ops.erase(it);
            break; // delete from list of ops in this channel
        }
    }

	
}


// void Channel::removeOper(Client *client)
// {
// 	_oper_clients.erase(this->_oper_clients.begin() + this->_clientIndex(_oper_clients, client));
// }

// void Channel::kick(Client *client, Client *target, std::string reason)
// {
// 	broadcast(RPL_KICK(client->getPrefix(), _name, target->getNickName(), reason));
// 	reason.clear();
// 	removeClient(target, reason);
// }

// void Channel::invit(Client *client, Client *target)
// {
// 	client->reply(RPL_INVITING(client->getNickName(), target->getNickName(), this->_name));
// 	target->write(RPL_INVITE(client->getPrefix(), target->getNickName(), this->_name));
// 	target->join(this);
// }

// bool	Channel::isInChannel(Client *client)
// {
// 	std::vector<Client *>::iterator it = _clients.begin();

// 	while (it != _clients.end())
// 	{
// 		if (*it == client)
// 			return true;
// 		it++;
// 	}
// 	return false;
// }

// unsigned long Channel::_clientIndex(std::vector<Client *> clients, Client *client)
// {
// 	unsigned long i = 0;
// 	std::vector<Client *>::iterator it = clients.begin();

// 	while (it != clients.end())
// 	{
// 		if (*it == client)
// 			return i;
// 		it++;
// 		i++;
// 	}
// 	return 0;
// }
