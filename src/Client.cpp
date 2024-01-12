/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/12 12:05:49 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Client.hpp"

Client::Client(int fd)
{
	_sendTo = "";
	_recvFrom = "";
   	_infos.cliFd = fd;
	//t_status		_state;
}

Client::~Client()
{
}

void			Client::sendMsg()
{
    
}

void			Client::recvMsg()
{
    
}