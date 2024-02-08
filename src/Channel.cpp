/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:06:20 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/08 11:43:47 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password, Client* ops, Server *server)
					: _name(name), _topic(""),_password(password), _server(server), \
                     _l(1000),_i(FALSE), _t(TRUE){_ops.push_back(ops);}
Channel::~Channel() 
{
    //if (_clients.empty())
    //{
    //    _clients.clear();
    //    return;
    //}
    //for (size_t i = _clients.size(); i > 0; --i)
    //{
    //    Client *client = _clients[i - 1]; // Access to th element at index i - 1
    //    if (client)
    //        delete _clients[i];
    //}
    //_clients.clear();
	// if (_ops.empty())
    //{
    //    _ops.clear();
    //    return;
    //}
    //for (size_t i = _ops.size(); i > 0; --i)
    //{
    //    Client *client = _ops[i - 1]; // Access to th element at index i - 1
    //    if (client)
    //        delete _ops[i];
    //}
    //_ops.clear();
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



void Channel::partChannel(Client* cli, std::string reason)
{
    std::string clientPrefix = cli->getPrefix();

    // Broadcast PART message to the channel with or without a reason
    if (reason.empty())
        this->broadcastChannel(cli, RPL_PART(clientPrefix, this->getName()));
    else
        this->broadcastChannel(cli, RPL_PART_REASON(clientPrefix, this->getName(), reason));
    reason.clear();

    bool clientFound = false;

    // Iterate through the list of clients in the channel
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == cli)
        {
            // Remove the client from the list of clients in this channel
            _clients.erase(it);
            clientFound = true;

            // If the client was an operator, remove them from the list of operators
            if (is_oper(cli))
                removeOpe(cli);

            // Remove the channel from the client's channel list
            cli->deleteChannel(this);
            
            // Exit the loop since the client has been found and processed
            break;
        }
    }

    // If the client was not found, send an error message
    if (!clientFound)
        addToClientBufferExtended(cli->getServer(), cli->getFd(), ERR_USERNOTINCHANNEL(cli->getNickname(), "", this->getName()));

    // If there are no more clients left in the channel, mark the channel for deletion
    if (_clients.empty())
        delete this;
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
			addToClientBuffer(getServer(), (*it)->getFd(), RPL_PRIVMSG(client->getPrefix(), getName(), message));
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

