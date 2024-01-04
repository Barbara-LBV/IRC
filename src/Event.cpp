/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:30:05 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/04 16:26:10 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Event.hpp"

Event::Event(){};

~Event::Event(){};

void   Event::setEventStruct(int id, short filt, u_short flags, u_int fflags, int data, void*   udata)
{
	
}

void          Event::initialize(int socket_fd)
{
	
}

void          Event::remove(int fd)
{
	
}

void          Event::add(Client*)
{
	
}

int           Event::kevent()
{
	
}