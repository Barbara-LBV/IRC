/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:32:59 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/02 15:24:30 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

// Command: QUIT
//    Parameters: [ <Quit Message> ]

//    A client session is terminated with a quit message.  The server
//    acknowledges this by sending an ERROR message to the client.

//    Numeric Replies:

//            None.

//    Example:

//    QUIT :Gone to have lunch        ; Preferred message format.

//    :syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch ; User
//                                    syrk has quit IRC to have lunch.

QuitCommand::QuitCommand(Server *server, bool auth) : Command(server, auth){}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client *client, std::vector<std::string> arguments)
{
	std::string  reason = "";
	if (!arguments.empty() || !arguments[0].empty())
	{
		for (size_t i = 0; i < arguments.size(); i++)
			reason = " " + arguments[i];
	}
	client->partAllChannel();
	//_server->delClient(client->getFd());
	addToClientBuffer(client->getServer(), client->getFd(), RPL_QUIT(client->getNickname(), reason));
}