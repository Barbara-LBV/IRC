/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:03:37 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/15 12:14:21 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "IrcLib.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Channel
{
	public:
		Channel(std::string topic);
		~Channel();

		/***********   Assessors   ***********/
		const std::string 	&getTopic(void) const;
		bool                getStatus(int);
		Client*             getOperator(void);
		std::string         getStatus(void);
		void 				setName(const std::string &name);
		void 				setTopic(const std::string &topic);
		void 				setStatus(std::string &status);
		
		/***********   Functions   ***********/
		void 				joinChannel(Client *cli);
   		void 				partChannel(Client *cli);
    	void 				invitationChannel(Client *cli);

	private:
		Channel(Channel const &c);
		Channel &operator=(Channel const &c);
		
		std::string	_topic;
		int			_status; // open/close status ??
		//servOp		_ops; // one chanop per channel
};

#endif