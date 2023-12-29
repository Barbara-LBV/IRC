/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:19:10 by blefebvr          #+#    #+#             */
/*   Updated: 2023/12/29 15:53:25 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/server.hpp"
#include "../lib/client.hpp"

bool checkArg(std::string port, std::string pwd)
{
	if (pwd.size() > 10)
	{
		std::cerr << "Chosen password is too long.\n";
		return false;
	}
	if (port.size() > 5)
	{
		std::cerr << port << " is out of port range (0 ~ 65535)\n";
		return false;
	}
	int portNb = atoi(port.c_str());
	if (portNb > 65535 || portNb < 0)
	{
		std::cerr << port << " is out of port range (0 ~ 65535)\n";
		return false;
	}
	return true;
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
	Server *serv = new Server();
	try
	{
		serv->initializeServer(atoi(av[1]));
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}