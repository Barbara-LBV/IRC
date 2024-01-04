/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/04 18:00:44 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <fstream>
#include <csignal>
#include "IrcLib.hpp"
#include "Client.hpp"
#include "Channel.hpp"

external bool server_shutdown;

class Client;
class Channel; 

class Server
{
	public:
		Server(std::string port, std::string pwd);
		~Server();
		typedef struct s_socket
		{
			int         fd;
			sockaddr_in addr;
			socklen_t   len;
		} t_socket;
		
		void 				initializeServer(int port);
		void 				createServerSocket(void);
		void				bindServerSocket(int port);
		void				listenForConnection(void);
		void				closeServFd();
		int					acceptConnection(void);
		void 				manageConnections(void);
		int					reduceFds(int nfds);
		void 				checkPoll(int rc);
		void				checkReception(int rc);
		void				sendMsg(void);
		void				receiveMsg(void);
		struct sockaddr_in &getServAdd(void);	
		socklen_t 			&getServAddLen(void);
		
	private:
		Server(Server const &s);
		Server &operator=(Server const &s);
		
	protected:
		t_socket			_socServ;
		struct sockaddr_in 	_add; // what for, as I have a t_socket struct ??
		socklen_t   		_addLen; // what for, as I have a t_socket struct ??
		std::string			_servPwd;
		std::string			_servInput;
		ssize_t     		_result; //variable qui retourne le nb de bytes envoyes par le client
    	ssize_t     		_remain;
		char				_buf[MAXBUF]; //buffer qui recoit les donnees
		int					_fd; // what for, as I have a t_socket struct ??
		struct pollfd		_fds[BACKLOG];
		int					_servPort;
		//std::map<int, Client *>	_clients;
		int					_nbCli;

};

#endif
/*     Have to manage the errors with errno instead of try/catch  */
		
	//public:

	//class CantCreateSocket : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't create socket." DEFAULT);
	//	}
	//};

	//class CantConfigSocket : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't configure socket." DEFAULT);
	//	}
	//};
	
	//class CantBind : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't bind socket" DEFAULT);
	//	}
	//};
	
	//class CantListen : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't listen network." DEFAULT);
	//	}
	//};
	//class CantAccept: public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't accept client's connection." DEFAULT);
	//	}
	//};
	//class CantSendMessage : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't send to client." DEFAULT);
	//	}
	//};
	//class CantReceiveMessage : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "The server coudn't receive message." DEFAULT);
	//	}
	//};
	//class PollIssue : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return ("");
	//	}
	//};
