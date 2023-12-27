/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/27 18:10:32 by blefebvr         ###   ########.fr       */
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

void	Client::connectToServer(int servSocket)
{
	_cliSize = sizeof(_addCli);
	int connRes = connect(servSocket, (struct sockaddr *)&_addCli, _cliSize);
	if (connRes == SOCKET_ERROR)
		throw FailConnexion();
}

int	const	&Client::getCliSocket(void)
{
	return _socCli;
}

struct sockaddr &Client::getCliAdd(void)
{
	return _addCli;
}