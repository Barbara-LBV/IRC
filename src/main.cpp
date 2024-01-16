/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:19:10 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/16 14:42:28 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"
#include "../lib/Client.hpp"
#include "../lib/IrcLib.hpp"
//#include "Parsing.cpp"

bool server_shutdown = false;

static void	signal_handler(int signal)
{
	(void)signal;
	server_shutdown = true;
	std::cout << "[Server] Shutting down...\n";
	exit(ERROR);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>\n";
		return 1;
	}
	if (checkArg(av[1], av[2]) == false)
		return 2;
	Server server(av[1], av[2]);
	server.initializeServer(atoi(av[1]));
	signal(SIGINT, signal_handler);
	try
	{
		server.manageConnections();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		//serv.closeServFd();
		//cli.closeCliFd();
	}
	return 0;
}