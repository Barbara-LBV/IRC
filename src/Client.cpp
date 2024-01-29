/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/29 11:06:37 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Client.hpp"

Client::Client(int fd, Server *server)
{
	_partialMsg = "";
	_recvdFromServ = "";
   	_infos._cliFd = fd;
	_infos._host = "localhost";
	_state._connectionPwd = FALSE;
	_state._registred = FALSE;
	_state._welcomed = FALSE;
	_state._toDisconnect = FALSE;
	_server = server;
	//_infos._pwd = server->getPwd();
}

Client::~Client(){}

std::string	const 	&Client::getNickname(void) const {return (_infos._nickname);}

std::string	const 	&Client::getUsername(void) const {return (_infos._username);}

std::string const 	&Client::getRealName()const {return (_infos._realname);}

std::string	const 	&Client::getHost(void)const  {return (_infos._host);}

std::string	const 	&Client::getMsgRecvd(void)const  {return (_recvdFromServ);}

std::string	const 	&Client::getPartialMsg(void)const  {return (_partialMsg);}

bool const			&Client::getRegistrationStatus(void)const {return _state._registred;}

bool	const		&Client::getConnPwd(void)const {return _state._connectionPwd;}

bool	const		&Client::getDeconnStatus(void)const{return _state._toDisconnect;}

bool const			&Client::getWelcomeStatus(void) const {return _state._welcomed;}

//std::string	const	&Client::getActiveChannel(void)const{return _channelName.top();}

int	const			&Client::getFd(void)const{return _infos._cliFd;}

Server				*Client::getServer(void){return _server;}

void				Client::setPartialMsg(std::string partialMsg){ _partialMsg += partialMsg;}

void				Client::setNickname(std::string nick){_infos._nickname = nick;}

void				Client::setUsername(std::string user){_infos._username = user;}

void				Client::setRealName(std::string realUser){_infos._realname = realUser;}

void				Client::setHost(std::string hostname){_infos._host = hostname;}

void				Client::setRecvMsg(std::string msg){_recvdFromServ += msg;}

void				Client::setPwd(std::string pwd){_infos._pwd = pwd;}

void				Client::setWelcomeStatus(bool b){_state._welcomed = b;}

void				Client::setChannelName(const std::string cName){_channelName.push_back(cName);}

std::string  		Client::getPrefix(void)const
{
	if (this->getNickname().empty())
		return ("*");
	return _infos._nickname + "!" + _infos._username + "@" + _infos._host;
}

std::string		Client::getActiveChannel(void)
{
    if (!_channelName.empty())
        return (_channelName.back()); 
    return NULL;
}

void				Client::deleteChannelName(const std::string& cName) const
{
    std::deque<std::string>::iterator it = std::remove(_channelName.begin(), _channelName.end(), cName);
    _channelName.erase(it, _channelName.end());
}

bool				Client::sendReply(std::vector<pollfd> fds, int fd, size_t i)
{
	int res;
	std::cout << "msg sent to client = " << getMsgRecvd() << std::endl;
	res = send(fd, getMsgRecvd().c_str(), MAXBUF, 0);
	if (res == ERROR)
	{
		std::cerr << "[Server] Sending reply failed.\n";
		setRecvMsg("");
		exit(ERROR);
	}
	if (res == 0)
	{
		std::cerr << "[Server] Client n#" << fd << " has disconnected\n";
		getServer()->delClient(fds, i);
		return FALSE;
	}
	return TRUE;
}

bool				Client::isRegistred(void)
{
	if (this->_state._registred == TRUE)
		return TRUE;
	if (!_infos._nickname.empty() && !_infos._username.empty() && !_infos._pwd.empty() && !_infos._realname.empty())
		return TRUE;
	return FALSE;	
}

void				Client::welcomeClient(Server *serv)
{
	if (this->isRegistred() == FALSE)
		return ;
	addToClientBuffer(serv, this->getFd(), RPL_WELCOME(this->getNickname(), this->getPrefix()));
	addToClientBuffer(serv, this->getFd(), RPL_YOURHOST(this->getNickname(), this->_server->getServerName(), "0.1"));
	addToClientBuffer(serv, this->getFd(), RPL_CREATED(this->getNickname(), this->_server->getStartTime()));
	addToClientBuffer(serv, this->getFd(), RPL_MYINFO(this->getNickname(), this->_server->getServerName(), "1.1", "io", "kost", "k"));
	
	addToClientBuffer(serv, this->getFd(), RPL_MOTDSTART(this->getNickname(), "42_Ftirc (localhost)"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), " :- Welcome to our IRC server!"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- .-.-----------.-."));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | |--FT_IRC---|#|"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | |-----------| |"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | |-blefebvr--| |"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | |-pmaimait--| |"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | \"-42-Paris-' |"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- |  .-----.-..   |"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- |  |     | || |||"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- |  |     | || \\/|"));
	addToClientBuffer(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- \"--^-----^-^^---'"));
	addToClientBuffer(serv, this->getFd(), RPL_ENDOFMOTD(this->getNickname()));
}

void				Client::partAllChannel(void)
{
	while (!getActiveChannel().empty())
	{
		Channel* channel = _server->getChannel(getActiveChannel());
		channel->partChannel(this);
		deleteChannelName(getActiveChannel());
	}
}