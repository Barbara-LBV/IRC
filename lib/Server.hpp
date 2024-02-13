/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/13 17:34:58 by blefebvr         ###   ########.fr       */
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
		CmdHandler 			*getCmdHandler(void){return _handler;};
		void				setFd(int fd);
		void				setPwd(std::string pwd);
		void				setMsg(std::string buf);
		void				setDatetime(struct tm *timeinfo);
		//void 				setChannel(Channel *c);
		//void				setClient(Client *c);
		
		/*********    Socket and connections management    *********/
		int 				initializeServer(void);
		int 				createServerSocket(void);
		int					bindServerSocket(void);
		int					listenForConnection(void);
		int					acceptConnection(void);
		int 				checkPoll(int rc);
		void 				manageConnections(void);
		int					manageExistingConn(std::vector<pollfd> &fds, std::vector<pollfd>::iterator it);
		int					addConnections(std::vector<pollfd> &poll_fds, std::vector<pollfd> &new_poll);
		int					managePolloutEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, int fd);
		int					managePollerEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, int fd);
		pollfd				getActivePoll(int i);
		int 				fillServinfo(char *port);

		/*********    Client management    *********/
		void				addClient(std::vector<pollfd> &new_poll, Client *cli);
		void				cantAddClient(int fd);
		void				delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator it, int fd);
		int					receiveMsg(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator it);
		void				stockMsg(Client *cli, char *s);
		bool				isValidNickname(std::string name);
		int					checkRecv(std::vector<pollfd> &poll_fds, int res, std::vector<pollfd>::iterator it);
		void				parseMsg(std::string msg, int fd); // to set the command 
		void				fillClient(Client *cli, std::vector <std::string> cmds); // with first
		
		/*********    Channel management    *********/
		void				addChannel(std::string chan_name, Channel* channel);
		void				delChannel(Channel *chan);
		void				cantAddChannel(void);
		Channel* 			getChannel(const std::string& cName);
		void 				broadcastChannel(Client* client, std::string message, Channel* channel);
		bool				isValidChannelName(std::string cName);
		
		/*********        Cleaning       ************/
		static void			signal_handler(int signal);
		
	private:
		Server(Server const &s);
		Server &operator=(Server const &s);	
		
		int						_servFd; // server's fd
		int						_servPort;
		int						_cliNb; // nb of active clients
		addrinfo 				_hints; // give indications for configure/initialize info linked to a network address
		addrinfo 				*_servInfo; // stock server's infos for listening for connections
		std::string				_servPwd; // server's password
		std::string				_servName; // server's name
		std::string				_time;
		ssize_t     			_result; // variable qui retourne le nb de bytes envoyes par le client
		std::map<int, Client *>	_clients; //client id, client class
		std::vector<Channel *>	_channels; // channel name, channel class
		CmdHandler				*_handler; // manage the cmmands
};

bool 							checkArg(std::string port, std::string pwd);
std::vector<std::string> 		splitMsg(std::string msg, char c); // for split the message from the commande

#endif
