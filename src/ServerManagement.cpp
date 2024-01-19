/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/19 12:51:18 by pmaimait         ###   ########.fr       */
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
		
		while (it != poll_fds.end())
		{
			// POLLIN => detects events from clients are coming through the socket; socket in readable mode
			std::cout << RED "At the begining of the 2e loop\n" DEFAULT;
			if(it->revents == 0)
				continue;
			// if (it->revents & POLLIN) // syntaxe = test if the revents bit is equal to 1
			// {
			// 	// check if server socket is "readable" and loop to accept all incoming connections
			//std::cout << RED "In POLLIN, before 'if' \n" DEFAULT;
			if (it->fd == _servFd) // if it's the listening socket (server's)
			{
				std::cout << RED "In soc server, before 'if' \n" DEFAULT;
				addConnections(tmp_poll);
				//poll_fds.insert(poll_fds.end(), tmp_poll.begin(), tmp_poll.end());
			}
			else if (it->fd != _servFd)// if it's a client already connected to server
			{
				std::cout << RED "In manageExistingConn function\n" DEFAULT;
				if (receiveMsg(_clients[it->fd], it->fd) == BREAK)// handle every new message => receive mode
					break ;
			}
			//}
			// POLLOUT => used to know when serv socket is ready to send messages to a client
			// else if (it->revents & POLLOUT) 
			// {
			// 	std::cout << RED "In managePollOut function\n" DEFAULT;
			// 	// if (managePolloutEvent() == BREAK)			
			// 	break ;
			// }
			// // POLLERR => set for a fd referring to the write end of a pipe when the read end has been closed.
			// else if (it->revents & POLLERR) 
			// {
			// 	/* the socket is diconnected so we clear the right Client node, clear the current fd etc */
			// 	std::cout << "[Server] FD " << it->fd << "disconnected \n";
			// 	if (managePollerrEvents(poll_fds, it, it->fd) == BREAK)
			// 	break ;
			// }
			it++;
		}
		poll_fds.insert(poll_fds.end(), tmp_poll.begin(), tmp_poll.end());
	}
}

int 	Server::addConnections(std::vector<pollfd> tmpPoll)
{
	int cliFd;
	
	cliFd = acceptConnection();
	if (_cliNb == ERROR)
		return CONTINUE;
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

int	Server::receiveMsg(Client *cli, int fd)
{
	char	buf[MAXBUF];
	
	memset(buf, 0, sizeof(buf));
	_result =  recv(fd, buf, MAXBUF, 0);
	if (checkRecv(_result, fd) == BREAK)
		return BREAK;
	buf[_result] = '\0';
	setMsg(buf);
	if (_result == MAXBUF && _cliMsg[_result] != '\n') // if the msg sent by client is longer than the MAXBUF
	{
		cli->setPartialMsg(buf); // to keep the msg until you manage to get the rest of the incoming msg
		std::cout << "[Client] partial message received from " << fd << " << " << cli->getPartialMsg() << std::endl;
		_cliMsg.clear();
	}
	else
	{
		//parse and handle commandes => handler.invoke();
		cli->setPartialMsg(buf); 
		std::cout << "[Client] Message received from " << fd << " << " << cli->getPartialMsg() << std::endl;
		_cliMsg.clear();
	}
	return TRUE;
}

int 	Server::managePolloutEvent(std::vector<pollfd> fds, std::vector<pollfd>::iterator it, int fd) //=> client en mode ecoute
{
	Client *client = getClient(fd);
	if (!client)
		std::cout << "[Server] Did not found connexion to client sorry" << std::endl;
	else
	{
		// sendMsg(client->getMsgSent());
		// client->getMsgSent().clear();
		if (client->getDeconnStatus() == true)
		{
			delClient(fds, it);
			return (BREAK);
		}
	}
	return (TRUE);
}
/* 1- clear les msg enregistres du client concerne
 2- le server envoie un msg au client concerne selon le cas 
     (msg collectif, accuse de reception, entree dans channel etc)
 2- verifier le statut du client pour un eventuel delete */

 int	Server::managePollerrEvents(std::vector<pollfd> fds, std::vector<pollfd>::iterator it, int fd)
 {
	if (fd == _servFd)
	{
		close(_servFd);
		exit(ERROR);
	}
	if (fd != _servFd)
	{
		delClient(fds, it);
		return BREAK;
	}
	return TRUE;
 }
 


