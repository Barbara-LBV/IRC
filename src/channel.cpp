/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:06:20 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/02 12:07:01 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/channel.hpp"

Channel::Channel()
{

}

Channel::Channel(Channel const &c)
{
	*this = c;
}

Channel &Channel::operator=(Channel const &c)
{
	if (this != &c)
	{

	}
	return *this;
}

Channel::~Channel()
{
}