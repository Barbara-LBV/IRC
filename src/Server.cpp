/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/23 18:51:12 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd):
 	_servFd(-1),
	_servPort(atoi(port.c_str())),
	_cliNb(0),
	_servPwd(pwd),
	_cliMsg(""),
	_servName("localhost"),
	_result(0),
	_handler (new CmdHandler(this))
{}

Server::~Server()
{
	delete _handler;
	close(_servFd );
}

/*********************  Assessors !!  ************************/

Channel*  		Server::getChannel(std::string chan){ return _channels[chan];}

Client*			Server::getClient(int fd){return _clients[fd];}

std::string		&Server::getPwd(void){ return _servPwd;}

int				&Server::getPort(void){ return _servPort;}

std::string 	&Server::getMsg(void){ return _cliMsg;}

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
	// Client not found
	return NULL; 
}

void			Server::setMsg(std::string buf){_cliMsg += buf;}

void Server::broadcastChannel(std::string message, Channel* channel) const
{
    for (std::vector<Client*>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it)
        if (channel->getName() == (*it)->getChannelName())
            this->send(message, (*it)->getFD());
}