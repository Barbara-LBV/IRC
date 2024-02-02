/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/01 16:44:09 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd, struct tm * time) :_handler (new CmdHandler(this))
{
 	_servFd = -1;
	_servPort = atoi(port.c_str());
	_cliNb = 0;
	_servPwd = pwd;
	_servName = "localhost";
	this->setDatetime(time);
	_result = 0;
}

Server::~Server()
{
	std::map<int, Client *>::iterator it = _clients.begin();
	for(; it != _clients.end(); it++)
	{
		delete it->second;
		close(it->first);
	}
	std::map<std::string, Channel *>::iterator it1 = _channels.begin();
	for (; it1 != _channels.end(); it1++)
		delete it1->second;
	//_poll_fds.clear();
	_channels.clear();
	_clients.clear();
	delete _handler;
	close(_servFd);
}

/*********************  Assessors !!  ************************/

Channel* Server::getChannel(const std::string& cName)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(cName);

    if (it != _channels.end())
        return it->second;     // Channel found, return the pointer
    else
        
        return NULL;       // Channel not found, return NULL
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
	// addToClientBuffer(this, nickname, ERR_NOSUCHNICK(client->getNickname(), target));
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
//void 	Server::broadcastChannel(std::string message, Channel* channel)
//{
//	std::vector<Client*>::iterator it = channel->getClients().begin();
	
//    for (; it != channel->getClients().end(); ++it)
//    {
//		if (channel->getName() == (*it)->getActiveChannel())
//		{
//			addToClientBuffer(this, (*it)->getFd(), message);
//            this->sendReply((*it)->getFd());
//		}
//	}
//}