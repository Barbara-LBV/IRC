/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/25 13:40:42 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void		Server::delClient(std::vector <pollfd> fds, size_t i)
{
	int cliFd = fds[i].fd;
	std::vector<pollfd>::iterator it = fds.begin();
	for (; it != fds.end(); it++)
	{
		if (it->fd == fds[i].fd)
			break ;
	}
	_clients.erase(fds[i].fd);
	fds.erase(it);
	close(cliFd);
	_cliNb--;
	std::cout << BLUE << "[Server] Client #" << cliFd
		<< " successfully disconnected. There is now " << _cliNb << " active connections." DEFAULT << std::endl;
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

void	Server::addClient(std::vector<pollfd> fds, int fd, size_t i)
{
	Client *cli = new Client(fd, this);
	pollfd newFd;

	fcntl(fd, F_SETFL, O_NONBLOCK);
	newFd.fd = fd;
	newFd.events = POLLIN | POLLOUT;
	fds.push_back(newFd);
	_clients.insert(std::pair<int, Client *>(fd, cli));
	if (receiveMsg(fds, fd, i) == BREAK)
		exit(ERROR);
	std::cout << "[Server] Added client #" << fd << " successfully" << std::endl;
	_cliNb++;
}

void	Server::cantAddClient(int cliSocket)
{
	(void) cliSocket;
	std::cout << RED << "[Server] You cannot join, the server is already full" << DEFAULT << std::endl;
	send(cliSocket, "[Server] You cannot join, the server is already full", 53, 0);
	close (cliSocket);
	close(_servFd); // really ?? do we still can recv/send msg with clients ??
}




