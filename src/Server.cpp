/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/19 15:21:01 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd)
{
    _servFd = -1;
	_servPwd = pwd;
	_cliMsg = "";
	_servPort = atoi(port.c_str());
	_servName = "Par & Bar IRC' Server";
	_result = 0;
	_cliNb = 0;
}

Server::~Server()
{
	close(_servFd );
}

/*********************  Assessors !!  ************************/

Channel*  		Server::getChannel(std::string chan){ return _channels[chan];}

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

void			Server::setMsg(std::string buf){_cliMsg = buf;}