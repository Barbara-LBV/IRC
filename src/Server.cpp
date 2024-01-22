/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/22 18:07:04 by blefebvr         ###   ########.fr       */
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

void			Server::setMsg(std::string buf){_cliMsg = buf;}
