/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/11 17:23:57 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void 	Server::manageConnections(void)
{
	int	rc, timeout = 3 * 60 * 100;
	std::vector<pollfd>	poll_fds, new_poll;
	pollfd 				serv_poll;
	
	serv_poll.fd 		= _socServ;
	serv_poll.events 	= POLLIN;
	poll_fds.push_back(serv_poll);
	
	while (server_shutdown == FALSE)
	{
		std::vector<pollfd>	tmp_poll;
		rc = poll((pollfd*)&poll_fds[0], (unsigned int)poll_fds.size(), timeout); // launch poll and check fails and timing
		checkPoll(rc);
		std::vector<pollfd>::iterator	it = poll_fds.begin();
		
		while (it != poll_fds.end())
		{
			// POLLIN => events which are coming through; socket in readable mode
			if (it->revents & POLLIN) // syntaxe = test if the revents bit is equal to 1
			{
				// check if server socket is "readable" and loop to accept all incoming connections
				if (it->fd == _socServ) // if it's the listening socket (server's)
				{
					if (addConnections(tmp_poll, it) == TRUE)
						continue ;
				}
				else // if it's a client already connected to server
				{
					std::cout << "[Server] FD " << it->fd << "is readable\n";
					manageExistingConn(*it);
				}
			}
			// POLLOUT => fd is ready to write and send messages through the socket
			else if (it->revents & POLLOUT) 
			{
				if (it->fd == _socServ) // if it's the listening socket (server's)
				{
					std::cout << "[Server] Server socket is writable\n";
				}
				else
				/* the socket is in Writable mode, so we send() messages */
				{
					std::cout << "[Server] FD " << it->fd << "is writable\n";
					manageExistingConn(*it);
				}
			}
			// POLLERR => set for a fd referring to the write end of a pipe when the read end has been closed.
			else if (it->revents & POLLERR) 
			{
				/* the socket is diconnected so we clear the right Client node, clear the current fd etc */
				std::cout << "[Server] FD " << it->fd << "disconnected \n";
				delClient(poll_fds);
			}
			++it;
		}
		poll_fds.insert(poll_fds.end(), tmp_poll.begin(), tmp_poll.end());
	}
}

bool 	Server::addConnections(std::vector<pollfd> fd, std::vector<pollfd>::iterator it)
{
	it->fd = acceptConnection();
	it->events = POLLIN;
	std::cout << "[Server] New incoming connection - " << it->fd << std::endl;
	if (_cliNb < MAXCONN)
		addClient(fd);	// function that fill the "_client" variable with all the client's infos
	else
		cantAddClient(fd); // what do we do when we cannot add more client ?
	return TRUE;
}

void 	Server::manageExistingConn(pollfd fd)
{
	if (fd.revents & POLLIN)
	{
		int rc = recv(fd.fd, _buf, sizeof(_buf), 0);
		int i = 0;
		_result = recv(fd.fd, _buf, sizeof(_buf), 0);
		
		checkReception(_result);
		//handle the reception only with recv, then check with read if \0 is found
		//otherwise handle the fact that the whole msg hasn't been received
		while (_result != 0)
		{
		}
	}
	else if (fd.revents & POLLOUT)
	{
		int rc = send(fd.fd, _buf, ??, 0);
		checkReception(rc);

	}
}

void	Server::addClient(std::vector<pollfd> fd)
{
	// 1/ add a new client node in _clients
	// 2/ add a new node in fd
	// 3/ make de client registration (?)
	_cliNb++;
}

void	Server::cantAddClient(std::vector<pollfd> fd)
{
	// ??????
}