/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/31 15:53:40 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void		Server::delClient(int fd)
{
	_clients.erase(fd);
	std::vector<pollfd>::iterator it = _poll_fds.begin();
	for (int i = 0; i < _cliNb + 1 ; ++i)
	{
		if (fd == it->fd)
		{
			_poll_fds.erase(it);
			break ;
		}
		++it;
	}
	_cliNb--;
	std::cout << BLUE << "[Server] Client #" << it->fd
	<< " successfully disconnected. There is now " << _cliNb << " active connections." DEFAULT << std::endl;
	close(fd);
}

void		Server::delChannel(std::string topic)
{
	// 1- withdraw chanop privigeles to the admin-client => change its status
	// 2- reduce the server's channel variable _channel
	std::map<std::string, Channel *>::iterator it = _channels.begin();
    while(it != _channels.end())
    {
		if (it->first == topic)
		{
			_channels.erase(it);
			break ;
		}
		++it;
	}
}

int Server::addClient(int fd)
{
	Client *cli = new Client(fd, this);
	pollfd	cliPoll;
	
	cliPoll.fd = fd;
	cliPoll.events = POLLIN;
	_clients.insert(std::pair<int, Client *>(fd, cli));
	_poll_fds.push_back(cliPoll);
	_cliNb++;
	//if (receiveMsg(fd) == BREAK)
	//	return BREAK ;
	return TRUE;
}

void	Server::cantAddClient(int cliSocket)
{
	(void) cliSocket;
	std::cout << RED << "[Server] You cannot join, the server is already full" << DEFAULT << std::endl;
	send(cliSocket, "[Server] You cannot join, the server is already full", 53, 0);
	close (cliSocket);
	close(_servFd); // we close the listening socket as we cannot add more clients
}




