/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/06 18:51:53 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void		Server::delClient(int fd)
{
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
	_clients.erase(fd);
	_cliNb--;
	if (_cliNb <= 0)
		_cliNb = 0;
	std::cout << BGREEN "[Server] " <<  GREEN "Client #" << fd
	<< " successfully disconnected. There is now " << _cliNb << " active connections." DEFAULT << std::endl;
	close(fd);
}

void		Server::delChannel(std::string topic)
{
	std::vector< Channel *>::iterator it = _channels.begin();
    while(it != _channels.end())
    {
		if ((*it)->getTopic() == topic)
		{
			_channels.erase(it);
			break ;
		}
		++it;
	}
}

void		Server::addClient(Client *cli)
{
	pollfd	cliPoll;
	
	cliPoll.fd = cli->getFd();
	cliPoll.events = POLLIN;
	_poll_fds.push_back(cliPoll);
	_clients.insert(std::pair<int, Client*>(cli->getFd(), cli));
	_cliNb++;
}

void	Server::cantAddClient(int cliSocket)
{
	(void) cliSocket;
	std::cout << BGREEN "[Server] " <<  GREEN "You cannot join, the server is already full" << DEFAULT << std::endl;
	send(cliSocket, "[Server] You cannot join, the server is already full", 53, 0);
	close (cliSocket);
	close(_servFd); // we close the listening socket as we cannot add more clients
}




