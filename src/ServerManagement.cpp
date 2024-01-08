/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/08 17:41:49 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void 	Server::manageConnections(void)
{
	int rc, compress_fds = FALSE, close_conn = FALSE;
	int	timeout = 3 * 60 * 100;
	
	std::vector<pollfd>	poll_fds;
	pollfd 				serv_poll, new_poll;
	
	serv_poll.fd 		= _socServ;
	serv_poll.events 	= POLLIN;
	poll_fds.push_back(serv_poll);
	
	while (server_shutdown == FALSE)
	{
		// launch poll and check fails eand timing
		rc = poll((pollfd*)&poll_fds[0], (unsigned int)poll_fds.size(), timeout);
		checkPoll(rc);
		// check revents of poll 
		for (unsigned int i = 0; i < poll_fds.size() ; i++)
		{
			if (poll_fds[i].revents == 0)
        		continue;
			if (poll_fds[i].revents != POLLIN)
			{
				std::cout << "[Server] Error! poll revents= " << poll_fds[i].revents << std::endl;
				server_shutdown = TRUE;
				break;
			}
			// check if server socket is "readable" and loop to accept connection
			if (poll_fds[i].fd == _socServ) // if it's the listening socket (server's)
			{
				std::cout << "[Server] Socket is readable\n";
				while (new_poll.fd != -1)
				{
					new_poll.fd = acceptConnection();
					new_poll.events = POLLIN;
					std::cout << "[Server] New incoming connection - " << new_poll.fd << std::endl;
          			poll_fds.push_back(new_poll);
					/* add a function that fill the "_client" variable with all the client's infos*/
					/* send automatic reply RPL_WELCOME */		
				}
			}
			else // if it's a client already connected to server
			{
				std::cout << "[Server] FD " << poll_fds[i].fd << "is readable\n";
				while (TRUE)
				{
					// receive and send msg
					rc = recv(poll_fds[i].fd, _buf, sizeof(_buf), 0);
					checkReception(rc);
					int len = rc;
					std::cout << "[Server] " << len << "bytes received\n";
					rc = send(poll_fds[i].fd, _buf, len, 0);
					if (rc == ERROR)
					{
						std::cerr << "[Server] coudn't send message";
						exit(ERROR);
					}
				}
				if (close_conn) // if a Client is closing its connection
				{
					close(poll_fds[i].fd);
					poll_fds[i].fd = -1;
					compress_fds = TRUE;
				}
			}
		}
	}
	if (compress_fds == TRUE)
	{
		compress_fds = FALSE;
		delClient(poll_fds);
	}
}

void		Server::delClient(std::vector <pollfd> fds)
{
	unsigned int len = fds.size();
    for (unsigned int i = 0; i < len; i++)
    {
		if (fds[i].fd == -1)
		{
			for(unsigned int j = i; j < len; j++)
				fds[j].fd = fds[j+1].fd;
			i--;
			len--;
		}
	}
}

//void		Server::delChannel(std::string chan)
//{
//	std::vector<servOp>::iterator it = _ops.begin();
	
//    for (; it != _ops.end(); ++it)
//    {
//		if (it.name == chan)
//			_ops.erase(it);
//	  }
//}

void 	Server::checkPoll(int rc)
{
	if (rc == ERROR && server_shutdown == TRUE)
		exit(ERROR);
	if (rc == ERROR)
	{
		std::cerr << "[Server] Poll failed" << std::endl;
		exit(ERROR);
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Poll timed out." << std::endl;
		server_shutdown == TRUE;
		exit(ERROR);
	}
}

void	Server::checkReception(int rc)
{
	if (rc < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cerr << "[Server] Reception failed.";
			exit(ERROR);
		}
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Connection closed." << std::endl;
		exit(ERROR);
	}
}