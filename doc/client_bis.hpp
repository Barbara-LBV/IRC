#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "IrcLib.hpp"
#include "Channel.hpp"

class Server;
class Channel; 

class Client
{
	public:
		Client();
		~Client();
		
		/* structures */
		typedef struct s_status //client status, usefull also for channel management
		{
			/* what's client status : registred, joined(channel) etc*/
			unsigned char pass : 1;
			
		} t_status;
		
		typedef struct s_names // all the infos allowing to identify a client
		{
			std::string	nickname; // pseudo: usefull for channel operator
			std::string	username; 
			sockaddr_in ip_addr; // IP adress: usefull for channel operator
			int			user_id; 
			
		} t_names;
		
		/* Assessors */
		
		/* connections management */
		
		/* Channel management */
		
		//void 			createClientSocket(Server &s);
		void  			connectToServer(Server &s);
		void			sendMsg();
		void			recvMsg();
		void			closeCliFd();
		int	const		&getCliSocket(void);
		struct sockaddr &getCliAdd(void);
		socklen_t 		&getCliSize(void);

	private:
		Client(Client const &s);
		Client &operator=(Client const &s);
		
		std::string			_servOutput;
		char				_buf[1096]; //each client has its own buffer. Must check that the whole msg has been received (ending with /0)
		int					_socCli;
		socklen_t			_cliSize;
		struct sockaddr_in	*_addCli;
};

#endif

/*     Have to manage the errors with errno instead of try/catch  */

	//public:
	//class FailConnexion : public std	//public:

	//class CantCreateSocket : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't create socket." DEFAULT);
	//	}
	//};

	//class CantConfigSocket : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't configure socket." DEFAULT);
	//	}
	//};
	
	//class CantBind : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't bind socket" DEFAULT);
	//	}
	//};
	
	//class CantListen : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't listen network." DEFAULT);
	//	}
	//};
	//class CantAccept: public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't accept client's connection." DEFAULT);
	//	}
	//};
	//class CantSendMessage : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't send to client." DEFAULT);
	//	}
	//};
	//class CantReceiveMessage : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "The server coudn't receive message." DEFAULT);
	//	}
	//};
	//class PollIssue : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return ("");
	//	}
	//};::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't connect to Server" DEFAULT);
	//	}
	//};
	//class CantAcceptServer : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Coudn't accept Server's socket" DEFAULT);
	//	}
	//};
	//class CantReceive : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "The client coudn't receive message from Server" DEFAULT);
	//	}
	//};
	//class CantSend : public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "The client coudn't send message to Server" DEFAULT);
	//	}
	//};
	//class TimeoutException: public std::exception
	//{
	//public:
	//	virtual const char* what() const throw()
	//	{
	//		return (YELLOW "Connection time out." DEFAULT);
	//	}
	//};


