/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/15 14:36:34 by blefebvr         ###   ########.fr       */
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
			// POLLIN => detects events from clients are coming through the socket; socket in readable mode
			if (it->revents & POLLIN) // syntaxe = test if the revents bit is equal to 1
			{
				// check if server socket is "readable" and loop to accept all incoming connections
				if (it->fd == _socServ) // if it's the listening socket (server's)
				{
					if (addConnections(tmp_poll) == TRUE)
						continue ;
				}
				else // if it's a client already connected to server
				{
					manageExistingConn(*it);// handle every new message => receive mode
					break ;
				}
			}
			// POLLOUT => used to know when serv socket is ready to send messages to a client
			else if (it->revents & POLLOUT) 
			{
				//managePolloutEvent()				
				break ;
			}
			// POLLERR => set for a fd referring to the write end of a pipe when the read end has been closed.
			else if (it->revents & POLLERR) 
			{
				//managePollerrEvents()	
				/* the socket is diconnected so we clear the right Client node, clear the current fd etc */
				std::cout << "[Server] FD " << it->fd << "disconnected \n";
				delClient(poll_fds, it);
				break ;
			}
			++it;
		}
		poll_fds.insert(poll_fds.end(), tmp_poll.begin(), tmp_poll.end());
	}
}

bool 	Server::addConnections(std::vector<pollfd> tmpPoll)
{
	pollfd 		cliSocket;
	
	cliSocket.fd = acceptConnection();
	cliSocket.events = POLLIN;
	
	if (_cliNb <= MAXCONN)
	{
		std::cout << "[Server] New incoming connection - " << cliSocket.fd << std::endl;
		tmpPoll.push_back(cliSocket);
		addClient(tmpPoll, cliSocket.fd);// function that fill the "_client" variable with all the client's infos
	}
	else
		cantAddClient(cliSocket.fd); // what do we do when we cannot add more client ?
	return TRUE;
}

void 	Server::manageExistingConn(pollfd fd)
{
	//handle the reception only with recv, then check with read if \0 is found
	//otherwise handle the fact that the whole msg hasn't been received
	
	//parse the received msg
	std::cout << "In manage existing connection function" << fd.fd << std::endl;
}

/*  managePolloutEvent => send message */

//void or bool 		managePollerrEvents() => check if there is a disconnection;

