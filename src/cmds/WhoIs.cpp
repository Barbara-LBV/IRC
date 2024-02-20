/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoIs.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:06:37 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/19 13:37:39 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

/*
      Command: WHOIS
   Parameters: [ <target> ] <mask> *( "," <mask> )

   This command is used to query information about particular user.
   The server will answer this command with several numeric messages
   indicating different statuses of each user which matches the mask (if
   you are entitled to see them).  If no wildcard is present in the
   <mask>, any information about that nick which you are allowed to see
   is presented.

   If the <target> parameter is specified, it sends the query to a
   specific server.  It is useful if you want to know how long the user
   in question has been idle as only local server (i.e., the server the
   user is directly connected to) knows that information, while
   everything else is globally known.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

           ERR_NOSUCHSERVER              ERR_NONICKNAMEGIVEN
           RPL_WHOISUSER                 ERR_NOSUCHNICK
           RPL_WHOISCHANNELS             RPL_WHOISSERVER
           RPL_AWAY                      RPL_WHOISOPERATOR
           RPL_WHOISIDLE                 
           RPL_ENDOFWHOIS
	
	  Examples:

   WHOIS wiz                       ; return available user information
                                   about nick WiZ

   WHOIS eff.org trillian          ; ask server eff.org for user
                                   information  about trillian
								   
*/

WhoIsCommand::WhoIsCommand(Server *server) : Command(server) {}

WhoIsCommand::~WhoIsCommand() {}

void WhoIsCommand::execute(Client *client, std::vector<std::string> arguments)
{
	std::string activeChannels(""), opOnChannels("");
	if (arguments.size() == 0)
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "WHOIS"));
	std::string name = arguments[0];
	if (name == client->getUsername() || name == client->getNickname())
	{
		activeChannels = retrieveChannelNames(client);
		if (activeChannels.size() == 0)
			activeChannels = "0";
		addToClientBufferExtended(client->getServer(), client->getFd(), RPL_WHOISUSER(client->getPrefix(), client->getUsername(), client->getHost(), activeChannels, client->getServer()->getServerName()));
		if (client->getChannels().size() > 0)
		{
			addToClientBuffer(client->getServer(), client->getFd(), RPL_WHOISCHANNELS(client->getNickname(), activeChannels));
			opOnChannels = retrieveChannelOp(client);
			if (!opOnChannels.empty())
				addToClientBufferExtended(client->getServer(), client->getFd(), RPL_WHOISOPERATOR(client->getNickname(), opOnChannels));
		}
	}
	else if(name == client->getServer()->getClientByNickname(name)->getNickname())
	{
		Client *cli = client->getServer()->getClientByNickname(name);
		activeChannels = retrieveChannelNames(cli);
		addToClientBufferExtended(cli->getServer(), cli->getFd(), RPL_WHOISUSER(cli->getPrefix(), cli->getUsername(), cli->getHost(), activeChannels, cli->getServer()->getServerName()));
		if (cli->getChannels().size() > 0)
		{
			addToClientBuffer(cli->getServer(), cli->getFd(), RPL_WHOISCHANNELS(cli->getNickname(), activeChannels));
			opOnChannels = retrieveChannelOp(cli);
			if (!opOnChannels.empty())
				addToClientBufferExtended(cli->getServer(), cli->getFd(), RPL_WHOISOPERATOR(cli->getNickname(), opOnChannels));
		}
	}
	else
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), client->getNickname()));
	return;
}

std::string		retrieveChannelNames(Client *client)
{
	std::string allChans("");
	std::deque<Channel *> channels = client->getChannels();
	std::deque<Channel *>::iterator it = channels.begin();
	for (; it != channels.end(); it++)
		allChans += (*it)->getName() + " ";
	return allChans;
}

std::string		retrieveChannelOp(Client *client)
{
	std::string chanOp("");
	std::deque<Channel *> channels = client->getChannels();
	std::deque<Channel *> ::iterator it = channels.begin();
	std::vector<Client *> ops;
	
	for (; it != channels.end(); it++)
	{
		ops = (*it)->getOperator();
		for (std::vector<Client *>::iterator ite = ops.begin(); ite!= ops.end(); ite++)
		{
			if(*ite == client)
			{
				chanOp += "@" + (*it)->getName() + " ";
				break ;
			}
		}
	}
	return chanOp;
}
