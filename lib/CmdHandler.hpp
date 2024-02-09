/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 11:05:56 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/09 15:44:26 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDHANDLER_HPP
# define CMDHANDLER_HPP

# include "IrcLib.hpp"
# include "Server.hpp"
# include "Command.hpp"
# include "Client.hpp"

class Server;
class Command;
class Client;

class CmdHandler
{
	private:
		//Server 							*_server;
		std::map<std::string, Command *> _commands;
		
	public:
		CmdHandler();
		~CmdHandler();

		void 			invoke(Server *serv, Client *client, std::string const &message);
		std::string 	parseCmd(std::string cmd);
};

#endif