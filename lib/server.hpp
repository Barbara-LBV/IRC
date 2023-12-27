/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/27 16:32:54 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <sys/epoll.h>
#include <exception>
#include <string.h>
#include "client.hpp"

# define DEFAULT "\001\033[0;39m\002"
# define RED "\001\033[1;91m\002"
# define YELLOW "\001\033[1;93m\002"
# define MAGENTA "\001\033[1;95m\002"
# define GRAY "\001\033[1;89m\002"
# define GREEN "\001\033[1;92m\002"
# define BLUE "\001\033[1;36m\002"

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define MAXBUF	1096
#define BACKLOG 5

class Client; 

class Server
{
	public:
		Server(int port, std::string pwd);
		Server(Server const &s);
		Server &operator=(Server const &s);
		~Server();

		// server socket creation
		void 	createServerSocket(void);

		//client connexions to server
		void	acceptConnexions(void);
		
	private:
		int						_socServ;
		struct sockaddr_in 		_addServ;
		std::string				_servPwd;
		std::string				_servInput;
		char					_buf[1096];
		//int						_servPort;
		std::map<int, Client *>	_clients;
		int						_nbCli;
		
	public:

	class CantCreateSocket : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "Coudn't create socket." DEFAULT);
		}
	};

	class CantBind : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "Coudn't bind socket" DEFAULT);
		}
	};
	
	class CantListen : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "Coudn't listen network." DEFAULT);
		}
	};
	class CantAccept: public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "Coudn't accept client's connection." DEFAULT);
		}
	};
	class CantSendMessage : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "Coudn't send to server." DEFAULT);
		}
	};
	class CantReceiveMessage : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "The server coudn't receive message." DEFAULT);
		}
	};
};

#endif