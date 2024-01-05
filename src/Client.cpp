/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/05 14:43:01 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Client.hpp"

Client::Client()
{
	_servOutput = "";
	_socCli = 0;
	_cliSize = 0;
}

Client::Client(Client const &c)
{
	*this = c;
}

Client &Client::operator=(Client const &c)
{
	if (this != &c)
	{
		_servOutput = c._servOutput;
		for (int i = 0; i < MAXBUF; i++)
			_buf[i] = c._buf[i];
	}
	return *this;
}

Client::~Client()
{
}

void	Client::connectToServer(Server &s)
{
    int fd_cli = s.acceptConnection();

    // Set the socket to non-blocking
    fcntl(fd_cli, F_SETFL, O_NONBLOCK);

    // Initialize pollfd structure
    struct pollfd fds[1];
    fds[0].fd = fd_cli;
    fds[0].events = POLLOUT;  // We are interested in the socket being writable

    // Attempt to connect
    if (connect(fd_cli, (struct sockaddr *)&s.getServAdd(), s.getServAddLen()) == ERROR)
    {
		std::cout << "errno = " << errno << std::endl;
        if (errno == EINPROGRESS)
        {
            // Connection is in progress, wait for it to complete using poll
            int pollResult = poll(fds, 1, /*timeout in milliseconds*/ -1);

            if (pollResult == -1)
            {
                // Handle poll error
				std::cout << "poll error" << std::endl;
                throw FailConnexion();
            }
            else if (pollResult == 0)
            {
                // Timeout occurred (optional, depending on your requirements)
                throw TimeoutException();
            }
            else
            {
                // Check if the socket is now writable
                if (fds[0].revents & POLLOUT)
                {
                    // Connection is complete, handle it appropriately
                    std::cout << "Connection established!" << std::endl;
                }
                else
                {
                    // Connection failed
					std::cout << "poll connection failed" << std::endl;
                    throw FailConnexion();
                }
            }
        }
        else
        {
            // Handle other connection errors
			std::cout << "other connection errors" << std::endl;
            throw FailConnexion();
        }
    }

    std::cout << "Connecting ..." << std::endl;
}

//void	Client::connectToServer(Server &s)
//{
//	int fd_cli = s.acceptConnection();
//	fcntl(fd_cli, F_SETFL, O_NONBLOCK);
//	//it's the client socket in the 1st param, and the server addr in 2nd et 2rd params
//	if (connect(fd_cli, (struct sockaddr *)&s.getServAdd(), s.getServAddLen()) == ERROR)
//	{
//		if (errno == EINPROGRESS)
//        {
//            // Connection is in progress, handle it appropriately
//            // You can use select/poll/epoll to check for the completion of the connection
//        }
//        else
//        	throw FailConnexion();
//	}
//	std::cout << "Connecting ..." << std::endl;
//}

int	const	&Client::getCliSocket(void)
{
	return _socCli;
}

socklen_t &Client::getCliSize(void)
{
	return _cliSize;
}