/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/26 17:35:52 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/client.hpp"

Client::Client()
{
	_servOutput = "";
	_socCli = 0;
	_cliSize = 0;
}

Client::Client(Client const &c)
{
	*this = c;
}

Client &Client::operator=(Client const &c)
{
	if (this != &c)
	{
		_servOutput = c._servOutput;
		for (int i = 0; i < MAXBUF; i++)
			_buf[i] = c._buf[i];
	}
	return *this;
}

Client::~Client()
{
}

void	Client::createClientSocket(int socServ)
{
	_cliSize = sizeof(_addCli);
	_socCli = accept(socServ, (struct sockaddr *)&_addCli, &_cliSize);
	if (_socCli == SOCKET_ERROR)
		throw CantAcceptServer();
	int connRes = connect(c._socClient, (sockaddr*)&_addServ, sizeof(_addServ));
	if (connRes == SOCKET_ERROR)
		throw FailConnexion();
}