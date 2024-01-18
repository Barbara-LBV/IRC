/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/18 15:55:39 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Client.hpp"

Client::Client(int fd)
{
	_completeMsg = "";
	_partialMsg = "";
	_recvdFromServ = "";
   	_infos._cliFd = fd;
	//t_status		_state;
}

Client::~Client(){}

std::string		Client::getNickname(void)const {return (_infos._nickname);}

std::string		Client::getUsername(void)const {return (_infos._username);}

std::string		Client::getHost(void)const {return (_infos._host);}

std::string		Client::getMsgRecvd(void)const {return (_recvdFromServ);}

std::string		Client::getPartialMsg(void)const {return (_partialMsg);}

std::string		Client::getMsgSent(void)const {return _completeMsg;}

void			Client::setPartialMsg(std::string partialMsg){ _partialMsg = partialMsg;}

void			Client::setNickname(std::string nick){_infos._nickname = nick;}

void			Client::setUsername(std::string user){_infos._username = user;}

void			Client::setHost(std::string hostname){_infos._host = hostname;}

void			Client::setMsgSent(std::string msg){_completeMsg = msg;}

void			Client::setPwd(std::string pwd){_infos._pwd = pwd;}

std::string     Client::getPrefix() const
{
	if (this->getNickname().empty())
		return ("*");
	return _infos._nickname + (_infos._username.empty() ? "" : "!" + _infos._username) + (_infos._host.empty() ? "" : "@" + _infos._host);
}

//void				Client::registringClient(std::string s)
//{

//}

//void				Client::sendMsgtoServer(std::string msg)
//{

//}

//void				Client::recvMsgfromServer(void)
//{
    
//}