/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/08 18:26:24 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"
//#include "ServerManagement.cpp"

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

//struct sockaddr_in 	&getServAdd(void);	
//socklen_t 			&getServAddLen(void);
//Channel 			&getChannel(void);
//Client				&getClient(void);
//servOp				&getOp(void);
//std::string			&getPwd(void);
//int					&getPort(void);
//void				setPwd(std::string pwd);

void	Server::createServerSocket(void)
{
	int rc, on = 1;
	_socServ = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_hints, 0, sizeof(_hints));
	if (_socServ == ERROR)
	{
		std::cerr << "[Server] Socket creation error" << std::endl;
		exit(-1);
	}
	std::cout << "[Server] Socket successfully created\n";
    rc = setsockopt(_socServ, SOL_SOCKET, SO_REUSEPORT, (const void*)&on, sizeof(on));
	if (rc == ERROR)
	{
		std::cerr << "[Server] Impossible to reuse the socket" << std::endl;
		exit(-1);
	}
}

void	Server::bindServerSocket(int port)
{
	memset(&_servInfo, 0, sizeof(sockaddr_in));
	_servInfo.sin_family = AF_INET;
	_servInfo.sin_addr.s_addr = INADDR_ANY;
	_servInfo.sin_port = htons(port);
	if (bind(_socServ, (struct sockaddr *)&_servInfo, sizeof(_servInfo)) == ERROR)
	{
		std::cerr << "[Server] Socket impossible to bind" << std::endl;
		exit(-1);
	}
	std::cout << "[Server] Bind succeeded on port: " << port << ", IP="
        << inet_ntoa(_servInfo.sin_addr) << std::endl;		
}

void	Server::listenForConnection(void)
{
	if (listen(_socServ, BACKLOG) ==  ERROR)
	{
		std::cerr << "[Server] Socket cannot listen" << std::endl;
		exit(-1);
	}
	std::cout << "[Server] Listening on socket fd: " << _socServ << std::endl;
}

int		Server::acceptConnection(void)
{
	socklen_t len = sizeof (_hints);
	int fd = accept(_socServ, (struct sockaddr *)&_hints, &len);
	if (fd == ERROR)
	{
		std::cerr << "[Server] Socket cannot accept connection" << std::endl;
		exit(-1);
	}
	return (fd);
}

void Server::initializeServer(int port)
{
    createServerSocket();
    bindServerSocket(port);
    listenForConnection();
}

struct sockaddr_in &Server::getServAdd(void)
{
	return (_servInfo);
}

//socklen_t 		&Server::getServAddLen(void)
//{
//	return (_socServ.len);
//}



//void	Server::receiveMsg(void)
//{
	
//}

//void 	Server::sendMsg(void)
//{
	
//}
