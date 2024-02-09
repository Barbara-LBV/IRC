/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/09 18:40:45 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd, struct tm * time) : _servInfo(NULL)
{
 	_servFd = -1;
	_servPort = atoi(port.c_str());
	_cliNb = 0;
	_servPwd = pwd;
	_servName = "localhost";
	this->setDatetime(time);
	_result = 0;
	memset(&_hints, 0, sizeof(_hints));
	_handler = new CmdHandler(this);
}

Server::~Server()
{
	close(_servFd);
	if (_clients.empty())
	{
        _clients.clear();
        return;
    }
	std::map<int, Client *>::iterator it = _clients.begin();
	for(; it != _clients.end(); ++it)
	{
		delete it->second;
		close(it->first);
	}
	_clients.clear();
	if (_channels.empty())
    {
        _channels.clear();
        return;
    }
    for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
       delete *it;
    _channels.clear();
	delete _handler;
}

/*********************  Assessors !!  ************************/

Channel* Server::getChannel(const std::string& cName)
{
    for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if ((*it)->getName() == cName)
            return *it; // Channel found, return the pointer
    }

    return NULL; // Channel not found, return NULL
}

void Server::addChannel(std::string chan_name, Channel* channel)
{
    if (channel)
    {
		if (_channels.size() == 0)
			_channels.push_back(channel);
        // Check if the channel name already exists
		else if (_channels[_channels.size() - 1]->getName() == chan_name)
		{
			// If not, add the channel to the map
			_channels.push_back(channel);
		}
		else
			std::cout << "channel name already is use\n";
    }
}


Client*			Server::getClient(int fd){return _clients[fd];}

std::string		&Server::getPwd(void){ return _servPwd;}

int				&Server::getPort(void){ return _servPort;}

int				&Server::getFd(void){ return _servFd;}

std::string		&Server::getServerName(void){return _servName;}

std::string		&Server::getStartTime(void){ return _time;}

Client 			*Server::getClientByNickname(const std::string &nickname)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second->getNickname() == nickname) {
			// Found the client with the given nickname
			return it->second;
		}
	}
	// addToClientBufferExtended(this, nickname, ERR_NOSUCHNICK(client->getNickname(), target));
	return NULL; 
}

void			Server::setFd(int fd){_servFd = fd;}

void			Server::setDatetime(struct tm *timeinfo)
{
	char buffer[80];

	strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
  	std::string str(buffer);
	_time = str;
}
//void 	Server::broadcastChannelPrimsg(std::string message, Channel* channel)
//{
//	std::vector<Client*>::iterator it = channel->getClients().begin();
	
//    for (; it != channel->getClients().end(); ++it)
//    {
//		if (channel->getName() == (*it)->getActiveChannel())
//		{
//			addToClientBufferExtended(this, (*it)->getFd(), message);
//            this->sendReply((*it)->getFd());
//		}
//	}
//}
