/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:19:10 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/07 11:37:47 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"
#include "../lib/Client.hpp"
#include "../lib/IrcLib.hpp"

bool server_shutdown = false;

void	Server::signal_handler(int signal)
{
	if (signal == SIGPIPE)
		return ;
	server_shutdown = true;
	std::cout << BGREEN "[Server]" GREEN << " Shutting down...\n" DEFAULT;
	//Server::cleanServer();
	//exit(ERROR);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << GREY "Usage: " DEFAULT << av[0] << "<port> <password>\n";
		return 1;
	}
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	if (checkArg(av[1], av[2]) == false)
		return 2;
	Server server(av[1], av[2], timeinfo);
	server.setHint();
	if (server.fillServinfo(av[1]) == ERROR)
		return 2;
	server.initializeServer();
	signal(SIGINT, Server::signal_handler);
	try
	{
		server.manageConnections();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}