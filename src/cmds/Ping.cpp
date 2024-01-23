/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:04:25 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/23 18:18:11 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

PingCommand::PingCommand(Server *server) : Command(server) {}

PingCommand::~PingCommand() {}

void PingCommand::execute(Client *client, std::vector<std::string> arguments)
{
	(void)client;
	(void)arguments;
	std::cout << " Execute Ping Command\n";
}