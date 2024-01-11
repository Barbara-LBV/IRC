/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/11 14:22:12 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd)
{
    _socServ = -1;
	_servPwd = pwd;
	_servInput = "";
	_servPort = atoi(port.c_str());
	_result = 0;
    _remain = 0;
	memset(&_hints, 0, sizeof(_hints));
}

Server::~Server()
{
	close(_socServ);
}

/*********************  Assessors !!  ************************/
