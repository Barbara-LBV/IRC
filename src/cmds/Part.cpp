/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:46 by pmaimait          #+#    #+#             */
/*   Updated: 2024/01/23 18:18:15 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

PartCommand::PartCommand(Server *server) : Command(server) {}

PartCommand::~PartCommand() {}

void PartCommand::execute(Client *client, std::vector<std::string> arguments)
{
	(void)client;
	(void)arguments;
	std::cout << " Execute Part Command\n";
}