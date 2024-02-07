/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:19:04 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/07 11:52:07 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::createServerSocket(void)
{
	int rc, fd, on = 1;
	fd = socket(_servInfo->ai_family, _servInfo->ai_socktype, _servInfo->ai_protocol);
	if (fd == ERROR)
	{
		std::cerr << BGREEN "[Server] " << GREEN "Socket creation error" << std::endl;
		server_shutdown = TRUE;
		close(fd);
		exit(ERROR);
	}
	setFd(fd);
    rc = setsockopt(getFd(), SOL_SOCKET, SO_REUSEPORT, (const void*)&on, sizeof(on));
	if (rc == ERROR)
	{
		std::cerr << BGREEN "[Server] " << GREEN "Impossible to reuse the socket" << std::endl;
		close(getFd());
		server_shutdown = TRUE;
		exit(ERROR);
	}
}

void	Server::setHint(void)
{
	_hints.ai_family = AF_INET;		  // We choose Ipv4
	_hints.ai_socktype = SOCK_STREAM; // We choose to work with TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;	  // We'll be on localhost by default
}

int Server::fillServinfo(char *port)
{
	if (getaddrinfo(NULL, port, &_hints, &_servInfo) < 0)
	{
		std::cerr << RED << "[Server] Flop du addrinfo" << DEFAULT << std::endl;
		return (ERROR);
	}
	return (TRUE);
}

void	Server::bindServerSocket(void)
{
	if (bind(getFd(), _servInfo->ai_addr, _servInfo->ai_addrlen) == ERROR)
	{
		std::cerr << BGREEN "[Server] " << GREEN "Socket impossible to bind" DEFAULT << std::endl;
		close(getFd());
		server_shutdown = TRUE;
		exit(ERROR);
	}
	fcntl(getFd(), F_SETFL, O_NONBLOCK);
}

void	Server::listenForConnection(void)
{
	if (listen(getFd(), BACKLOG) ==  ERROR)
	{
		std::cerr << BGREEN "[Server] " << GREEN "Socket cannot listen" DEFAULT << std::endl;
		server_shutdown = TRUE;
		close(getFd());
		exit(ERROR);
	}
	std::cout << BGREEN "[Server] " << GREEN "Listening on socket fd #" << _servFd << DEFAULT << std::endl;
}

int		Server::acceptConnection(void)
{
	sockaddr_in client;
	socklen_t addr_size = sizeof(sockaddr_in);
	int cliFd = accept(getFd(), (sockaddr *)&client, &addr_size);
	if (cliFd == ERROR)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cerr << BGREEN "[Server] " << GREEN "Socket cannot accept connection" DEFAULT << std::endl;
			return BREAK ;
		}
	}
	fcntl(cliFd, F_SETFL, O_NONBLOCK);
	return (cliFd);
}

void Server::initializeServer(void)
{
    createServerSocket();
    bindServerSocket();
    listenForConnection();
	std::cout << BGREEN "[Server] " <<  GREEN "Waiting for connections... " DEFAULT << std::endl;
	freeaddrinfo(_servInfo);
}

int 	Server::checkPoll(int rc)
{
	if (rc == ERROR && server_shutdown == TRUE)
		return ERROR;
	if (rc == ERROR)
	{
		std::cerr << BGREEN "[Server] " <<  GREEN "Poll failed" DEFAULT << std::endl;
		server_shutdown = TRUE;
		return ERROR;
	}
	if (rc == 0)
	{
		std::cerr << BGREEN "[Server] " <<  GREEN "Poll timed out." DEFAULT << std::endl;
		return BREAK ;
	}
	return TRUE;
}

int		Server::checkRecv(int res, int fd)
{
	(void)fd;
	if (res == ERROR)
	{
		std::cout << BGREEN "[Server] " <<  GREEN "recv() error\n" DEFAULT;
		return ERROR;
	}	
	if (res == 0)
	{
		std::cout << BGREEN "[Server] " <<  GREEN "Client disconnected\n" DEFAULT;
		return ERROR;
	}
	return TRUE;
}

void 	addToClientBufferExtended(Server *server, int cliFd, std::string reply)
{
	Client *client = server->getClient(cliFd);
	
	if (client)
		client->setRecvMsg(":" + server->getServerName() + " " + reply);
	client->sendReply(client->getFd());
}

void	addToClientBuffer(Server *server, int cliFd, std::string reply)
{
	Client *client = server->getClient(cliFd);
	
	if (client)
		client->setRecvMsg(reply);
	client->sendReply(client->getFd());
}
