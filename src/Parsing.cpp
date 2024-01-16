/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 16:33:18 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/16 11:30:29 by blefebvr         ###   ########.fr       */
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

void	Server::stockMsg(Client *cli, char *buf)
{
	std::string partialTmp = cli->getPartialMsg();
	
	if (partialTmp.empty())
		cli->setPartialMsg(buf);
	else
	{
		partialTmp.insert(partialTmp.size(), buf);
		cli->setPartialMsg(partialTmp);
	}
}

/*void	Server::splitMsg(Client *cli, std::string msg)
{
	std::string tmp;
	size_t	pos(0);
	
	while (pos < std::string::npos)
	{
		if (msg.find("PASS", pos) < std::string::npos)
		{
			//inserer PASS et sa suite dans une string tmp, puis setter le mdp
			cli->setPwd(tmp);
			pos += msg.find("PASS", pos);
			tmp.clear();
		}
		if (msg.find("NICK", pos) < std::string::npos)
		{
        	cli->setNickname();
			pos += msg.find("NICK", pos);
		}
		if (msg.find("USER", pos) < std::string::npos)
		{
			cli->setUsername();
			pos += msg.find("USER", pos);
		}
		else
			break ;
	}
}*/

//std::string	Server::parseMsg()
//{
	//return msg;
//}

