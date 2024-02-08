/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/08 12:50:38 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

int 	Server::addConnections(std::vector<pollfd>&poll_fds, std::vector<pollfd> &new_poll)
{
	int 	cliFd;
	Client *client;
	
	cliFd = acceptConnection();
	if (cliFd == ERROR)
	{
		std::cout << BGREEN "[Server] " <<  GREEN "Coudn't accept incoming connection.\n" DEFAULT;
		return BREAK ;
	}
	if (cliFd > MAXCONN)
	{
		cantAddClient(cliFd);
		return ERROR ;
	}
	client = new Client(cliFd, this);
	if (poll_fds.size() <= MAXCONN)
		addClient(new_poll, client);
	return cliFd;
}

int			Server::receiveMsg(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator it)
{
	(void)poll_fds;
	char	buf[MAXBUF];
	Client *cli = _clients[it->fd];
	
	memset(buf, 0, MAXBUF);
	_result = recv(it->fd, buf, MAXBUF, 0);
	if (checkRecv(_result, it->fd) == ERROR)
	{
		delClient(poll_fds, it, it->fd);
		return BREAK;
	}
	buf[_result] = '\0';
	cli->setPartialMsg(buf);
	std::string fullMsg = cli->getPartialMsg();
	memset(buf, 0, sizeof(MAXBUF));
	if (_result == MAXBUF && fullMsg[_result - 1] != '\n') // if the msg sent by client is longer than the MAXBUF
		std::cout << BBLUE "[Client] " << BLUE "Partial message from " << it->fd << DEFAULT "   << " << cli->getPartialMsg();
	else if (_result <= MAXBUF)
		std::cout << BBLUE "[Client] " << BLUE "Message from " << it->fd << DEFAULT " << " << cli->getPartialMsg();
	return TRUE;
}

void		Server::delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator it, int fd)
{
	_clients.erase(fd);
	poll_fds.erase(it);
	_cliNb--;
	if (_cliNb <= 0)
		_cliNb = 0;
	std::cout << BGREEN "[Server] " <<  GREEN "Client #" << fd
	<< " successfully disconnected. There is now " << _cliNb << " active connections." DEFAULT << std::endl;
	close(fd);
}

void		Server::delChannel(Channel *chan)
{
	std::vector< Channel *>::iterator it = _channels.begin();
    while(it != _channels.end())
    {
		if (*it == chan)
		{
			_channels.erase(it);
			break ;
		}
		++it;
	}
}

void		Server::addClient(std::vector<pollfd> &poll_fds, Client *cli)
{
	pollfd	cliPoll;
	
	cliPoll.fd = cli->getFd();
	cliPoll.events = POLLIN | POLLOUT;
	poll_fds.push_back(cliPoll);
	_clients.insert(std::pair<int, Client*>(cli->getFd(), cli));
	_cliNb++;
}

void		Server::cantAddClient(int cliSocket)
{
	(void) cliSocket;
	std::cout << BGREEN "[Server] " <<  GREEN "You cannot join, the server is already full" << DEFAULT << std::endl;
	send(cliSocket, "[Server] You cannot join, the server is already full", 53, 0);
	close (cliSocket);
	close(_servFd); // we close the listening socket as we cannot add more clients
}
