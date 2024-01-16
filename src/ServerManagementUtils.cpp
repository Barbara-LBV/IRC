/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/16 15:40:59 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

void		Server::delClient(std::vector <pollfd> fds, std::vector <pollfd>::iterator &it)
{
	int cliFd = it->fd;
	_clients.erase(it->fd);
	fds.erase(it);
	close(cliFd);
	_cliNb--;
	std::cout << BLUE << "[Server] Client #" << cliFd
		<< " successfully disconnected. There is now " << _cliNb << " active connections." DEFAULT << std::endl;
}

void		Server::delChannel(std::string topic)
{
	/* quid =>*/
	
	// 1- withdraw chanop privigeles to the admin-client => change its status
	// 2- reduce the server's channel variable _channel
	std::map<std::string, Channel *>::iterator it = _channels.begin();
    while(it != _channels.end())
    {
		if (it->first == topic)
		{
			_channels.erase(it);
			break ;
		}
		++it;
	}
}

void	Server::addClient(std::vector<pollfd> fds, int fd)
{
	Client *cli = new Client(fd);
	pollfd newFd;

	newFd.fd = fd;
	newFd.events = POLLIN | POLLOUT;
	fds.push_back(newFd);
	receiveMsg(cli, fd);
	std::cout << BLUE << _cliMsg << DEFAULT << std::endl;
	//splitMsg(cli, cli->getMsgSent());
	//add here the 1st msg sent by client to get the client info =>split
	/* put here the registration client function ?? */
	_clients.insert(std::pair<int, Client *>(fd, cli));
	std::cout << "[Server] Added client #" << fd 
		<< " successfully" << std::endl;
	_cliMsg.clear();
	_cliNb++;
}

void	Server::cantAddClient(int cliSocket)
{
	std::cout << RED << ERR_FULL_SERV << DEFAULT << std::endl;
	send(cliSocket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, 0);
	close(_servFd); // really ?? do we still can recv/send msg with clients ??
}

//void	Server::sendMsg(void)
//{
	
//}

void	Server::receiveMsg(Client *cli, int fd)
{
	char	buf[MAXBUF];
	
	_result =  recv(fd, buf, MAXBUF, 0);
	setMsg(buf);
	if (_result == ERROR)
	{
		std::cerr << "[Server] Coudn't receive client's message." << std::endl;
		exit(ERROR);
	}
	else if (_result == MAXBUF && !_cliMsg.find("\r\n")) // if the msg sent by client is longer than the MAXBUF
		stockMsg(cli, buf); // to keep the msg until you manage to get the rest of the incoming msg
	else 
		cli->setMsgSent(getMsg());
}

//void or bool 		Server::managePolloutEvent(){}
//void or bool 		Server::managePollerrEvents(){}
