/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:39 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/19 16:10:07 by blefebvr         ###   ########.fr       */
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
		Client(int fd, Server *server);
		~Client();
		
		/**********    Structures    *********/
		typedef struct s_names // all the infos allowing to identify a client
		{
			int				_cliFd; // client fd
			std::string		_nickname; // pseudo: usefull for channel operator
			std::string		_oldNick; //if it's not the 1st nickname
			std::string		_username; // user = personn who is using IRC Client software
			std::string		_pwd;
			std::string		_host; // IP adress: usefull for channel operator
		} t_names;

		typedef struct s_status // the client status to check what to do
		{
			bool			_connectionPwd;
			bool			_registred;
			bool			_welcomed;
			bool			_toDisconnect;
		} t_status;
		
		/**********    Assessors     *********/
		std::string			getNickname(void)const;
		std::string			&getUsername(void);
		std::string			&getHost(void);
		std::string			&getMsgRecvd(void);
		std::string			&getPartialMsg(void);
		std::string			&getMsgSent(void);
		std::string			getPrefix(void)const;
		bool				&getConnPwd(void);
		bool				&getRegistrationStatus(void);
		bool				&getWelcomeStatus(void);
		bool				&getDeconnStatus(void);
		void				setNickname(std::string);
		void				setUsername(std::string);
		void				setHost(std::string hot);
		void				setPwd(std::string pwd);
		void 				setMsgSent(std::string msg);
		void				setPartialMsg(std::string partialMsg);
		void				welcomeClient(void);
		void 				reply(const std::string &reply);
		void				write(std::string s);
		
		/**********    Connections Management     *********/
		//void			registringClient(std::string s); //split the 1st line received from client to get names and set them if nec
		bool				isRegistred(void);
		void				sendMsgtoServer(std::string msg);
		void				recvMsgfromServer();
		
		/**********    Channel Management    *********/
		
		//which "mode" : chanOp, simple user, disconnected, username a incrementer

	private:
		Client(Client const &s);
		Client &operator=(Client const &s);
		
		std::string		_completeMsg; // each client has its own send buffer.
		std::string		_partialMsg; 
		std::string		_recvdFromServ; // each client has its own recv buffer. Must check that the whole msg has been received (ending with /0) then stock it in _recvFrom.
		t_names			_infos; 
		t_status		_state;
		Server*			_server;
};

#endif


