/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:39 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/02 11:47:20 by blefebvr         ###   ########.fr       */
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
		Client(int fd, Server *server);
		~Client();
		
		/**********    Structures    *********/
		typedef struct s_names // all the infos allowing to identify a client
		{
			int				_cliFd; // client fd
			std::string		_nickname; // pseudo: usefull for channel operator
			std::string		_oldNick; //if it's not the 1st nickname
			std::string		_username; // user = personn who is using IRC Client software
			std::string 	_realname;
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
		std::string	const 	&getNickname(void)const;
		std::string	const 	&getUsername(void)const ;
		std::string const 	&getRealName()const ;
		std::string	const 	&getHost(void)const ;
		std::string		 	getMsgRecvd(void)const ;
		std::string		 	getPartialMsg(void)const ;
		std::string		 	getMsgSent(void)const ;
		std::string			getPrefix(void)const ;
		bool const			&getConnPwd(void) const ;
		bool const			&getWelcomeStatus(void) const ;
		bool const			&getRegistrationStatus(void)const ;
		bool const			&getDeconnStatus(void)const ;
		int	const			&getFd(void)const ;
		Server				*getServer(void);
		void				setNickname(std::string);
		void				setUsername(std::string);
		void				setRealName(std::string);
		void				setHost(std::string hot);
		void				setPwd(std::string pwd);
		void				setWelcomeStatus(bool);
		void 				setRecvMsg(std::string msg);
		void				setPartialMsg(std::string partialMsg);
		void				resetPartialMsg(void);
		void				resetRecvMsg(void);
;		
		/**********    Messages Management     *********/
		//void			registringClient(std::string s); //split the 1st line received from client to get names and set them if nec
		bool				isRegistred(void);
		void				sendMsgtoServer(std::string msg);
		void				recvMsgfromServer(void);
		void				welcomeClient(Server *serv);			
		bool				sendReply(int fd);
		
		/**********    Channel Management    *********/
		//std::string	const	&getChannelName()const;
		std::string			getActiveChannel(void);
		void				setChannelName(std::string n);
		void				partAllChannel(void);
		void			    deleteChannelName(const std::string& cName) const;

	private:
		Client(Client const &s);
		Client &operator=(Client const &s);

		std::string				_partialMsg; 
		std::string				_recvdFromServ; // each client has its own recv buffer. Must check that the whole msg has been received (ending with /0) then stock it in _recvFrom.
		t_names					_infos; 
		t_status				_state;
		Server*					_server;
		mutable std::deque<std::string> _channelName;
};

void	addToClientBuffer(Server *server, int cliFd, std::string reply);

#endif


