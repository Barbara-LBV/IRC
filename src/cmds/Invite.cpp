/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:55:34 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/23 18:18:29 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"
#include "../../lib/Command.hpp"

InvitCommand::InvitCommand(Server *server) : Command(server) {}

InvitCommand::~InvitCommand() {}

void InvitCommand::execute(Client *client, std::vector<std::string> arguments)
{
	(void)client;
	(void)arguments;
	std::cout << " Execute Invite Command\n";
}