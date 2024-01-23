/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:32:59 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/23 18:18:02 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

QuitCommand::QuitCommand(Server *server) : Command(server) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client *client, std::vector<std::string> arguments)
{
	(void)client;
	(void)arguments;
	std::cout << " Execute Quit Command\n";
}