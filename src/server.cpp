/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/22 16:50:33 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/server.hpp"

Server::Server(int port, std::string pwd)
{
	_socServ = 0;
	_servPort = port;
	_servPwd = pwd;
}

Server::Server(Server const &s)
{
	*this = s;
}

Server &Server::operator=(Server const &s)
{
	if (this != &s)
	{
		_fdsServ = s._fdsServ;		
		_socServ = s._socServ;
		_servPort = s._servPort;
		_servPwd = s._servPwd;
	}
	return *this;
}

Server::~Server()
{
	std::vector<int>::iterator it = _fdsServ.begin();
	while (it != _fdsServ.end())
	{
		_fdsServ.erase(it);
		it++;
	}
	_fdsServ.clear();
}

void	Server::createSocket(void)
{
	_socServ = socket(AF_INET, SOCK_STREAM, 0);
	if (_socServ == SOCKET_ERROR)
		throw CantCreateSocket();
	struct sockaddr_in addServ;
	addServ.sin_addr.s_addr = inet_addr("127.0.0.1");
	addServ.sin_family = AF_INET;
	addServ.sin_port = htons(30000);
	if (bind(_socServ, (const struct  sockaddr *)&addServ, sizeof(addServ)) == SOCKET_ERROR)
		throw CantBind();
	std::cout << "bind: " << _socServ << std::endl;		
	if (listen(_socServ, 5) ==  SOCKET_ERROR)
		throw CantListen();
	std::cout << "listen\n";
}

void	Server::acceptConnexions(void)
{
	int connRes = connect(_socServ, (sockaddr*)&_addServ, sizeof(_addServ));
	if (connRes == SOCKET_ERROR)
		throw FailConnexion();
	while (1)
	{
		//write
		//send
		//wait
		//display
		//read
		//close socket 		
	}
}