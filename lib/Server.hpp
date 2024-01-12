/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/12 19:06:02 by blefebvr         ###   ########.fr       */
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

struct servOp // ChanOps necessary variables
{
	std::string name;
	std::string	host;
	std::string	pwd;
	int			cliId; //client's fd
};

class Server
{
	public:
		Server(std::string port, std::string pwd);
		~Server();
		
		/*********    Assessors    *********/
		struct sockaddr_in 	&getServAdd(void);	
		socklen_t 			&getServAddLen(void);
		Channel 			&getChannel(void);
		Client				&getClient(void);
		servOp				&getOp(void);
		std::string			&getPwd(void);
		int					&getPort(void);
		void				setPwd(std::string pwd);

		/*********    Socket and connections management    *********/
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
		void 				manageConnections(void);
		void				manageExistingConn(pollfd fd);
		bool 				addConnections(std::vector<pollfd> fd, std::vector<pollfd>::iterator it);
		void 				handleExistingConn(void);
		//void or bool 		managePolloutEvent();
		//void or bool 		managePollerrEvents();

		/*********    Client management    *********/
		void 				addClient(std::vector<pollfd> fds,int fd);
		void				cantAddClient(int fd);
		void				delClient(std::vector <pollfd> fds, std::vector <pollfd>:: iterator &it, int cliFd);
		
		/*********    Channel management    *********/
		void				addChannel(std::string topic);
		void				delChannel(std::string topic);
		void				cantAddChannel(void);
		
	private:
		Server(Server const &s);
		Server &operator=(Server const &s);	
		
		int									_socServ;
		sockaddr_in 						_hints; // give indications for configure/initialize info linked to a network address
		sockaddr_in 						_servInfo; // stock server's infos for listening for connections
		std::string							_servPwd;
		std::string							_cliMsg;
		ssize_t     						_result; // variable qui retourne le nb de bytes envoyes par le client
    	ssize_t     						_remain;
		char								_buf[MAXBUF]; //buffer qui recoit les donnees
		int									_servPort;
		int									_cliNb;
		std::map<int, Client *>				_clients; //client id, client class
		std::map<std::string, Channel *>	_channels; // channel name, channel class
		std::vector<servOp>					_ops; // vector of serOp struct
};

#endif
