/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:06:20 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/07 12:46:21 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password, Client* ops, Server *server)
					: _name(name), _topic(""),_password(password), _server(server), \
                     _l(1000),_i(FALSE), _t(TRUE){_ops.push_back(ops);}
Channel::~Channel() 
{
    _clients.clear();
    _ops.clear();
}

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
			cli->deleteChannel(this);   // remove the name of channel in  the client info
            if ((it + 1) == _clients.end())
                delete this;
            break; 
        }
        if (it == _ops.end())
            addToClientBufferExtended(cli->getServer(), cli->getFd(), ERR_USERNOTINCHANNEL(cli->getNickname(), cli->getNickname(), this->getName()));
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

void 	Channel::broadcastChannel(Client* client, std::string message)
{
    std::vector<Client*> cli_target = getClients();
	std::vector<Client*>::iterator it = cli_target.begin();
	
   for (; it != cli_target.end(); ++it)
   {
		if (this == (*it)->getActiveChannel() && client != *it)
			addToClientBufferExtended(getServer(), (*it)->getFd(), RPL_PRIVMSG(client->getNickname(), getName(), message));
	}
}

void        Channel::replyList(Client* client)
{
    std::vector<std::string> nickname = getNicknames();
    std::string list = "";
    for (std::vector<std::string>::iterator it = nickname.begin(); it != nickname.end(); ++it)
    {
        if (!it->empty()) 
        list += *it + " ";
    } 
    list += "\n";
    addToClientBuffer(client->getServer(), client->getFd(), RPL_NAMREPLY(client->getNickname(), this->getName(), list));
    addToClientBuffer(client->getServer(), client->getFd(), RPL_ENDOFNAMES(client->getNickname(), this->getName()));
}

void    Channel::removeClient(Client *client, std::string reason)
{
    	std::string clientPrefix = client->getPrefix();

	if (reason.empty())
		this->broadcastChannel(client, RPL_PART(clientPrefix, this->getName()));
	else
		this->broadcastChannel(client, RPL_PART_REASON(clientPrefix, this->getName(), reason));
	reason.clear();

	if (!_ops.empty())
		_ops.erase(this->_ops.begin() + this->clientIndex(_ops, client));
	//if (!_clients.empty())
	//	_clients.erase(this->_clients.begin() + this->clientIndex(_clients, client));
	//client->leave(this, 1, reason);

	if (_clients.empty())
	{
		// free chan and remove it from server
		return;
	}
	if (_admin == client)
		_admin = _clients.begin().operator*();

}

size_t   Channel::clientIndex(std::vector<Client *> clients, Client *client)
{
	size_t i = 0;
	std::vector<Client *>::iterator it = clients.begin();

	while (it != clients.end())
	{
		if (*it == client)
			return i;
		it++;
		i++;
	}
	return 0;
}