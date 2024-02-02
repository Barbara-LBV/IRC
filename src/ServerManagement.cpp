/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/02 11:40:59 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::manageConnections(void)
{
	int 	new_socket, rc;
	pollfd	servPoll;
	int conn_count = 1; // Start with one for the server socket
	
	servPoll.fd 		= _servFd;
	servPoll.events 	= POLLIN;
	_poll_fds.push_back(servPoll);
	
    while (server_shutdown == FALSE) 
	{
        // Poll for events
		int i = 0;
        int activity = poll((pollfd *)&_poll_fds[0], (unsigned int)_poll_fds.size(), TIMEOUT);
        if (checkPoll(activity) == BREAK)
			break;
		for (; i < conn_count ; i++)
		{
			std::cout << "in loop for i=" << i << std::endl;
			std::cout << "in loop for poll size=" << _poll_fds.size() << std::endl;
			//std::cout << "in loop for poll revent[i]=" << _poll_fds[i].revents<< std::endl;
			if (_poll_fds[i].revents == 0)
				continue ; 
        	if (_poll_fds[i].fd == _servFd)
			{
				// New incoming connection
				new_socket = addConnections();
				if (new_socket == BREAK || new_socket == ERROR)
					continue ;
				conn_count++;
				std::cout << "New connection on fd #" << new_socket << " accepted.\n";
			}
			else if (i > 0)
			{
				std::cout << "in loop recev " << std::endl;
				rc = receiveMsg(_poll_fds[i].fd);
				if (rc == BREAK && conn_count > 1)
					conn_count--;
			}
		}
	}
}

//void	Server::manageConnections(void)
//{
//	int 	new_socket, rc;
//	pollfd	servPoll;
//	int client_count = 1; // Start with one for the server socket

		
//	servPoll.fd 		= _servFd;
//	servPoll.events 	= POLLIN | POLLOUT;
//	_poll_fds.push_back(servPoll);
	
//    while (server_shutdown == FALSE) 
//	{
//        // Poll for events
//        int activity = poll((pollfd *)&_poll_fds[0], client_count, TIMEOUT);
//        if (checkPoll(activity) == BREAK)
//			break;
//		for (int i = 0; i < client_count; i++)
//		{
//        	if (_poll_fds[i].revents & POLLIN)
//			{
//				if (_poll_fds[i].fd == _servFd)
//				{	// New incoming connection
//					new_socket = addConnections();
//					if (new_socket == BREAK || new_socket == ERROR)
//						continue ;
//					client_count++;
//					std::cout << "New connection on fd #" << new_socket << " accepted.\n";
//				}
//				else
//				{
//					rc = receiveMsg(_poll_fds[i].fd);
//				if (rc == BREAK && client_count > 1)
//					client_count--;
//				}
//			}
//			else if (_poll_fds[i].revents & POLLOUT)
//			{
//				if (managePolloutEvent(_poll_fds[i].fd) == BREAK)
//					client_count--;
//			}
//			else if (_poll_fds[i].revents & POLLERR)
//			{
//				managePollerrEvents(_poll_fds[i].fd);
//			}
//		}
//	}
//}

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
	std::cout << "_result =" << _result << std::endl;
	std::cout << "buf =" << buf << std::endl;
	if (checkRecv(_result, fd) == ERROR)
		return BREAK;

	buf[_result] = '\0';
	cli->setPartialMsg(buf);
	std::string fullMsg = cli->getPartialMsg();
	memset(buf, 0, sizeof(MAXBUF));
	if (_result == MAXBUF && fullMsg[_result - 1] != '\n') // if the msg sent by client is longer than the MAXBUF
	{
		std::cout << "[Client] Partial message received from " << fd << "   << " << cli->getPartialMsg() << std::endl;
		return TRUE;
	}
	else if (_result <= MAXBUF)
	{
		std::cout << "[Client] Message received from " << fd << " << " << cli->getPartialMsg() << std::endl;
		cli->resetPartialMsg();
		std::vector<std::string> cmds = splitMsg(fullMsg, '\n');
		for(size_t i = 0; i < cmds.size(); i++)
			_handler->invoke(this, cli, cmds[i]);
		//cli->sendReply(fd);
		return TRUE;
	}
	return FALSE;
}

int 	Server::managePolloutEvent(int fd) //=> client en mode ecoute
{
	Client *client = _clients[fd];
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
	else
	{
		delClient(fd);
		return BREAK;
	}
	return TRUE;
 }
 


