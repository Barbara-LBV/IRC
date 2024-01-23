/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 16:33:18 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/23 14:47:23 by blefebvr         ###   ########.fr       */
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

std::vector<std::string>	splitMsg(std::string msg, char c)
{
	std::vector<std::string> cmd;
	std::string				 line;
	size_t pos(0);

	while (pos < std::string::npos)
	{
		pos = msg.find(c);
		line = msg.substr(0, pos);
		cmd.push_back(line);
		msg.erase(0, pos);
		line.clear();
	}
	return cmd;
}

//void	Server::parseCmd(Client *cli, std::vector <std::string> cmds)
//{
	
//}

