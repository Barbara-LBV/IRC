/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:03:37 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/23 18:52:49 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "IrcLib.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Channel
{
	public:
		Channel(std::string const &name, std::string const &password, Client* ops, Server *server);
		~Channel();

		/***********   Assessors   ***********/
		const std::string 			getName(void) const;
		const std::string 			&getTopic(void) const;
		bool                		getStatus(int);
		std::vector<Client*>       getOperator(void){return _ops;};
		std::string         		getStatus(void);
		std::vector<std::string>	getNickNames();
		std::vector<Client*>  		getClients(){return _clients;};
		Client*						getClient(const std::string &nickname);
		
		void 				setName(const std::string &name);
		void 				setTopic(const std::string &topic);
		void 				setStatus(std::string &status);
		
		/***********   Functions   ***********/
		void 				joinChannel(Client *cli);
   		void 				partChannel(Client *cli);
    	void 				invitationChannel(Client *cli);
		bool    			is_oper(Client *client);

	private:
		Channel(Channel const &name);
		Channel &operator=(Channel const &c);
		
		const std::string	_name;
		const std::string	_topic;
		const std::string	_password;
		Server*			_server;
		std::vector<Client *>_clients;
		std::vector<Client *>_ops;
		int		_l;		//Set/remove the user limit to channel
		bool	_i;     //Set/remove Invite-only channel
		bool	_t;		//Set/remove the restrictions of the TOPIC command to channel operator

		//bool		_privChannel; // true: channel with pwd / false: channel without pwd
		//int			_status; // open/close status ?? 1
};

#endif