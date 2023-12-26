/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/26 17:33:52 by blefebvr         ###   ########.fr       */
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
	_cliInput = "";
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
		_cliInput = s._cliInput;
		for (int i = 0; i < MAXBUF; i++)
			_buf[i] = s._buf[i];
		std::map<int, Client *>::iterator it = _clients.begin();
		for (; it != _clients.end(); it++)
			_clients.erase(it);
		it = s._clients.begin();
		for (; it != s._clients.end(); it++)
			_clients.insert(std::pair<int, struct Client*>(it->first, it->second));
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
	if (_socServ == SOCKET_ERROR)
		throw CantCreateSocket();
	if (bind(_socServ, (struct sockaddr *)&_addServ, sizeof(_addServ)) == SOCKET_ERROR)
		throw CantBind();
	std::cout << "bind: " << _socServ << std::endl;		
	if (listen(_socServ, BACKLOG) ==  SOCKET_ERROR)
		throw CantListen();
	std::cout << "listen\n";
}

void	Server::acceptConnexions(Client const &c)
{
	while (1)
	{
		getline(std::cin, _cliInput);
		int sendRes = send(_socServ, _cliInput.c_str(), _cliInput.size() + 1, 0);
		if (sendRes == SOCKET_ERROR)
			throw CantSend();
		memset(_buf, 0, MAXBUF);
		int bytesReceived = recv(_socServ, _buf, MAXBUF, 0);
		if (bytesReceived == SOCKET_ERROR)
			throw CantReceive();
		std::cout << "SERVER: " << std::string(_buf, bytesReceived) << std::endl;
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