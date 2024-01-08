/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:39 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/08 18:24:54 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "IrcLib.hpp"
//#include "Channel.hpp"

class Server;
//class Channel; 

class Client
{
	public:
		Client();
		~Client();
		
		/* structures */
		typedef struct s_names // all the infos allowing to identify a client
		{
			std::string	nickname; // pseudo: usefull for channel operator
			std::string	oldNick;
			std::string	username; // user = personn who is using IRC Client software
			int			cliFd; // client fd
			int			user_id; 
			std::string	host; // IP adress: usefull for channel operator
		} t_names;

		typedef struct s_status // the client status to check what to do
		{
			bool			_connPwd;
			bool			_registred;
			bool			_welcomed;
			bool			_hasAllInfo;
			bool			_toDisconnect;
		} t_status;
		
		/* Assessors */
		
		/* connections management */
		
		/* Channel management => which "mode" : chanOp, simple user, disconnected ?*/
		
		void			sendMsg();
		void			recvMsg();

	private:
		Client(Client const &s);
		Client &operator=(Client const &s);
		
		std::string		_sendTo; //each client has its own send buffer.
		std::string		_recvFrom;//each client has its own recv buffer. Must check that the whole msg has been received (ending with /0) then stock it in _recvFrom.
		t_names			_client; 
		t_status		_state;
};

#endif

