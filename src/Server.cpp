/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/04 16:28:16 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd)
{
	_socServ.len = sizeof(_socServ.addr);
    _socServ.fd  = -1;
	_servPwd = pwd;
	_servInput = "";
	_servPort = atoi(port.c_str());
	_result = 0;
    _remain = 0;
}

//Server::Server(Server const &s)
//{}

//Server &Server::operator=(Server const &s)
//{}

Server::~Server()
{
	for (int i = 0; i < BACKLOG; i++)
  	{
   		if (_fds[i].fd >= 0)
    		close(_fds[i].fd);
	}
}

void	Server::createServerSocket(void)
{
	int rc, on = 1;
	_socServ.fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_add, 0, sizeof(_add));
	if (_socServ.fd == SOCKET_ERROR)
		throw CantCreateSocket();
	std::cout << "Server's socket successfully created\n";
    rc = setsockopt(_socServ.fd, SOL_SOCKET, SO_REUSEPORT, (const void*)&on, sizeof(on));
	if (rc < 0)
		throw CantConfigSocket();
	
}

void	Server::bindServerSocket(int port)
{
	memset(&_socServ.addr, 0, sizeof(sockaddr_in));
	_socServ.addr.sin_family = AF_INET;
	_socServ.addr.sin_addr.s_addr = INADDR_ANY;
	_socServ.addr.sin_port = htons(port);
	if (bind(_socServ.fd, (struct sockaddr *)&_socServ.addr, sizeof(_socServ.addr)) == SOCKET_ERROR)
		throw CantBind();
	std::cout << "Bind succeeded on port: " << port << ", IP="
        << inet_ntoa(_socServ.addr.sin_addr) << std::endl;		
}

void	Server::listenForConnection(void)
{
	if (listen(_socServ.fd, BACKLOG) ==  SOCKET_ERROR)
		throw CantListen();
	std::cout << "Listen on socket fd: " << _socServ.fd << std::endl;
	memset(_fds, 0, sizeof(_fds));
	_fds[0].fd = _socServ.fd;
	_fds[0].events = POLLIN;
}

int		Server::acceptConnection(void)
{
	_addLen = sizeof (_add);
	_fd = accept(_socServ.fd, (struct sockaddr *)&_add, &_addLen);
	if (_fd ==  SOCKET_ERROR)
		throw CantAccept();
	else
        fcntl(_fd, F_SETFL, O_NONBLOCK);
	return (_fd);
}

void Server::initializeServer(int port)
{
    createServerSocket();
    bindServerSocket(port);
    listenForConnection();
	manageConnections();
}

struct sockaddr_in &Server::getServAdd(void)
{
	return (_socServ.addr);
}

socklen_t 		&Server::getServAddLen(void)
{
	return (_socServ.len);
}

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
				std::cout << "Error! poll revents= " << _fds[i].revents << std::endl;
				end_server = TRUE;
				break;
			}
			// 3- verifier si la socket du serv est "readable" et boucler pour accepter les connections
			if (_fds[i].fd == _socServ.fd) //s'il s'agit de la socket d'ecoute ..
			{
				std::cout << "Server socket is readable\n";
				while (new_fd != -1)
				{
					new_fd = acceptConnection();
					std::cout << "New incoming connection - " << new_fd << std::endl;
          			_fds[nfds].fd = new_fd;
					_fds[nfds].events = POLLIN;
					nfds++;
				}
			}
			else // s'il s'agit de connexions clients
			{
				std::cout << "FD " << _fds[i].fd << "is readable\n";
				while (TRUE)
				{
					// 4- fonction pour recevoir et envoyer des msg
					rc = recv(_fds[i].fd, _buf, sizeof(_buf), 0);
					checkReception(rc);
					int len = rc;
					std::cout << "SERVER> " << len << "bytes received\n";
					rc = send(_fds[i].fd, _buf, len, 0);
					if (rc < 0)
					throw CantSendMessage();
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
		nfds = reduceFds(nfds);
	}
}

int	Server::reduceFds(int nfds)
{
    for (int i = 0; i < nfds; i++)
    {
		if (_fds[i].fd == -1)
		{
			for(int j = i; j < nfds; j++)
				_fds[j].fd = _fds[j+1].fd;
			i--;
			nfds--;
		}
	}
	return nfds;
}

void 	Server::checkPoll(int rc)
{
	if (rc < 0)
	{
		std::cout << "Poll failed" << std::endl;
		throw PollIssue();
	}
	if (rc == 0)
	{
		std::cout << "Poll timed out. End of Program" << std::endl;
		throw PollIssue();
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

//void	Server::receiveMsg(void)
//{
	
//}

//void 	Server::sendMsg(void)
//{
	
//}
//void	Server::acceptConnexions(void)
//{
//	int	fdCli;
	
//	while (1)
//	{
//		if (_nbCli <= BACKLOG)
//		{
//			_nbCli++;
//			Client *cli = new Client();
//			//cli->connectToServer(_socServ);
//			fdCli = accept(_socServ, (struct sockaddr *)&cli->_addCli, &cli->getCliSize());
//			if (_clients.find(fdCli) == _clients.end())
//			{
//				_clients.insert(std::pair<int, Client *>(fdCli, cli));
//				//int connRes = connect(cli->getCliSocket(), (sockaddr*)&_addServ, sizeof(_addServ));
//				//if (connRes == SOCKET_ERROR)
//				//	throw Client::FailConnexion();
//				//std::cout << "Client n." << _nbCli << " is connecting..." << std::endl;
//			}
//		}
//		getline(std::cin, _servInput);
//		int sendRes = send(_socServ, _servInput.c_str(), _servInput.size() + 1, 0);
//		if (sendRes == SOCKET_ERROR)
//			throw CantSendMessage();
//		int bytesReceived = recv(_socServ, _buf, MAXBUF, 0);
//		if (bytesReceived == SOCKET_ERROR)
//			throw CantReceiveMessage();
//		std::cout << "Server> " << std::string(_buf, bytesReceived) << std::endl;
//		//write
//		//send
//		//wait
//		//recv
//		//display
//		//read
//		//close socket 		
//	}
//	close(_socServ);
//}
