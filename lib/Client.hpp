/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:39 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/05 17:22:37 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "IrcLib.hpp"
#include "Channel.hpp"

class Server;
class Channel; 

class Client
{
	public:
		Client();
		~Client();
		
		/* structures */
		typedef struct s_status //client status, usefull also for channel management
		{
			/* what's client status : registred, joined(channel) etc*/
			unsigned char pass : 1;
			
		} t_status;
		
		typedef struct s_names // all the infos allowing to identify a client
		{
			std::string	nickname; // pseudo: usefull for channel operator
			std::string	oldNick;
			std::string	username; 
			int			cliFd; // IP adress: usefull for channel operator
			int			user_id; 
			std::string	host;
		} t_names;
		
		/* Assessors */
		
		/* connections management */
		
		/* Channel management => which "mode" : chanOp, simple user, disconnected ?*/
		
		void  			connectToServer(Server &s);
		void			sendMsg();
		void			recvMsg();
		void			closeCliFd();
		int	const		&getCliSocket(void);

	private:
		Client(Client const &s);
		Client &operator=(Client const &s);
		
		std::string			_sendTo;
		std::string			_recvFrom;
		char				_buf[1096]; //each client has its own buffer. Must check that the whole msg has been received (ending with /0)
		
};

#endif


