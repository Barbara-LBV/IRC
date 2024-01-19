/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/19 12:20:05 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Client.hpp"

Client::Client(int fd, Server *server)
{
	_completeMsg = "";
	_partialMsg = "";
	_recvdFromServ = "";
   	_infos._cliFd = fd;
	_state._connectionPwd = FALSE;
	_state._registred = FALSE;
	_state._welcomed = FALSE;
	_state._toDisconnect = FALSE;
	_server = server;
	//_infos._pwd = server->getPwd();
}

Client::~Client(){}

std::string		Client::getNickname(void) const {return (_infos._nickname);}

std::string		&Client::getUsername(void) {return (_infos._username);}

std::string		&Client::getHost(void) {return (_infos._host);}

std::string		&Client::getMsgRecvd(void) {return (_recvdFromServ);}

std::string		&Client::getPartialMsg(void) {return (_partialMsg);}

std::string		&Client::getMsgSent(void) {return _completeMsg;}

bool			&Client::getRegistrationStatus(void) {return _state._registred;}

bool			&Client::getConnPwd(void){return _state._connectionPwd;}

bool			&Client::getWelcomeStatus(void){return _state._welcomed;}

bool			&Client::getDeconnStatus(void){return _state._toDisconnect;}

void			Client::setPartialMsg(std::string partialMsg){ _partialMsg += partialMsg;}

void			Client::setNickname(std::string nick){_infos._nickname = nick;}

void			Client::setUsername(std::string user){_infos._username = user;}

void			Client::setHost(std::string hostname){_infos._host = hostname;}

void			Client::setMsgSent(std::string msg){_completeMsg += msg;}

void			Client::setPwd(std::string pwd){_infos._pwd = pwd;}

std::string     Client::getPrefix(void) const
{
	if (this->getNickname().empty())
		return ("*");
	return _infos._nickname + (_infos._username.empty() ? "" : "!" + _infos._username) + (_infos._host.empty() ? "" : "@" + _infos._host);
}

bool			Client::isRegistred(void)
{
	if (_infos._nickname.empty())
		return FALSE;
	if (_infos._username.empty())
		return FALSE;
	if (_infos._pwd.empty())
		return FALSE;
	if (this->_state._registred == FALSE)
		this->_state._registred = TRUE;
	if (this->_state._welcomed == FALSE)
	{
		this->_state._welcomed = TRUE;
		this->welcomeClient();
	}
	return TRUE;	
}

void	Client::welcomeClient(void)
{
	if (!this->isRegistred())
		return ;
	reply(RPL_WELCOME(this->getNickname(), this->getPrefix()));
	reply(RPL_YOURHOST(this->getNickname(), this->_server->getServerName(), "0.1"));
	reply(RPL_CREATED(this->getNickname(), this->_server->getStartTime()));
	reply(RPL_MYINFO(this->getNickname(), this->_server->getServerName(), "0.1", "aiorsw", "IObeiklmnopstv"));

	// TODO: faire des fonction RPL
	reply("375 " + this->getNickname() + " :- " + this->_server->getServerName() + " Message of the day -");
	reply("372 " + this->getNickname() + " :- Welcome to our IRC server!");
	// personalize the "drawing" below
	reply("- .-.-----------.-.");
	reply("- | |--FT_IRC---|#|");
	reply("- | |-----------| |");
	reply("- | |-blefebvr--| |");
	reply("- | |-pmaimait--| |");
	reply("- | \"-42-Paris-' |");
	reply("- |  .-----.-..   |");
	reply("- |  |     | || |||");
	reply("- |  |     | || \\/|");
	reply("- \"--^-----^-^^---'");

	reply("376 " + this->getNickname() + " :End of MOTD command");
}

void	Client::reply(const std::string &reply) 
{
	std::cout << this->_infos._cliFd << " : " + this->_server->getServerName() + " " + reply << std::endl;
}

// void				Client::sendMsgtoServer(std::string msg)
// {

// }

// void				Client::recvMsgfromServer(void)
// {
    
// }

// void	Client::write(std::string s)
// {
	
// }

//void				Client::registringClient(std::string s)
//{

//}