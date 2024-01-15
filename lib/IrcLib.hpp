/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcLib.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 16:21:43 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/15 18:38:14 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCLIB_HPP
# define IRCLIB_HPP

/****************** Libraries *******************/

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
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/epoll.h>
#include <exception>
#include <string.h>
#include <cerrno>
#include <fcntl.h>
#include <poll.h>
#include <ctime>
#include <sys/types.h>

/****************** Includes *******************/

#include "Replies.hpp"
// # include "Command.hpp"

/*********************  Colors *******************/

# define DEFAULT "\001\033[0;39m\002"
# define RED "\001\033[1;91m\002"
# define YELLOW "\001\033[1;93m\002"
# define MAGENTA "\001\033[1;95m\002"
# define GRAY "\001\033[1;89m\002"
# define GREEN "\001\033[1;92m\002"
# define BLUE "\001\033[1;36m\002"

/*********************  Macros *******************/

# define ERROR 		-1
# define MAXBUF		4096
# define BACKLOG 	5
# define MAXCONN	10
# define MAXCHAN	10
# define TRUE		1
# define FALSE		0

#endif