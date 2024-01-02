/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:03:37 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/02 12:06:04 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:39 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/02 12:01:58 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

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
#include "server.hpp"
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

class Server;
class Client;

class Channel : public Client
{
	public:
		Channel();
		Channel(Channel const &s);
		Channel &operator=(Channel const &s);
		~Channel();

	private:
		

		
	//public:
	//class FailConnexion : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't connect to Server" DEFAULT);
	//	}
	//};
	//class CantAcceptServer : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't accept Server's socket" DEFAULT);
	//	}
	//};
	//class CantReceive : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "The client coudn't receive message from Server" DEFAULT);
	//	}
	//};
	//	class CantSend : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "The client coudn't send message to Server" DEFAULT);
	//	}
	//};
};

#endif