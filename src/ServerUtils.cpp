/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:19:04 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/01 15:12:16 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::createServerSocket(void)
{
	int rc, fd, on = 1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == ERROR)
	{
		std::cerr << "[Server] Socket creation error" << std::endl;
		server_shutdown = TRUE;
		close(fd);
		exit(ERROR);
	}
	setFd(fd);
    rc = setsockopt(getFd(), SOL_SOCKET, SO_REUSEPORT, (const void*)&on, sizeof(on));
	if (rc == ERROR)
	{
		std::cerr << "[Server] Impossible to reuse the socket" << std::endl;
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
		std::cerr << "[Server] Socket impossible to bind" << std::endl;
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
		std::cerr << "[Server] Socket cannot listen" << std::endl;
		server_shutdown = TRUE;
		close(getFd());
		exit(ERROR);
	}
	std::cout << "[Server] Listening on socket fd: " << _servFd << std::endl;
}

int		Server::acceptConnection(void)
{
	socklen_t len = sizeof (_hints);
	int cliFd = accept(getFd(), (struct sockaddr *)&_hints, &len);
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
		server_shutdown = TRUE;
		//free function
		exit(ERROR);
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
		delClient(fd);
		return ERROR;
	}
	return TRUE;
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
	
	if (client)
		client->setRecvMsg(reply);
}
