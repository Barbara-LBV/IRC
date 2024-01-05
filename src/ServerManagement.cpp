/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:58:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/05 17:04:50 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void 	Server::manageConnections(void)
{
	int end_server = FALSE, compress_fds = FALSE, close_conn = FALSE;
	int current_size, i, rc, new_fd = 0;
	int	timeout = 3 * 60 * 100;
	int nfds = 1;
	
	while (end_server == FALSE)
	{
		// 1- lancer poll et faire les checks de fails et de temps => 1 fonction
		rc = poll(_fds, nfds, timeout);
		checkPoll(rc);
		current_size = nfds;
		// 2- verifie les revents de poll => 1 fonction
		for (i = 0; i < current_size ; i++)
		{
			if(_fds[i].revents == 0)
        		continue;
			if(_fds[i].revents != POLLIN)
			{
				std::cout << "[Server] Error! poll revents= " << _fds[i].revents << std::endl;
				end_server = TRUE;
				break;
			}
			// 3- verifier si la socket du serv est "readable" et boucler pour accepter les connections
			if (_fds[i].fd == _socServ) //s'il s'agit de la socket d'ecoute ..
			{
				std::cout << "[Server] Socket is readable\n";
				while (new_fd != -1)
				{
					new_fd = acceptConnection();
					std::cout << "[Server] New incoming connection - " << new_fd << std::endl;
          			_fds[nfds].fd = new_fd;
					_fds[nfds].events = POLLIN;
					nfds++;
				}
			}
			else // s'il s'agit de connexions clients
			{
				std::cout << "[Server] FD " << _fds[i].fd << "is readable\n";
				while (TRUE)
				{
					// 4- fonction pour recevoir et envoyer des msg
					rc = recv(_fds[i].fd, _buf, sizeof(_buf), 0);
					checkReception(rc);
					int len = rc;
					std::cout << "[Server] " << len << "bytes received\n";
					rc = send(_fds[i].fd, _buf, len, 0);
					if (rc == ERROR)
					{
						std::cerr << "";
						exit(ERROR);
					}
				}
				if (close_conn)
				{
					close(_fds[i].fd);
					_fds[i].fd = -1;
					compress_fds = TRUE;
				}
			}
		}
	}
	if (compress_fds == TRUE)
	{
		compress_fds = FALSE;
		nfds = delClient(nfds);
	}
}

int		Server::delClient(int cliNb)
{
    for (int i = 0; i < cliNb; i++)
    {
		if (_fds[i].fd == -1)
		{
			for(int j = i; j < cliNb; j++)
				_fds[j].fd = _fds[j+1].fd;
			i--;
			cliNb--;
		}
	}
}

void		Server::delChannel(std::string chan)
{
	std::vector<servOp>::iterator it = _ops.begin();
	
    for (; it != _ops.end(); ++it)
    {
		if (it == chan)
		{
			for(int j = i; j < cliNb; j++)
				_fds[j].fd = _fds[j+1].fd;
			i--;
			cliNb--;
		}
	}
}

void 	Server::checkPoll(int rc)
{
	if (rc == ERROR)
	{
		std::cerr << "[Server] Poll failed" << std::endl;
		exit(ERROR);
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Poll timed out. End of Program" << std::endl;
		exit(ERROR);
	}
}

void	Server::checkReception(int rc)
{
	if (rc < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cout << "Reception failed. ";
			throw CantReceiveMessage();
		}
	}
	if (rc == 0)
	{
		std::cout << "Connection closed" << std::endl;
		throw PollIssue();
	}
}