/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/07 14:59:33 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void	Server::manageConnections(void)
{
	int 					new_socket;
	std::vector<pollfd>		poll_fds;
	pollfd					servPoll;
	//int conn_count = 1; // Start with one for the server socket
	
	servPoll.fd 		= _servFd;
	servPoll.events 	= POLLIN;
	poll_fds.push_back(servPoll);
	
    while (server_shutdown == FALSE) 
	{
        // Poll for events
		std::vector<pollfd>		new_poll;
        int activity = poll((pollfd *)&poll_fds[0], poll_fds.size(), TIMEOUT);
        if (checkPoll(activity) == BREAK)
			break;
		std::vector<pollfd>::iterator it = poll_fds.begin();
		while (it != poll_fds.end())
		{
			//std::cout << "in global loop, it.fd = " << it->fd << std::endl;
			if (it->revents & POLLIN)
			{
				if (it->fd == _servFd)
				{
					// New incoming connection
					new_socket = addConnections(poll_fds, new_poll);
					if (new_socket == BREAK || new_socket == ERROR)
						continue ;
					std::cout << BGREEN "[Server] " <<  GREEN "New connection on fd #" << new_socket << " accepted.\n" DEFAULT;
				}
				else
				{
					if (receiveMsg(poll_fds, it) ==  BREAK)
						break;
				}
			}
			else if (it->revents & POLLOUT)
			{
				if (managePolloutEvent(poll_fds, it, it->fd) == BREAK)
					break ;
			}
			else if (it->revents & POLLERR)
			{
				if (managePollerEvent(poll_fds, it, it->fd) == BREAK)
					break ;
				else
					exit(ERROR);
			}
			++it;
		}
		poll_fds.insert(poll_fds.end(), new_poll.begin(), new_poll.end()); 
	}
}

int	Server::managePolloutEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, int fd)
{
	Client *client = _clients[fd];
	if (!client)
		std::cout << "[Server] Did not found connexion to client sorry" << std::endl;
	else
	{
		std::string msg = client->getPartialMsg();
		std::vector<std::string> cmds = splitMsg(msg, '\n');
		for(size_t i = 0; i < cmds.size(); i++)
			_handler->invoke(this, client, cmds[i]);
		client->resetPartialMsg();
		if (client->getDeconnStatus() == true)
		{
			delClient(poll_fds, it, fd);
			return (BREAK);
		}
	}
	return (TRUE);
}

int	Server::managePollerEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, int fd)
{
	if (it->fd == _servFd)
	{
		std::cerr << RED << "[Server] Listen socket error" << DEFAULT << std::endl;
		return (ERROR);
	}
	else
	{
		delClient(poll_fds, it, fd);
		return (BREAK);
	}
}