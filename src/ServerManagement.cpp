/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/18 11:56:26 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void 	Server::manageConnections(void)
{
	int	rc;
	std::vector<pollfd>	poll_fds; // struct { int fd; short events; short revents;}
	pollfd 				serv_poll;
	
	serv_poll.fd 		= _servFd;
	serv_poll.events 	= POLLIN;
	poll_fds.push_back(serv_poll);
	
	while (server_shutdown == FALSE)
	{
		std::vector<pollfd>	tmp_poll;
		rc = poll((pollfd*)&poll_fds[0], (unsigned int)poll_fds.size(), TIMEOUT); // launch poll and check fails and timing
		checkPoll(rc);
		std::vector<pollfd>::iterator	it = poll_fds.begin();
		
		for (; it != poll_fds.end(); ++it)
		{
			// POLLIN => detects events from clients are coming through the socket; socket in readable mode
			std::cout << RED "At the begining of the 2e loop\n" DEFAULT;
			if (it->revents & POLLIN) // syntaxe = test if the revents bit is equal to 1
			{
				// check if server socket is "readable" and loop to accept all incoming connections
				std::cout << RED "In POLLIN, before 'if' \n" DEFAULT;
				if (it->fd == _servFd) // if it's the listening socket (server's)
				{
					std::cout << RED "In soc server, before 'if' \n" DEFAULT;
					if (addConnections(tmp_poll) == TRUE)
						continue; ;
				}
				else // if it's a client already connected to server
				{
					manageExistingConn(*it);// handle every new message => receive mode
					std::cout << RED "In manageExistingConn function\n" DEFAULT;
					break ;
				}
			}
			// POLLOUT => used to know when serv socket is ready to send messages to a client
			else if (it->revents & POLLOUT) 
			{
				std::cout << RED "In managePollOut function\n" DEFAULT;
				//managePolloutEvent()				
				break ;
			}
			// POLLERR => set for a fd referring to the write end of a pipe when the read end has been closed.
			else if (it->revents & POLLERR) 
			{
				/* the socket is diconnected so we clear the right Client node, clear the current fd etc */
				std::cout << "[Server] FD " << it->fd << "disconnected \n";
				//managePollerrEvents(it->fd);
				break ;
			}
		}
		poll_fds.insert(poll_fds.end(), tmp_poll.begin(), tmp_poll.end());
	}
}

bool 	Server::addConnections(std::vector<pollfd> tmpPoll)
{
	int cliFd;
	
	cliFd = acceptConnection();
	if (_cliNb <= MAXCONN)
	{
		std::cout << "[Server] New incoming connection on fd n#" << cliFd << std::endl;
		//tmpPoll.push_back(cliSocket);
		addClient(tmpPoll, cliFd);// function that fill the "_client" variable with all the client's infos
	}
	else
	{
		cantAddClient(cliFd); // what do we do when we cannot add more client ?
		return FALSE;
	}
	return TRUE;
}

void 	Server::manageExistingConn(pollfd fd)
{
	//handle the reception only with recv, then check with read if \0 is found
	//otherwise handle the fact that the whole msg hasn't been received
	
	//parse the received msg
	std::cout << "In manage existing connection function" << fd.fd << std::endl;
}

/*  managePolloutEvent(int cliFd) => client en mode ecoute 
 1- clear les msg enregistres du client concerne
 2- le server envoie un msg au client concerne selon le cas 
     (msg collectif, accuse de reception, entree dans channel etc)
 2- verifier le statut du client pour un eventuel delete */

 /*  void managePollerrEvents() => si une erreur arrive sur les sockets
 1- si c'est la socket server => close
 2- si socket client, delete client concerne 
 void	managePollerrEvent(int fd)
 {
	if (fd == _servFd)
		close(_servFd);
	else
		delClient(poll_fds, fd);
 }
 */


