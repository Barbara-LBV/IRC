/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/08 17:09:43 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <csignal>
#include "IrcLib.hpp"
#include "Client.hpp"
#include "Channel.hpp"

extern bool server_shutdown;

class Client;
class Channel; 

struct servOp // necessary variables super-clients XD
{
	std::string name;
	std::string	host;
	std::string	pwd;
};

class Server
{
	public:
		Server(std::string port, std::string pwd);
		~Server();
		
		/* Assessors */
		struct sockaddr_in 	&getServAdd(void);	
		socklen_t 			&getServAddLen(void);
		Channel 			&getChannel(void);
		Client				&getClient(void);
		servOp				&getOp(void);
		std::string			&getPwd(void);
		int					&getPort(void);
		void				setPwd(std::string pwd);

		/* Socket management */
		void 				initializeServer(int port);
		void 				createServerSocket(void);
		void				bindServerSocket(int port);
		void				listenForConnection(void);
		void				closeServFd();
		int					acceptConnection(void);
		void 				checkPoll(int rc);
		void				checkReception(int rc);
		void				sendMsg(void);
		void				receiveMsg(void);

		/* Client management */
		void 				addClient(void);
		void				delClient(std::vector<pollfd> fds);
		void 				manageConnections(void);

		/* Channel management */
		void				addChannel(std::string topic);
		void				delChannel(std::string topic);
		
	private:
		Server(Server const &s);
		Server &operator=(Server const &s);	
	//protected: 
		int									_socServ;
		sockaddr_in 						_hints; // give indications for configure/initialize info linked to a network address
		sockaddr_in 						_servInfo; // stock server's infos for listening for connections
		std::string							_servPwd;
		std::string							_servInput;
		ssize_t     						_result; //variable qui retourne le nb de bytes envoyes par le client
    	ssize_t     						_remain;
		char								_buf[MAXBUF]; //buffer qui recoit les donnees
		int									_servPort;
		std::map<int, Client *>				_clients; //client id, client class
		std::map<std::string, Channel *>	_channels; // channel name, channel class
		std::vector<servOp>					_ops; // vector of serOp struct
};

bool checkArg(std::string port, std::string pwd);

#endif
