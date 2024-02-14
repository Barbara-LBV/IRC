/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:06:20 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/14 10:45:52 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password, Server *server)
					: _name(name),_password(password), _server(server)
                    {
                        _clients.clear();
                        _ops.clear();
                        _invited.clear();
                        _topic = "";
                        _l = 1000;
                        _i = FALSE;
                        _t = TRUE;
                    }
                    
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

void Channel::addClient(Client *cli)
{
    // Check if the client is already in the channel
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == cli)
        {
            // If the client is already in the channel, print an error message and return
            addToClientBuffer(cli->getServer(), cli->getFd(), ERR_USERONCHANNEL(cli->getPrefix(), cli->getNickname(), getName()));	
            return;
        }
    }
    // If the client is not already in the channel, add it to the list
    _clients.push_back(cli);
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
	return FALSE;
}

bool Channel::partChannel(Client* cli, std::string reason)
{
    //bool clientFound = false;

    // Iterate through the list of clients in the channel
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == cli)
        {
            _clients.erase(it);
            cli->deleteChannel(this);
            addToClientBuffer(cli->getServer(), cli->getFd(), RPL_PART(cli->getPrefix(), getName()));	
            _server->broadcastChannel(cli, RPL_PART(cli->getPrefix(), getName()), this);
            reason.clear();
            
            if (is_oper(cli))
                removeOpe(cli);
            if (_ops.size() == 0 && _clients.size() > 0)
            {
                cli = _clients.begin().operator*();
                addOperator(cli);
            }
            
            //clientFound = true;
            break;
        }
    }

    if (_clients.size() == 0)
    {
       _server->delChannel(this);
       addToClientBuffer(_server, cli->getFd(), RPL_ENDOFNAMES(cli->getNickname(), this->getName()));
       delete this;
       return FALSE;
    }
    //else
    //{
    //    // this->broadcastChannelPart(cli, reason);
    //    //_server->broadcastChannel(cli, RPL_PART(cli->getPrefix(), getName()), this);
    //    //reason.clear();
    //}
    return TRUE;
}

void    Channel::removeClient(Client* cli)
{
    std::string clientPrefix = cli->getPrefix();
    bool clientFound = false;

    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == cli)
        {
            _clients.erase(it);
            if (is_oper(cli))
                removeOpe(cli);
            cli->deleteChannel(this);
            clientFound = true;
            break;
        }
    }
   
    if (clientFound == false)
        addToClientBufferExtended(cli->getServer(), cli->getFd(), ERR_USERNOTINCHANNEL(cli->getNickname(), "", this->getName()));
     
    if (_clients.size() == 0)
    {
       _server->delChannel(this);
       delete this;
    }
}

void Channel::removeOpe(Client *client)
{
    std::vector<Client*>::iterator it;
    for (it = _ops.begin(); it != _ops.end(); ++it)
    {
        if (*it == client)
        {
            _ops.erase(it); // delete from list of operators in this channel
            return ;
        }
    }

    if (it == _ops.end())
    {
        std::cout << client->getNickname() + " is not an operator of this channel\n";
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
bool        Channel::isInvited(Client *client)
{
    for  (std::vector<Client *>::iterator it = _invited.begin(); it != _invited.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}


void 	Channel::broadcastChannelPrimsg(Client* client, std::string message)
{
    std::vector<Client*> cli_target = getClients();
	std::vector<Client*>::iterator it = cli_target.begin();

    for (; it != cli_target.end(); ++it)
    {
		if (this == (*it)->getActiveChannel() && client != *it)
			addToClientBuffer(getServer(), (*it)->getFd(), RPL_PRIVMSG(client->getPrefix(), getName(), message));
	}
}

void 	Channel::broadcastChannelmessage(Client* client, std::string message)
{
    std::vector<Client*> cli_target = getClients();
	std::vector<Client*>::iterator it = cli_target.begin();
	
   for (; it != cli_target.end(); ++it)
   {
		if (this == (*it)->getActiveChannel() && client != *it)
			addToClientBuffer(getServer(), (*it)->getFd(), message);
	}
}

void 	Channel::broadcastChannelPart(Client* client, std::string reason)
{
    std::vector<Client*> cli_target = getClients();
	std::vector<Client*>::iterator it = cli_target.begin();
	
   for (; it != cli_target.end(); ++it)
   {
		if (this == (*it)->getActiveChannel() && client != *it)
			addToClientBuffer(getServer(), (*it)->getFd(),  RPL_PART_REASON(client->getPrefix(), this->getName(), reason));
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

