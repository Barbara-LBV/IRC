/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/16 15:00:27 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd)
{
    _servFd = -1;
	_servPwd = pwd;
	_cliMsg = "";
	_servPort = atoi(port.c_str());
	//_result = 0;
    //_remain = 0;
	_cliNb = 0;
}

Server::~Server()
{
	close(_servFd );
}

/*********************  Assessors !!  ************************/

std::string Server::getMsg(void)const
{
	return _cliMsg;
}

void		Server::setMsg(std::string buf)
{
	_cliMsg = buf;
}