/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:39 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/26 17:35:37 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

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
#define MAXBUF	1096

class Client
{
	public:
		Client();
		Client(Client const &s);
		Client &operator=(Client const &s);
		~Client();

		void 	createClientSocket(int socServ);

	private:
		struct sockaddr_in*	_addCli;
		std::string			_servOutput;
		char				_buf[1096];
		int					_socCli;
		socklen_t			_cliSize;
		
	class FailConnexion : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "Coudn't connect to the server" DEFAULT);
		}
	};
	class CantAcceptServer : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "Coudn't accept server's socket." DEFAULT);
		}
	};
	class CantReceive : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return (YELLOW "The client coudn't receive message." DEFAULT);
		}
	};
};

#endif