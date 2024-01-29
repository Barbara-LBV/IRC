/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 11:08:47 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/24 16:02:40 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/CmdHandler.hpp"

CmdHandler::CmdHandler(Server *server)
{
	_server = server;
	_commands["PASS"] = new PassCommand(_server, false);
	_commands["NICK"] = new NickCommand(_server, false);
	_commands["USER"] = new UserCommand(_server, false);
	_commands["QUIT"] = new QuitCommand(_server, false);
	_commands["PING"] = new PingCommand(_server);
	_commands["PONG"] = new PongCommand(_server);
	_commands["JOIN"] = new JoinCommand(_server);
	_commands["MODE"] = new ModeCommand(_server);
	_commands["PART"] = new PartCommand(_server);
	_commands["KICK"] = new KickCommand(_server);
	_commands["INVITE"] = new InvitCommand(_server);
	_commands["PRIVMSG"] = new PrivMsgCommand(_server);
	_commands["NOTICE"] = new NoticeCommand(_server);
	_commands["WHO"] = new WhoCommand(_server);
	_commands["LIST"] = new ListCommand(_server);
}

CmdHandler::~CmdHandler()
{
	std::map<std::string, Command *>::iterator it = _commands.begin();
	for (; it != _commands.end(); it++)
		delete it->second;
}

void 	CmdHandler::invoke(Server *serv, Client *client, std::string const &msg)
{
	std::stringstream	parse(msg);
	std::string 		line, name;
	std::vector<std::string> args;

	while (getline(parse, line))
	{
		line = line.substr(0, line[line.length() - 1] == '\r' ? line.length() - 1 : line.length());
		name = line.substr(0, line.find(' '));
		if (name == "CAP")
			continue;
		//std::cout << "In invoke function, in while loop \n";
		try
		{
			Command *command = _commands.at(name);
			//std::cout << "name command = " << name << std::endl;
			std::string buf;
			std::stringstream ss(line.substr(name.length(), line.length()));
			
			while (ss >> buf)
				args.push_back(buf);
			if (command->getAuthRequired() && client->isRegistred() == FALSE)
			{
				addToClientBuffer(serv, client->getFd(), ERR_NOTREGISTERED(client->getNickname()));
				return;
			}
			command->execute(client, args);
		}
		catch (const std::out_of_range &e)
		{
			if (name != "CAP")
				addToClientBuffer(serv, client->getFd(), ERR_UNKNOWNCOMMAND(client->getNickname(), name));
		}
	}
}