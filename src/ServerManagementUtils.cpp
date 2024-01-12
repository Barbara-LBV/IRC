/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/12 18:41:17 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void 	Server::checkPoll(int rc)
{
	if (rc == ERROR && server_shutdown == TRUE)
		exit(ERROR);
	if (rc == ERROR)
	{
		std::cerr << "[Server] Poll failed" << std::endl;
		exit(ERROR);
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Poll timed out." << std::endl;
		server_shutdown == TRUE;
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
			exit(ERROR);
		}
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Connection closed." << std::endl;
		exit(ERROR);
	}
}

void		Server::delClient(std::vector <pollfd> fds, std::vector <pollfd>:: iterator &it, int cliFd)
{
	_clients.erase(it->fd);
	fds.erase(it);
	close(cliFd);
	_cliNb--;
	std::cout << BLUE << "[Server] Client #" << cliFd
		<< " successfully disconnected. There is now " << _cliNb << " active connections." DEFAULT << std::endl;
}

void		Server::delChannel(std::string topic)
{
	/* quid =>*/
	
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

void	Server::addClient(std::vector<pollfd> fds, int fd)
{
	Client *cli = new Client(fd);
	pollfd newFd;

	newFd.fd = fd;
	newFd.events = POLLIN | POLLOUT;
	fds.push_back(newFd);
	_clients.insert(std::pair<int, Client *>(fd, cli));
	std::cout << BLUE << "[Server] Added client #" << fd 
		<< " successfully" << DEFAULT << std::endl;
	/* put here the registration client function ?? */
	_cliNb++;
}

void	Server::cantAddClient(int cliSocket)
{
	std::cout << RED << ERR_FULL_SERV << DEFAULT << std::endl;
	send(cliSocket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, 0);
	close(_socServ); // really ?? does we still can recv/send msg with clients ??
}

//managePolloutEvent()	
//managePollerrEvent()	