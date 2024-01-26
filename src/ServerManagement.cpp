/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/26 17:09:44 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

//int	Server::manageConnections(void)
//{
//	int	rc;
//	std::vector<pollfd>	poll_fds; // struct { int fd; short events; short revents;}
//	pollfd 				serv_poll;
	
//	serv_poll.fd 		= _servFd;
//	serv_poll.events 	= POLLIN;
//	poll_fds.push_back(serv_poll);
	
//	while (server_shutdown == FALSE)
//	{
//		std::vector<pollfd>	tmp_poll;
//		rc = poll((pollfd*)&poll_fds[0], (unsigned int)poll_fds.size(), TIMEOUT); // launch poll and check fails and timing
//		checkPoll(rc);
//		std::vector<pollfd>::iterator	it = poll_fds.begin();
//		for (;it != poll_fds.end(); ++it)
//		{
//			// POLLIN => detects events from clients are coming through the socket; socket in readable mode
//			 if (it->revents & POLLIN) // syntaxe = test if the revents bit is equal to 1
//			 {
//			 	// check if server socket is "readable" and loop to accept all incoming connections
//				std::cout << RED "In POLLIN, bp #1" DEFAULT;
//				if (it->fd == _servFd) // if it's the listening socket (server's)
//				{
//					if (addConnections(tmp_poll, it) == CONTINUE)
//						continue ;
//				}
//				else // if it's a client already connected to server
//				{
//					std::cout << RED "In soc server, bp#2 \n" DEFAULT;
//					if (receiveMsg(poll_fds, it, it->fd) == BREAK)// handle every new message => receive mode
//						break ;
//				}
//			 }
//			 //POLLOUT => used to know when serv socket is ready to send messages to a client
//			 else if (it->revents & POLLOUT) 
//			 {
//				std::cout << RED "In soc server, bp#6 \n" DEFAULT;
//			 	if (managePolloutEvent(poll_fds, it, it->fd) == BREAK)			
//			 		break ;
//			 }
//			 // POLLERR => set for a fd referring to the write end of a pipe when the read end has been closed.
//			 else if (it->revents & POLLERR) 
//			 {
//			 	// the socket is diconnected so we clear the right Client node, clear the current fd etc
//				std::cout << RED "In soc server, bp#7 \n" DEFAULT;
//			 	std::cout << "[Server] FD " << it->fd << "disconnected \n";
//			 	if (managePollerrEvents(poll_fds, it, it->fd) == BREAK)
//			 		break ;
//				else
//					return ERROR;
//			 }
//			 std::cout << RED "In soc server, bp#8 \n" DEFAULT;
//			//++it;
//		}
//		poll_fds.insert(poll_fds.end(), tmp_poll.begin(), tmp_poll.end());
//	}
//	return TRUE;
//}

int	Server::manageConnections(void)
{
	int	rc(0);
	
	_clientsFds->fd 	= _servFd;
	_clientsFds->events = POLLIN;
	
	while (server_shutdown == FALSE)
	{
		rc = poll(_clientsFds, (unsigned int)_clients.size() + 1, TIMEOUT); // launch poll and check fails and timing
		checkPoll(rc);
		std::cout << "bp#1 in loop function"<< std::endl;
		size_t i = 0;
		while (i < _clients.size() + 1)
		{
			std::cout << "bp#2 in loop function"<< std::endl;
			if (_clientsFds[i].revents == 0)
				continue ;
			// POLLIN => detects events from clients are coming through the socket; socket in readable mode
			if (_clientsFds[i].fd == _servFd) // if it's the listening socket (server's)
			{
				addConnections();
				std::cout << RED "bp#1 in loop function i= " << i << std::endl;
			}
			else if (i > 0)// if it's a client already connected to server
			{
				std::cout << RED "In soc server, bp#2 \n" DEFAULT;
				std::cout << RED "bp#2 in loop function i= " DEFAULT << i << std::endl;
				receiveMsg(_clientsFds[i].fd);
			}
			// //POLLOUT => used to know when serv socket is ready to send messages to a client
			// else if (it->revents & POLLOUT) 
			// {
			//	std::cout << RED "In soc server, bp#6 \n" DEFAULT;
			// 	if (managePolloutEvent(poll_fds, it, it->fd) == BREAK)			
			// 		break ;
			// }
			// // POLLERR => set for a fd referring to the write end of a pipe when the read end has been closed.
			// else if (it->revents & POLLERR) 
			// {
			// 	// the socket is diconnected so we clear the right Client node, clear the current fd etc
			//	std::cout << RED "In soc server, bp#7 \n" DEFAULT;
			// 	std::cout << "[Server] FD " << it->fd << "disconnected \n";
			// 	if (managePollerrEvents(poll_fds, it, it->fd) == BREAK)
			// 		break ;
			//	else
			//		return ERROR;
			// }
			// std::cout << RED "In soc server, bp#8 \n" DEFAULT;
			++i;
		}
		//poll_fds.insert(poll_fds.end(), tmp_poll.begin(), tmp_poll.end());
	}
	return TRUE;
}

