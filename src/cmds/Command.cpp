/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:48:39 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/22 16:52:01 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"

Command::Command(Server *server)
{
	_server = server;
	_commands["PASS"] = new PassCommand(_server);
	_commands["NICK"] = new NickCommand(_server);
	_commands["USER"] = new UserCommand(_server);
	_commands["QUIT"] = new QuitCommand(_server);
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

Command::~Command()
{
	for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
}