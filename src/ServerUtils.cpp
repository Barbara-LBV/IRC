/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:19:04 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/31 14:14:08 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::createServerSocket(void)
{
	int rc, on = 1;
	_servFd = socket(AF_INET, SOCK_STREAM, 0);
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
	fcntl(_servFd, F_SETFL, O_NONBLOCK);
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
		if (errno != EWOULDBLOCK)
		{
			std::cerr << "[Server] Socket cannot accept connection" << std::endl;
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
	std::cout << "[Server] Waiting for connections... " << std::endl;
}

int 	Server::checkPoll(int rc)
{
	if (rc == ERROR && server_shutdown == TRUE)
		exit(ERROR);
	if (rc == ERROR)
	{
		std::cerr << "[Server] Poll failed" << std::endl;
		return BREAK ;
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Poll timed out." << std::endl;
		return BREAK ;
	}
	return TRUE;
}

int		Server::checkRecv(int res, int fd)
{
	if (res == ERROR)
	{
		std::cout << "[Server] recv() error\n";
		return ERROR;
	}	
	if (res == 0)
	{
		std::cout << "[Server] User #" << fd << " disconnected\n";
		delClient(fd);
		return ERROR;
	}
	return TRUE;
}

void	Server::checkReception(int rc)
{
	if (rc < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cerr << "[Server] Reception/Sending failed.\n";
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

bool	Server::isValidNickname(std::string name)
{
	std::map<int, Client *>::iterator it = _clients.begin();
	
	while (it != _clients.end())
	{
		if (it->second->getNickname() == name)
			return FALSE;	
	}
	return TRUE;
}

bool	Server::isValidChannelName(std::string cName)
{
    cName[0] == '#' ? cName : cName = "#" + cName;
    std::map<std::string, Channel*>::iterator it = _channels.begin();

    while (it != _channels.end())
    {
        if (it->first == cName)
            return false;
        ++it;
    } 
    return true;
}

void 	addToClientBuffer(Server *server, int cliFd, std::string reply)
{
	Client *client = server->getClient(cliFd);
	
	if (client && !reply.empty())
		client->setRecvMsg(reply);
}
