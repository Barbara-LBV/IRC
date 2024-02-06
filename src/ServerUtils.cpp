/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:19:04 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/06 13:40:42 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::createServerSocket(void)
{
	int rc, fd, on = 1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
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

void	Server::bindServerSocket(int port)
{
	memset(&_servInfo, 0, sizeof(sockaddr_in));
	_servInfo.sin_family = AF_INET;
	_servInfo.sin_addr.s_addr = INADDR_ANY;
	_servInfo.sin_port = htons(port);
	if (bind(getFd(), (struct sockaddr *)&_servInfo, sizeof(_servInfo)) == ERROR)
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
	socklen_t len = sizeof (_hints);
	int cliFd = accept(getFd(), (struct sockaddr *)&_hints, &len);
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

void Server::initializeServer(int port)
{
    createServerSocket();
    bindServerSocket(port);
    listenForConnection();
	std::cout << BGREEN "[Server] " <<  GREEN "Waiting for connections... " DEFAULT << std::endl;
}

int 	Server::checkPoll(int rc)
{
	if (rc == ERROR && server_shutdown == TRUE)
		throw ;
	if (rc == ERROR)
	{
		std::cerr << BGREEN "[Server] " <<  GREEN "Poll failed" DEFAULT << std::endl;
		server_shutdown = TRUE;
		throw ;
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
	if (res == ERROR)
	{
		std::cout << BGREEN "[Server] " <<  GREEN "recv() error\n" DEFAULT;
		return ERROR;
	}	
	if (res == 0)
	{
		delClient(fd);
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
