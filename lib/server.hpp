/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:11:00 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/22 16:49:03 by blefebvr         ###   ########.fr       */
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
#include <vector>
#include <sys/epoll.h>
#include <exception>

# define DEFAULT "\001\033[0;39m\002"
# define RED "\001\033[1;91m\002"
# define YELLOW "\001\033[1;93m\002"
# define MAGENTA "\001\033[1;95m\002"
# define GRAY "\001\033[1;89m\002"
# define GREEN "\001\033[1;92m\002"
# define BLUE "\001\033[1;36m\002"

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

class Server
{
	public:
		Server(int port, std::string pwd);
		Server(Server const &s);
		Server &operator=(Server const &s);
		~Server();

		// server socket creation
		void 	createSocket(void);

		//client connexions to server
		void	acceptConnexions(void);
		
	private:
		std::vector<int>	_fdsServ; //or in another class, which will handle the sockets, map ?? 
		int					_socServ;
		struct sockaddr_in 	_addServ;
		int					_servPort;
		std::string			_servPwd;
		
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
	class FailConnexion : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "Coudn't connect through server" DEFAULT);
		}
	};
};

#endif