/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 16:33:18 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/19 12:13:29 by pmaimait         ###   ########.fr       */
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
// void CommandHandler::invoke(Client *client, const std::string &message)
// {

// 	std::stringstream ssMessage(message);
// 	std::string syntax;

// 	while (std::getline(ssMessage, syntax))
// 	{

// 		syntax = syntax.substr(0, syntax[syntax.length() - 1] == '\r' ? syntax.length() - 1 : syntax.length());
// 		std::string name = syntax.substr(0, syntax.find(' '));

// 		try
// 		{
// 			Command *command = _commands.at(name);

// 			std::vector<std::string> arguments;

// 			std::string buf;
// 			std::stringstream ss(syntax.substr(name.length(), syntax.length()));

// 			while (ss >> buf)
// 			{
// 				arguments.push_back(buf);
// 			}

// 			if (command->authRequired() && !client->isRegistered())
// 			{
// 				client->reply(ERR_NOTREGISTERED(client->getNickName()));
// 				return;
// 			}

// 			command->execute(client, arguments);
// 		}
// 		catch (const std::out_of_range &e)
// 		{
// 			if (name != "CAP")
// 				client->reply(ERR_UNKNOWNCOMMAND(client->getNickName(), name));
// 		}
// 	}
// }
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

