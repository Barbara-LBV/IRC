/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/27 18:07:24 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/server.hpp"

Server::Server(int port, std::string pwd)
{
	_socServ = 0;
	_servPwd = pwd;
	_addServ.sin_addr.s_addr = inet_addr("127.0.0.1");
	_addServ.sin_family = AF_INET;
	_addServ.sin_port = htons(port);
	_servInput = "";
	_nbCli = 0;
}

Server::Server(Server const &s)
{
	*this = s;
}

Server &Server::operator=(Server const &s)
{
	if (this != &s)
	{	
		_socServ = s._socServ;
		_addServ = s._addServ;
		_servPwd = s._servPwd;
		_servInput = s._servInput;
		for (int i = 0; i < MAXBUF; i++)
			_buf[i] = s._buf[i];
		std::map<int, Client *>::iterator it = _clients.begin();
		for (; it != _clients.end(); it++)
			_clients.erase(it);
		std::map<int, Client *>::iterator it1 = _clients.begin();
		for (; it1 != s._clients.end(); it1++)
			_clients.insert(std::pair<int, Client*>(it1->first, it1->second));
	}
	return *this;
}

Server::~Server()
{
	std::map<int, Client *>::iterator it = _clients.begin();
	for ( ;it != _clients.end(); ++it)
		_clients.erase(it);
	_clients.clear();
}

void	Server::createServerSocket(void)
{
	_socServ = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_addServ, 0, sizeof(_addServ));
	if (_socServ == SOCKET_ERROR)
		throw CantCreateSocket();
	if (bind(_socServ, (struct sockaddr *)&_addServ, sizeof(_addServ)) == SOCKET_ERROR)
		throw CantBind();
	std::cout << "bind: " << _socServ << std::endl;		
	if (listen(_socServ, BACKLOG) ==  SOCKET_ERROR)
		throw CantListen();
	std::cout << "Listen: Waiting for connexions..." << std::endl;
}

void	Server::acceptConnexions(void)
{
	int	fdCli;
	
	while (1)
	{
		if (_nbCli <= BACKLOG)
		{
			_nbCli++;
			Client *cli = new Client();
			cli->connectToServer(_socServ);
			fdCli = accept(_socServ, (struct sockaddr *)&cli->_addCli, &cli->_cliSize);
			if (_clients.find(fdCli) == _clients.end())
			{
				_clients.insert(std::pair<int, Client *>(fdCli, cli));
				//int connRes = connect(cli->getCliSocket(), (sockaddr*)&_addServ, sizeof(_addServ));
				//if (connRes == SOCKET_ERROR)
				//	throw Client::FailConnexion();
				//std::cout << "Client n." << _nbCli << " is connecting..." << std::endl;
			}
		}
		getline(std::cin, _servInput);
		int sendRes = send(_socServ, _servInput.c_str(), _servInput.size() + 1, 0);
		if (sendRes == SOCKET_ERROR)
			throw CantSendMessage();
		int bytesReceived = recv(_socServ, _buf, MAXBUF, 0);
		if (bytesReceived == SOCKET_ERROR)
			throw CantReceiveMessage();
		std::cout << "Server> " << std::string(_buf, bytesReceived) << std::endl;
		//write
		//send
		//wait
		//recv
		//display
		//read
		//close socket 		
	}
	close(_socServ);
}

int	Client::connectToServer(int servSocket)
{
	_cliSize = sizeof(_addCli);
	int connRes = connect(servSocket, (struct sockaddr *)&_addCli, _cliSize);
	if (connRes == SOCKET_ERROR)
		throw FailConnexion();
	_socCli = accept(servSocket, (struct sockaddr *)&_addCli, &_cliSize);
	return (_socCli);
}

int	const	&Client::getCliSocket(void)
{
	return _socCli;
}