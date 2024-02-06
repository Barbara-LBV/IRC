/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/06 19:02:18 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::manageConnections(void)
{
	int 	new_socket, rc;
	pollfd	servPoll;
	//int conn_count = 1; // Start with one for the server socket
	
	servPoll.fd 		= _servFd;
	servPoll.events 	= POLLIN;
	_poll_fds.push_back(servPoll);
	
    while (server_shutdown == FALSE) 
	{
        // Poll for events
        int activity = poll((pollfd *)&_poll_fds[0], (unsigned int)_poll_fds.size(), TIMEOUT);
        if (checkPoll(activity) == BREAK)
			break;
		for (size_t i = 0; i < _poll_fds.size() ; ++i)
		{
			if (_poll_fds[i].revents == 0)
				continue ; 
        	else if (_poll_fds[i].fd == _servFd)
			{
				// New incoming connection
				new_socket = addConnections();
				if (new_socket == BREAK || new_socket == ERROR)
					continue ;
				std::cout << BGREEN "[Server] " <<  GREEN "New connection on fd #" << new_socket << " accepted.\n" DEFAULT;
			}
			else if (i > 0)
				rc = receiveMsg(_poll_fds[i].fd);
		}
	}
}

int 	Server::addConnections(void)
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
	if (cliFd > 3 && cliFd <= MAXCONN)
		addClient(client);
	return cliFd;
}

int	Server::receiveMsg(int fd)
{
	char	buf[MAXBUF];
	Client *cli = _clients[fd];
	
	memset(buf, 0, MAXBUF);
	_result = recv(fd, buf, MAXBUF, 0);
	if (checkRecv(_result, fd) == ERROR)
	{
		delClient(fd);
		return BREAK;
	}
	buf[_result] = '\0';
	cli->setPartialMsg(buf);
	std::string fullMsg = cli->getPartialMsg();
	memset(buf, 0, sizeof(MAXBUF));
	if (_result == MAXBUF && fullMsg[_result - 1] != '\n') // if the msg sent by client is longer than the MAXBUF
	{
		std::cout << BBLUE "[Client] " << BLUE "Partial message received from " << fd << DEFAULT "   << " << cli->getPartialMsg() << std::endl;
		return TRUE;
	}
	else if (_result <= MAXBUF)
	{
		std::cout << BBLUE "[Client] " << BLUE "Message received from " << fd << DEFAULT " << " << cli->getPartialMsg() << std::endl;
		cli->resetPartialMsg();
		std::vector<std::string> cmds = splitMsg(fullMsg, '\n');
		for(size_t i = 0; i < cmds.size(); i++)
			_handler->invoke(this, cli, cmds[i]);
		return TRUE;
	}
	return FALSE;
}
