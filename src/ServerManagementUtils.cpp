/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/11 17:09:00 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

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
			std::cerr << "[Server] Reception/Sending failed.";
			exit(ERROR);
		}
	}
	if (rc == 0)
	{
		std::cerr << "[Server] Connection closed." << std::endl;
		exit(ERROR);
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

void		Server::delChannel(std::string chan)
{
	std::vector<servOp>::iterator it = _ops.begin();
	
    for (; it != _ops.end(); ++it)
    {
		if (it->name == chan)
			_ops.erase(it);
	  }
}

