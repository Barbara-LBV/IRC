/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:32:59 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/19 13:39:10 by blefebvr         ###   ########.fr       */
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
			reason += " " + arguments[i];
		reason[0] == ':' ? reason : reason.insert(0, 1, ':');
	}
	
	// quitAllChannels(client, reason);
	addToClientBuffer(client->getServer(), client->getFd(), RPL_QUIT(client->getPrefix(), reason));
	client->setDeconnStatus(true);
}

// void 	quitAllChannels(Client *cli, std::string reason)
// {
// 	(void)reason;
// 	std::deque<Channel *> chan = cli->getChannels();
// 	if (chan.empty())
//     {
//         chan.clear();
//         return;
//     }
// 	else
// 	{
// 		std::deque<Channel *>::iterator it = chan.begin();
// 		std::vector<Client *> clients;
// 		std::vector<Client *>::iterator ite;
// 		for (;it != chan.end() ; it++)
// 		{
// 			(*it)->removeClient(cli);
// 			// 			break ;
// 			// clients = (*it)->getClients();
// 			// if (clients.size() > 0)
// 			// {
// 			// 	ite = clients.begin();
// 			// 	for (; ite != clients.end(); ite++)
// 			// 	{
// 			// 		if (*ite == cli)
// 			// 		{
// 			// 			if ((*it)->isOper(cli)) 
//             //    			 	(*it)->removeOpe(cli);
// 			// 			clients.erase(ite);
// 			// 			if (clients.size() == 0)
// 			// 				(*ite)->getServer()->delChannel(*it);
//     		// 			addToClientBuffer(cli->getServer(), cli->getFd(), RPL_QUIT(cli->getPrefix(), reason));	
// 			// 			break ;
// 			// 		}
// 			// 	}
// 			// }
// 		}
// 		addToClientBuffer(cli->getServer(), cli->getFd(), RPL_QUIT(cli->getPrefix(), reason));	
// 	}	
//    	chan.clear();
// }