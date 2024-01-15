/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 16:33:18 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/15 18:05:39 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

bool checkArg(std::string port, std::string pwd)
{
	if (pwd.size() > 10)
	{
		std::cerr << pwd << ": chosen password is too long.\n";
		return false;
	}
	for(size_t i = 0; i < port.size(); i++)
	{
		if (!isdigit(port[i]))
		{
			std::cerr << port << ": unvalid port number\n";
			return false;
		}
	}
	if (port.size() > 5)
	{
		std::cerr << port << ": out of port range (0 ~ 65535)\n";
		return false;
	}
	int portNb = atoi(port.c_str());
	if (portNb > 65535 || portNb < 0)
	{
		std::cerr << port << ": out of port range (0 ~ 65535)\n";
		return false;
	}
	return true;
}



