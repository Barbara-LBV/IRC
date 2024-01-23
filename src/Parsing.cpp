/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 16:33:18 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/22 17:54:36 by blefebvr         ###   ########.fr       */
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

void	Server::parseMsg(std::string msg, int fd)
{
	std::stringstream	parse(msg);
	std::string 		line, buf, name;
	std::vector<std::string> args;

	while (getline(parse, name))
	{
		line = line.substr(0, line[line.length() - 1] == '\r' ? line.length() - 1 : line.length());
		name = line.substr(0, line.find(' '));

		try
		{
			Command *command = _commands.at(name);
			td::string buf;
			std::stringstream ss(syntax.substr(name.length(), syntax.length()));
			
			while (ss >> buf)
				arguments.push_back(buf);
			if (!client->isRegistered())
			{
				addToClientBuffer(this, _clients[fd], ERR_NOTREGISTERED(client->getNickName()));
				return;
			}
			command->execute(client, arguments);
		}
		catch (const std::out_of_range &e)
		{
			if (name != "CAP")
				addToClientBuffer(this, _client[fd], ERR_UNKNOWNCOMMAND(client->getNickName(), name));
		}
	}
}

void	Server::parseCmd(Client *cli, std::vector <std::string> cmds)
{
	
}

