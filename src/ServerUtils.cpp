/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:19:04 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/15 18:59:39 by blefebvr         ###   ########.fr       */
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
		server_shutdown = TRUE;
		exit(ERROR);
	}
    rc = setsockopt(_socServ, SOL_SOCKET, SO_REUSEPORT, (const void*)&on, sizeof(on));
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
	if (bind(_socServ, (struct sockaddr *)&_servInfo, sizeof(_servInfo)) == ERROR)
	{
		std::cerr << "[Server] Socket impossible to bind" << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
}

void	Server::listenForConnection(void)
{
	if (listen(_socServ, BACKLOG) ==  ERROR)
	{
		std::cerr << "[Server] Socket cannot listen" << std::endl;
		server_shutdown = TRUE;
		exit(ERROR);
	}
	std::cout << "[Server] Listening on socket fd: " << _socServ << std::endl;
}

int		Server::acceptConnection(void)
{
	socklen_t len = sizeof (_hints);
	int cliFd = accept(_socServ, (struct sockaddr *)&_hints, &len);
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

void	Server::splitMsg(Client *cli, std::string msg)
{
	std::string tmp;
	size_t	pos(0);
	
	while (pos < std::string::npos)
	{
		if (msg.find("PASS", pos) < std::string::npos)
		{
			//inserer PASS et sa suite dans une string tmp, puis setter le mdp
			cli->setPwd(tmp);
			pos += msg.find("PASS", pos);
			tmp.clear();
		}
		if (msg.find("NICK", pos) < std::string::npos)
		{
        	cli->setNickname();
			pos += msg.find("NICK", pos);
		}
		if (msg.find("USER", pos) < std::string::npos)
		{
			cli->setUsername();
			pos += msg.find("USER", pos);
		}
		else
			break ;
	}
}