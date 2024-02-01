/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/01 16:20:12 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <csignal>
#include "IrcLib.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "CmdHandler.hpp"

extern bool server_shutdown;

class Client;
class Channel;
class CmdHandler;

class Server
{
	public:
		Server(std::string port, std::string pwd, struct tm * time);
		~Server();
		
		/*********    Assessors    *********/
		struct sockaddr_in 	&getServAdd(void);	
		socklen_t 			&getServAddLen(void);
		
		Client*				getClient(int fd);
		Client*				getClientByNickname(const std::string &nickname);
		std::string			&getPwd(void);
		std::string			&getServerName(void);
		std::string			&getStartTime(void);
		int					&getPort(void);
		int					&getFd(void);
		std::string 		&getMsg(void);
		void				setFd(int fd);
		void				setPwd(std::string pwd);
		void				setMsg(std::string buf);
		void				setDatetime(struct tm *timeinfo);
		//void 				setChannel(Channel *c);
		//void				setClient(Client *c);
		
		/*********    Socket and connections management    *********/
		void 				initializeServer(int port);
		void 				createServerSocket(void);
		void				bindServerSocket(int port);
		void				listenForConnection(void);
		void				closeServFd(void);
		int					acceptConnection(void);
		int 				checkPoll(int rc);
		void 				manageConnections(void);
		int					manageExistingConn(std::vector<pollfd> fds, std::vector<pollfd>::iterator it);
		int					addConnections(void);
		int					handleExistingConn(void);
		int					managePolloutEvent(int fd);
		int					managePollerrEvents(int fd);
		pollfd				getActivePoll(int i);

		/*********    Client management    *********/
		int					addClient(int fd);
		void				cantAddClient(int fd);
		void				delClient(int fd);
		//bool				sendReply(int fd);
		int					receiveMsg(int fd);
		void				stockMsg(Client *cli, char *s);
		bool				isValidNickname(std::string name);
		int					checkRecv(int res, int fd);
		void				parseMsg(std::string msg, int fd); // to set the command 
		void				fillClient(Client *cli, std::vector <std::string> cmds); // with first
		void				parseCmd(Client *cli, std::vector <std::string> cmds);
		//void				addToClientBuffer(Server *s, Client *cli, std::string reply);

		/*********    Channel management    *********/
		void				addChannel(std::string topic);
		void				delChannel(std::string topic);
		void				cantAddChannel(void);
		Channel* 			getChannel(const std::string& cName);
		void 				broadcastChannel(std::string message, Channel* channel);
		bool				isValidChannelName(std::string cName);
		
	private:
		Server(Server const &s);
		Server &operator=(Server const &s);	
		
		int									_servFd; // server's fd
		int									_servPort;
		int									_cliNb; // nb of active clients
		sockaddr_in 						_hints; // give indications for configure/initialize info linked to a network address
		sockaddr_in 						_servInfo; // stock server's infos for listening for connections
		std::string							_servPwd; // server's password
		std::string							_servName; // server's name
		std::string							_time;
		ssize_t     						_result; // variable qui retourne le nb de bytes envoyes par le client
		std::map<int, Client *>				_clients; //client id, client class
		std::map<std::string, Channel *>	_channels; // channel name, channel class
		CmdHandler							*_handler; // manage the cmmands
		//std::vector<pollfd>					_poll_fds; // to handle all cnnections
};

bool 						checkArg(std::string port, std::string pwd);
std::vector<std::string> 	splitMsg(std::string msg, char c); // for split the message from the commande

#endif
