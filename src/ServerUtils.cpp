/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:19:04 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/11 14:21:29 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

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