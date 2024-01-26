/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/26 15:09:13 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void		Server::delClient(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[fd])
			break ;
	}
	_clients.erase(fd);
	close(fd);
	_cliNb--;
	std::cout << BLUE << "[Server] Client #" << fd 
		<< " successfully disconnected. There is now " << _cliNb 
			<< " active connections." DEFAULT << std::endl;
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

void	Server::addClient(int fd)
{
	Client *cli = new Client(fd, this);
	fcntl(fd, F_SETFL, O_NONBLOCK);
	std::cout << "bp#0 in addClient function"<< std::endl;
	_clients.insert(std::pair<int, Client *>(fd, cli));
	std::cout << "bp#1 in addClient function"<< std::endl;
	std::cout << "size of vector clients = " << _clients.size() << std::endl;
	constructFds();
	std::cout << "bp#2 in addClient function"<< std::endl;
	if (receiveMsg(fd) == BREAK)
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




