/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:06:37 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/14 16:58:51 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

/*
      Command: WHO
   Parameters: [ <mask> [ "o" ] ]

   The WHO command is used by a client to generate a query which returns
   a list of information which 'matches' the <mask> parameter given by
   the client.  In the absence of the <mask> parameter, all visible
   (users who aren't invisible (user mode +i) and who don't have a
   common channel with the requesting client) are listed.  The same
   result can be achieved by using a <mask> of "0" or any wildcard which
   will end up matching every visible user.

   The <mask> passed to WHO is matched against users' host, server, real
   name and nickname if the channel <mask> cannot be found.
   
    If the "o" parameter is passed only operators are returned according
   to the <mask> supplied.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_WHOREPLY                  RPL_ENDOFWHO

   Examples:

   WHO *.fi                        ; Command to list all users who match
                                   against "*.fi".

   WHO jto* o                      ; Command to list all users with a
                                   match against "jto*" if they are an
                                   operator.								   
*/

//WhoCommand::WhoCommand(Server *server) : Command(server) {}

//WhoCommand::~WhoCommand() {}

//void WhoCommand::execute(Client *client, std::vector<std::string> arguments)
//{
//	std::string activeChannels(""), opOnChannels("");
	
//	if (arguments.size() == 0)
//		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "WHOIS"));
//	if (arguments[0] == client->getUsername())
//	{
//		addToClientBufferExtended(client->getServer(), client->getFd(), RPL_WHOISUSER(client->getNickname(), client->getUsername(), client->getHost(), activeChannels, client->getServer()->getServerName()));
		
//		if (client->getChannels().size() != 0)
//		{
//			activeChannels = retrieveChannelNames(client);
//			addToClientBuffer(client->getServer(), client->getFd(), RPL_WHOISCHANNELS(client->getNickname(), activeChannels));
//			if (client)
//			{
//				opOnChannels = retrieveChannelOp(client);
//				addToClientBuffer(client->getServer(), client->getFd(), RPL_WHOISOPERATOR(client->getNickname(), opOnChannels));
//			}
//		}
//	}
//	else
//		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getPrefix(), client->getNickname()));
//	return;
//}

//std::string		retrieveChannelNames(Client *client)
//{
//	std::string allChans("");
//	std::deque<Channel *> channels = client->getChannels();
//	std::deque<Channel *>::iterator it = channels.begin();
//	for (; it != channels.end(); it++)
//		allChans += (*it)->getName() + " ";
//	return allChans;
//}

//std::string		retrieveChannelOp(Client *client)
//{
//	std::string chanOp("");
//	std::deque<Channel *> channels = client->getChannels();
//	std::deque<Channel *> ::iterator it = channels.begin();
//	std::vector<Client *> ops;
	
//	for (; it != channels.end(); it++)
//	{
//		ops = (*it)->getOperator();
//		for (std::vector<Client *>::iterator ite = ops.begin(); ite!= ops.end(); ite++)
//		{
//			if(*ite == client)
//			{
//				chanOp += (*it)->getName() + " ";
//				break ;
//			}
//		}
//	}
//	return chanOp;
//}