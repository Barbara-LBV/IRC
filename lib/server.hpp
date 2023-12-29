/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/29 15:50:05 by blefebvr         ###   ########.fr       */
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
#include <fcntl.h>
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
		Server();
		~Server();
		typedef struct s_socket
		{
			int         fd;
			sockaddr_in addr;
			socklen_t   len;
		} t_socket;
		
		//server socket creation
		void 	initializeServer(int port);
		void 	createServerSocket(void);
		void	bindServerSocket(int port);
		void	listenForConnetion(void);
		//client connexions to server
		int		acceptConnection(void);
		
	private:
		Server(Server const &s);
		Server &operator=(Server const &s);
		//server socket creation
		//void 	initializeServer(int port);
		//void 	createServerSocket(void);
		//void	bindServerSocket(int port);
		//void	listenForConnetion(void);
		//client connexions to server
		//int	acceptConnection(void);
		
	protected:
		t_socket			_socServ;
		struct sockaddr_in 	_add;
		std::string			_servPwd;
		std::string			_servInput;
		ssize_t     		_result;
    	ssize_t     		_remain;
		char				_buf[MAXBUF];
		int					_fd;
		
		//int						_servPort;
		//std::map<int, Client *>	_clients;
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