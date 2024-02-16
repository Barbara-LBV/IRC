/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:32:59 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/16 11:55:42 by pmaimait         ###   ########.fr       */
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
	
	if (arguments.empty())
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "QUIT"));
		return;
	}
	
	if (!arguments.empty() || arguments.size() > 0)
	{
		reason = arguments[0];
		for (size_t i = 1; i < arguments.size(); i++)
			reason = " " + arguments[i];
	}
	
	addToClientBuffer(client->getServer(), client->getFd(), RPL_QUIT(client->getPrefix(), reason));
	client->partAllChannel();
	client->setDeconnStatus(true);
}