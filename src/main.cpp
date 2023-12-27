/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:19:10 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/27 16:34:45 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/server.hpp"
#include "../lib/client.hpp"

//bool checkArg(std::string port, std::string pwd)
//{
	
//}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: <value>\n";
		return 1;
	}
	//if (checkArg(av[1], av[2]) == false)
	//	return 2;
	Server *serv = new Server(atoi(av[1]), av[2]);
	try
	{
		serv->createServerSocket();
		serv->acceptConnexions();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}