/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:05:32 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/23 18:18:05 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

PrivMsgCommand::PrivMsgCommand(Server *server) : Command(server) {}

PrivMsgCommand::~PrivMsgCommand() {}

void PrivMsgCommand::execute(Client *client, std::vector<std::string> arguments)
{
	(void)client;
	(void)arguments;
	std::cout << " Execute PrivMsg Command\n";
}