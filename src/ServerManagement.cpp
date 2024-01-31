/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/31 15:04:06 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::manageConnections(void)
{
	int 	new_socket;
	pollfd	servPoll;
	int client_count = 1; // Start with one for the server socket
	
	servPoll.fd 		= _servFd;
	servPoll.events 	= POLLIN;
	_poll_fds.push_back(servPoll);
	
    while (server_shutdown == FALSE) 
	{
        // Poll for events
        int activity = poll((pollfd *)&_poll_fds[0], client_count, TIMEOUT);
        if (checkPoll(activity) == BREAK)
			break ;
		for (int i = 0; i < client_count; i++)
		{
			if (_poll_fds[i].revents == 0)
					continue ; 
        	if (_poll_fds[i].fd == getFd())
			{
				// New incoming connection
				new_socket = addConnections();
				//if (new_socket == BREAK || new_socket == ERROR)
				//	continue ;
				client_count++;
				std::cout << "New connection on fd #" << new_socket << " accepted.\n";
			}
			else
			{
				if (receiveMsg(_poll_fds[i].fd) == BREAK)
				{
					// Client disconnected
					std::cout << "Client disconnected" << std::endl;
					delClient(_poll_fds[i].fd);
					client_count--;
				}
			}
		}
	}
}

int 	Server::addConnections(void)
{
	int cliFd(0);
	
	cliFd = acceptConnection();
	if (cliFd == ERROR)
	{
		std::cout << "[Server] Coudn't accept incoming connection.\n";
		return BREAK ;
	}
	else if (cliFd <= MAXCONN)
	{
		std::cout << "[Server] New incoming connection on fd n#" << cliFd << std::endl;
		if (addClient(cliFd) == BREAK)
			return BREAK ;
	}
	if (cliFd > MAXCONN)
	{
		cantAddClient(cliFd);
		return ERROR ;
	}
	return cliFd;
}

int	Server::receiveMsg(int fd)
{
	char	buf[MAXBUF];
	Client *cli = _clients[fd];
	
	memset(buf, 0, MAXBUF);
	_result = recv(fd, buf, MAXBUF, 0);
	
	if (checkRecv(_result, fd) == ERROR)
		return BREAK;
		
	buf[_result] = '\0';
	//setMsg(buf);
	cli->setPartialMsg(buf);
	std::string fullMsg = cli->getPartialMsg();
	memset(buf, 0, sizeof(MAXBUF));
	if (_result == MAXBUF && fullMsg[_result - 1] != '\n') // if the msg sent by client is longer than the MAXBUF
	{
		std::cout << "[Client] Partial message received from " << fd << "   << " << cli->getPartialMsg() << std::endl;
		return TRUE;
	}
	else if (fullMsg[_result - 1] == '\n')
	{
		std::vector<std::string> cmds = splitMsg(fullMsg, '\n');
		for(size_t i = 0; i < cmds.size(); i++)
			_handler->invoke(this, cli, cmds[i]);
		//if (_clients[fd]->getWelcomeStatus() == TRUE)
		cli->sendReply(fd);
		std::cout << "[Client] Message received from " << fd << " << " << cli->getPartialMsg() << std::endl;
		cli->setPartialMsg("");
		fullMsg.clear();
		return TRUE;
	}
	return FALSE;
}

int 	Server::managePolloutEvent(std::vector<pollfd> fds, int fd, size_t i) //=> client en mode ecoute
{
	Client *client = getClient(fds[i].fd);
	if (!client)
		std::cout << "[Server] Didn't find connexion to client sorry" << std::endl;
	else
	{
		client->sendReply(fd);
		if (client->getDeconnStatus() == true)
		{
			delClient(i);
			return (BREAK);
		}
	}
	return (TRUE);
}
/* 1- clear les msg enregistres du client concerne
 2- le server envoie un msg au client concerne selon le cas 
     (msg collectif, accuse de reception, entree dans channel etc)
 2- verifier le statut du client pour un eventuel delete */

 int	Server::managePollerrEvents(std::vector<pollfd> fds, size_t i)
 {
	if (fds[i].fd == _servFd)
	{
		close(_servFd);
		exit(ERROR);
	}
	if (fds[i].fd != _servFd)
	{
		delClient(i);
		return BREAK;
	}
	return TRUE;
 }
 


