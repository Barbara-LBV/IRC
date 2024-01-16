/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:19:04 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/16 15:00:06 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::createServerSocket(void)
{
	int rc, on = 1;
	_servFd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_hints, 0, sizeof(_hints));
	if (_servFd == ERROR)
	{
		std::cerr << "[Server] Socket creation error" << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
    rc = setsockopt(_servFd, SOL_SOCKET, SO_REUSEPORT, (const void*)&on, sizeof(on));
	if (rc == ERROR)
	{
		std::cerr << "[Server] Impossible to reuse the socket" << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
}

void	Server::bindServerSocket(int port)
{
	memset(&_servInfo, 0, sizeof(sockaddr_in));
	_servInfo.sin_family = AF_INET;
	_servInfo.sin_addr.s_addr = INADDR_ANY;
	_servInfo.sin_port = htons(port);
	if (bind(_servFd, (struct sockaddr *)&_servInfo, sizeof(_servInfo)) == ERROR)
	{
		std::cerr << "[Server] Socket impossible to bind" << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
}

void	Server::listenForConnection(void)
{
	if (listen(_servFd, BACKLOG) ==  ERROR)
	{
		std::cerr << "[Server] Socket cannot listen" << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
	std::cout << "[Server] Listening on socket fd: " << _servFd << std::endl;
}

int		Server::acceptConnection(void)
{
	socklen_t len = sizeof (_hints);
	int cliFd = accept(_servFd, (struct sockaddr *)&_hints, &len);
	if (cliFd == ERROR)
	{
		std::cerr << "[Server] Socket cannot accept connection" << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
	return (cliFd);
}

void Server::initializeServer(int port)
{
    createServerSocket();
    bindServerSocket(port);
    listenForConnection();
	std::cout << "[Server] Waiting for connections... " << std::endl;
}

void 	Server::checkPoll(int rc)
{
	if (rc == ERROR && server_shutdown == TRUE)
	{
		exit(ERROR);
		server_shutdown = TRUE;
	}
	if (rc == ERROR)
	{
		std::cerr << "[Server] Poll failed" << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Poll timed out." << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
}

void	Server::checkReception(int rc)
{
	if (rc < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cerr << "[Server] Reception/Sending failed.";
			server_shutdown = TRUE;
			exit(ERROR);
		}
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Connection closed." << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
}
