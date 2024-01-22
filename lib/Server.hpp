/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/22 11:17:40 by pmaimait         ###   ########.fr       */
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
class Command;

class Server
{
	public:
		Server(std::string port, std::string pwd);
		~Server();
		
		/*********    Assessors    *********/
		struct sockaddr_in 	&getServAdd(void);	
		socklen_t 			&getServAddLen(void);
		Channel*			getChannel(std::string chan);
		Client*				getClient(int fd);
		Client*				getClientByNickname(const std::string &nickname);
		std::string			&getPwd(void);
		std::string			&getServerName(void);
		std::string			&getStartTime(void);
		int					&getPort(void);
		std::string 		&getMsg(void);
		void				setPwd(std::string pwd);
		void				setMsg(std::string buf);
		//void 				setChannel(Channel *c);
		//void				setClient(Client *c);
		

		/*********    Socket and connections management    *********/
		void 	initializeServer(int port);
		void 	createServerSocket(void);
		void	bindServerSocket(int port);
		void	listenForConnection(void);
		void	closeServFd(void);
		int		acceptConnection(void);
		void 	checkPoll(int rc);
		void	checkReception(int rc);
		void 	manageConnections(void);
		int		manageExistingConn(std::vector<pollfd> fds, std::vector<pollfd>::iterator it);
		int 	addConnections(std::vector<pollfd> fd);
		int		handleExistingConn(void);
		int		managePolloutEvent(std::vector<pollfd> fds, std::vector<pollfd>::iterator it, int fd);
		int		managePollerrEvents(std::vector<pollfd> fds, std::vector<pollfd>::iterator it, int fd);

		/*********    Client management    *********/
		void 	addClient(std::vector<pollfd> fds,int fd);
		void	cantAddClient(int fd);
		void	delClient(std::vector <pollfd> fds, std::vector <pollfd>::iterator &it);
		void	sendMsg(void);
		int		receiveMsg(Client *cli, int fd);
		void	stockMsg(Client *cli, char *s);
		void 	splitMsg(Client *cli, std::string msg); // for split the message from the commande
		bool	isValidNickname(std::string name);
		int		checkRecv(int res, int fd);
		void	parseFirstMsg(std::string msg, int fd); // to set the command 
		
		/*********    Channel management    *********/
		void	addChannel(std::string topic);
		void	delChannel(std::string topic);
		void	cantAddChannel(void);
		
	private:
		Server(Server const &s);
		Server &operator=(Server const &s);	
		
		int									_servFd; // server's fd
		int									_servPort;
		int									_cliNb;
		sockaddr_in 						_hints; // give indications for configure/initialize info linked to a network address
		sockaddr_in 						_servInfo; // stock server's infos for listening for connections
		std::string							_servPwd;
		std::string							_cliMsg;
		std::string							_servName;
		std::string							_time;
		ssize_t     						_result; // variable qui retourne le nb de bytes envoyes par le client
		std::map<int, Client *>				_clients; //client id, client class
		std::map<std::string, Channel *>	_channels; // channel name, channel class
};

bool checkArg(std::string port, std::string pwd);

#endif
