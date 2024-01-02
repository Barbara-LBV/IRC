/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/02 18:01:59 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/client.hpp"
#include <cerrno>

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

void	Client::connectToServer(Server &s)
{
	int fd_cli = s.acceptConnection();
	//it's the client socket in the 1st param, and the server addr in 2nd et 2rd params
	if (connect(fd_cli, (struct sockaddr *)&s.getServAdd(), s.getServAddLen()) == SOCKET_ERROR)
		throw FailConnexion();
	std::cout << "Connecting ..." << std::endl;
}

int	const	&Client::getCliSocket(void)
{
	return _socCli;
}

socklen_t &Client::getCliSize(void)
{
	return _cliSize;
}