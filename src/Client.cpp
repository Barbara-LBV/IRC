/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/23 18:54:38 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Client.hpp"

Client::Client(int fd, Server *server)
{
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

bool			&Client::getRegistrationStatus(void) {return _state._registred;}

bool			&Client::getConnPwd(void){return _state._connectionPwd;}

bool			&Client::getWelcomeStatus(void){return _state._welcomed;}

bool			&Client::getDeconnStatus(void){return _state._toDisconnect;}

std::string		Client::getChannelName(void){return _channelName.top();}

int				&Client::getFd(void){return _infos._cliFd;}

Server			*Client::getServer(void){return _server;}

void			Client::setPartialMsg(std::string partialMsg){ _partialMsg += partialMsg;}

void			Client::setNickname(std::string nick){_infos._nickname = nick;}

void			Client::setUsername(std::string user){_infos._username = user;}

void			Client::setHost(std::string hostname){_infos._host = hostname;}

void			Client::setRecvMsg(std::string msg){_recvdFromServ += msg;}

void			Client::setPwd(std::string pwd){_infos._pwd = pwd;}

void			Client::setChannelName(std::string n){_channelName.push(n);}

std::string     Client::getPrefix(void) const
{
	if (this->getNickname().empty())
		return ("*");
	return _infos._nickname + (_infos._username.empty() ? "" : "!" + _infos._username) + (_infos._host.empty() ? "" : "@" + _infos._host);
}

bool	Client::sendReply(int fd)
{
	int res;
	res = send(fd, getMsgRecvd().c_str(), MAXBUF, 0);
	if (res == ERROR)
	{
		std::cerr << "[Server] Sending reply failed.\n";
		exit(ERROR);
	}
	if (res == 0)
	{
		std::cerr << "[Server] Client n#" << fd << " has disconnected\n";
		//delClient(vector pollfd, fd);
		return FALSE;
	}
	getMsgRecvd().clear();
	return TRUE;
}

bool			Client::isRegistred(void)
{
	if (this->_state._registred == TRUE)
		return TRUE;
	if (_infos._nickname.empty())
		return FALSE;
	if (_infos._username.empty())
		return FALSE;
	if (_infos._pwd.empty())
		return FALSE;
	if (this->_state._registred == FALSE)
	{
		this->_state._registred = TRUE;
		if (this->_state._welcomed == FALSE)
		{
			this->welcomeClient(this->getServer());
			this->_state._welcomed = TRUE;
		}
	}
	return TRUE;	
}

void	Client::welcomeClient(Server *serv)
{
	if (!this->isRegistred())
		return ;
	addToClientBuffer(serv, this->getCliFd(), RPL_WELCOME(this->getNickname(), this->getPrefix()));
	addToClientBuffer(serv, this->getCliFd(), RPL_YOURHOST(this->getNickname(), this->_server->getServerName(), "0.1"));
	addToClientBuffer(serv, this->getCliFd(), RPL_CREATED(this->getNickname(), this->_server->getStartTime()));
	addToClientBuffer(serv, this->getCliFd(), RPL_MYINFO(this->getNickname(), this->_server->getServerName(), "0.1", "aiorsw", "IObeiklmnopstv"));
	
	addToClientBuffer(serv, this->getCliFd(), "375 " + this->getNickname() + " :- " + this->_server->getServerName() + " Message of the day -");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + " :- Welcome to our IRC server!");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- .-.-----------.-.");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- | |--FT_IRC---|#|");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- | |-----------| |");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- | |-blefebvr--| |");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- | |-pmaimait--| |");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- | \"-42-Paris-' |");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- |  .-----.-..   |");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- |  |     | || |||");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- |  |     | || \\/|");
	addToClientBuffer(serv, this->getCliFd(), "372 " + this->getNickname() + "- \"--^-----^-^^---'");
	addToClientBuffer(serv, this->getCliFd(), "376 " + this->getNickname() + " :End of MOTD command");
}
