/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:03:37 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/24 17:21:32 by pmaimait         ###   ########.fr       */
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
		const std::string 			getName(void) const {return _name;};
		const std::string 			&getTopic(void) const;
		std::vector<Client*>        getOperator(void){return _ops;};
		size_t							getL(){return _l;};
		bool						getI(){return _i;};
		bool						getT(){return _t;};
		
		std::vector<std::string>	getNickNames();
		std::vector<Client*>  		getClients(){return _clients;};
		Client*						getClient(const std::string &nickname);
		
		void 				setTopic(const std::string &topic);
		void				setL(size_t N){_l = (N != 0) ? N : 1; };
		void				setI(int b){_i = b;};
		void				setT(int b){_t = b;};
		
		/***********   Functions   ***********/
		void 				joinChannel(Client *cli){_clients.push_back(cli);};
   		void 				partChannel(Client *cli);
		bool    			is_oper(Client *client);
		void 				removeOpe(Client *client);
		bool				isInChannel(Client *client);
		void				addOperator(Client *client){_ops.push_back(client);};
		
		
	private:
		Channel(Channel const &name);
		Channel &operator=(Channel const &c);
		
		const std::string	_name;
		std::string	_topic;
		std::string	_password;
		Server*			_server;
		std::vector<Client *>_clients;
		std::vector<Client *>_ops;
		size_t		_l;		//Set/remove the user limit to channel
		bool	_i = false;     //Set/remove Invite-only channel, true is invite only
		bool	_t = true;		//Set/remove the restrictions of the TOPIC command to channel operator
		                        //true is only operator can set topic
		
};

#endif