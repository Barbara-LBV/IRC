/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/24 15:03:30 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd)
{
    _servFd = -1;
	_servPwd = pwd;
	_cliMsg = "";
	_servPort = atoi(port.c_str());
	_servName = "localhost";
	_result = 0;
	_cliNb = 0;
}

Server::~Server()
{
	close(_servFd );
}

/*********************  Assessors !!  ************************/

Channel* Server::getChannel(const std::string& cName)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(cName);

    if (it != _channels.end())
        return it->second;     // Channel found, return the pointer
    else
        
        return nullptr;       // Channel not found, return nullptr
}


Client*			Server::getClient(int fd){return _clients[fd];}

Client *Server::getClientByNickname(const std::string &nickname)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second->getNickname() == nickname) {
			// Found the client with the given nickname
			return it->second;
		}
	}
	// Client not found
	return nullptr; 
}

std::string		&Server::getPwd(void){ return _servPwd;}

int				&Server::getPort(void){ return _servPort;}

std::string 	&Server::getMsg(void){ return _cliMsg;}

std::string		&Server::getServerName(void){return _servName;}

std::string		&Server::getStartTime(void){ return _time;}

Client *Server::getClientByNickname(const std::string &nickname)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second->getNickname() == nickname) {
			// Found the client with the given nickname
			return it->second;
		}
	}
	// Client not found
	return NULL; 
}

void			Server::setMsg(std::string buf){_cliMsg = buf;}


void Server::broadcastChannel(std::string message, Channel* channel) const
{
    for (std::vector<Client*>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it)
        if (channel->getName() == (*it)->getChannelName())
            this->send(message, (*it)->getFD());
}
