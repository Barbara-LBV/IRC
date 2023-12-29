/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/29 15:51:02 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/server.hpp"

Server::Server()
{
	_socServ.len = sizeof(_socServ.addr);
    _socServ.fd  = -1;
	_servPwd = "";
	_servInput = "";
	_result = 0;
    _remain = 0;
}

//Server::Server(Server const &s)
//{}

//Server &Server::operator=(Server const &s)
//{}

Server::~Server()
{
	//std::map<int, Client *>::iterator it = _clients.begin();
	//for ( ;it != _clients.end(); ++it)
	//	_clients.erase(it);
	//_clients.clear();
}

void	Server::createServerSocket(void)
{
	_socServ.fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_add, 0, sizeof(_add));
	if (_socServ.fd == SOCKET_ERROR)
		throw CantCreateSocket();
	int toggle = 1;
    setsockopt(_socServ.fd, SOL_SOCKET, SO_REUSEPORT, (const void*)&toggle, sizeof(toggle));
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

void	Server::listenForConnetion(void)
{
	if (listen(_socServ.fd, BACKLOG) ==  SOCKET_ERROR)
		throw CantListen();
	std::cout << "Listen on socket fd: " << _socServ.fd << std::endl;
}

int		Server::acceptConnection(void)
{
	_fd = accept(_socServ.fd, (struct sockaddr *)&_add, &_socServ.len);
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
    listenForConnetion();
}

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
