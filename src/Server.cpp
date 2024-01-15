/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/15 16:05:46 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd)
{
    _socServ = -1;
	_servPwd = pwd;
	_cliMsg = "";
	_servPort = atoi(port.c_str());
	//_result = 0;
    //_remain = 0;
	_cliNb = 0;
	memset(&_hints, 0, sizeof(_hints));
}

Server::~Server()
{
	close(_socServ);
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