int 	Server::addConnections(void)
{
	int cliFd;
	
	cliFd = acceptConnection();
	if (_cliNb == ERROR)
		return CONTINUE;
	if (_cliNb <= MAXCONN)
	{
		std::cout << "[Server] New incoming connection on fd n#" << cliFd << std::endl;
		//fds, size_t i.push_back(cliFd);
		addClient(cliFd);// function that fill the "_client" variable with all the client's infos
	
	}
	else
		cantAddClient(cliFd); // what do we do when we cannot add more client ?
	return TRUE;
}

int	Server::receiveMsg(int fd)
{
	char	buf[MAXBUF];
	Client *cli = _clients[fd];
	memset(buf, 0, sizeof(buf));
	std::cout << "in received function fd =" << fd << std::endl;
	_result = recv(fd, buf, MAXBUF, 0);
	if (checkRecv(_result, fd) == ERROR)
	{
		delClient(fd);
		memset(buf, 0, sizeof(buf));
		return BREAK;
	}
	buf[_result] = '\0';
	setMsg(buf);
	if (_result == MAXBUF && _cliMsg[_result] != '\n') // if the msg sent by client is longer than the MAXBUF
	{
		cli->setPartialMsg(buf); // to keep the msg until you manage to get the rest of the incoming msg
		_cliMsg.clear();
		return TRUE;
	}
	else if (_cliMsg[_result - 1] == '\n')
	{
		std::vector<std::string> cmds = splitMsg(_cliMsg, '\n');
		for(size_t i = 0; i < cmds.size(); i++)
			_handler->invoke(this, cli, cmds[i]);
		if (_clients[fd]->getWelcomeStatus() == FALSE)
		{
			_clients[fd]->sendReply(fd);
			_clients[fd]->setWelcomeStatus(TRUE);
		}
		else
			_clients[fd]->sendReply(fd);
		cli->setPartialMsg(_cliMsg);
		std::cout << "[Client] Message received from " << fd << " << " << cli->getPartialMsg() << std::endl;
		cli->setPartialMsg("");
		_cliMsg.clear();
		memset(buf, 0, sizeof(buf));
		return TRUE;
	}
	return FALSE;
}

int 	Server::managePolloutEvent(int fd) //=> client en mode ecoute
{
	Client *client = getClient(fd);
	if (!client)
		std::cout << "[Server] Didn't find connexion to client sorry" << std::endl;
	else
	{
		client->sendReply(fd);
		if (client->getDeconnStatus() == true)
		{
			delClient(fd);
			return (BREAK);
		}
	}
	return (TRUE);
}
/* 1- clear les msg enregistres du client concerne
 2- le server envoie un msg au client concerne selon le cas 
     (msg collectif, accuse de reception, entree dans channel etc)
 2- verifier le statut du client pour un eventuel delete */

 int	Server::managePollerrEvents(int fd)
 {
	if (fd == _servFd)
	{
		close(_servFd);
		exit(ERROR);
	}
	if (fd != _servFd)
	{
		delClient(fd);
		return BREAK;
	}
	return TRUE;
 }
