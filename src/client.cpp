/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/22 16:23:32 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/client.hpp"

Client::Client()
{
	
}

Client::Client(Client const &c)
{
	*this = c;
}

Client &Client::operator=(Client const &c)
{
	if (this != &c)
	{
		
	}
	return *this;
}

Client::~Client()
{
	std::vector<int>::iterator it = _fdsCli.begin();
	while (it != _fdsCli.end())
	{
		_fdsCli.erase(it);
		it++;
	}
	_fdsCli.clear();
}