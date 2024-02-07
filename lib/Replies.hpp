/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 11:52:09 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/07 15:12:02 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// for all the different automatic responses server and client have to send
// http://chi.cs.uchicago.edu/chirc/irc_examples.html
// http://chi.cs.uchicago.edu/chirc/irc_examples.html#logging-into-an-irc-server
// https://gist.github.com/proxypoke/2264878

#ifndef REPLIES_HPP
# define REPLIES_HPP

//ERROR REPLIES
#define ERR_NEEDMOREPARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters\r\n"
#define ERR_NOTONCHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel\r\n"
#define ERR_USERNOTINCHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel\r\n"
#define ERR_CHANNELISFULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l) \r\n"
#define ERR_BADCHANNELKEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)\r\n"
#define ERR_INVITONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)\r\n"
#define ERR_NOSUCHCHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel\r\n"
#define ERR_CHANOPRIVSNEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator\r\n"
#define ERR_NONICKNAMEGIVEN(source)						"431 " + source + " :Nickname not given\r\n"
#define ERR_NICKNAMEINUSE(source, nickname)				"433 " + source + " " + nickname + " :Nickname is already in use\r\n"
#define ERR_ALREADYREGISTERED(source)					"462 " + source + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(source)						"464 " + source + " :Password incorrect\r\n"
#define ERR_NOTREGISTERED(source)						"451 " + source + " :You have not registered\r\n"
#define ERR_UNKNOWNCOMMAND(source, command)				"421 " + source + " " + command + " :Unknown command\r\n"
#define ERR_USERONCHANNEL(source, target, channel)		"443 " + source + " " + target + " " + channel + " :is already on channel\r\n"
#define ERR_NOSUCHNICK(source, name)					"401 " + source + " " + name + " :No such nick/channel\r\n"
#define ERR_INVITEONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)\r\n"

// NUMERIC REPLIES
#define RPL_WELCOME(source, prefix)										"001 " + source + " :Welcome to the Internet Relay Network " + prefix + "\r\n"
#define RPL_YOURHOST(source, servername, version)						"002 " + source + " :Your host is " + servername + ", running version " + version + "\r\n"
#define RPL_CREATED(source, date)										"003 " + source + " :This server was created " + date + "\r\n"
#define RPL_MYINFO(source, servername, version, usermodes, chanmodes)	"004 " + source + " :" + servername + " " + version + " " + usermodes + " " + chanmodes + "\r\n"

#define RPL_NAMREPLY(source, channel, users)			"353 " + source + " = " + channel + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(source, channel)					"366 " + source + " " + channel + " :End of /NAMES list.\r\n"
#define RPL_INVITING(source, channel, target)			"341 " + source + " " + channel + " " + target + "\r\n"

#define RPL_NOTOPIC(source, channel)					"331 " + source + " " + channel + " :No topic is set\r\n"
#define RPL_TOPIC(source, channel, topic)				"332 " + source + " " + channel + " :" + topic + "\r\n"

#define RPL_WHOREPLY(source, channel, username, hostname, serverhostname, nickname, realname)	"352 " + source + " " + channel + " " + username + " " + hostname + " " + serverhostname + " " + nickname + " H :0 " + realname + "\r\n"
#define RPL_ENDOFWHO(source, channel)					"315 " + source + " " + channel + " :End of WHO list\r\n"

#define RPL_LIST(source, channel, nbUsers, topic)		"322 " + source + " " + channel + " " + nbUsers + " :" + topic + "\r\n"
#define RPL_LISTEND(source)					"323 " + source + " :End of LIST\r\n"

// COMMAND REPLIES

#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args + "\r\n"
#define RPL_PING(source, token)						":" + source + " PONG :" + token + "\r\n"
#define RPL_QUIT(source, message)					":" + source + " QUIT :" + message + "\r\n"
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel + "\r\n"
#define RPL_PART(source, channel)					":" + source + " PART " + channel + "\r\n"
#define RPL_PART_REASON(source, channel, reason)	":" + source + " PART " + channel + " :" + reason + "\r\n"
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason + "\r\n"

#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message + "\r\n"
//#define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")

#define RPL_NOTICE(source, target, message)			":" + source + " NOTICE " + target + " :" + message + "\r\n"
#define RPL_INVITE(source, target, channel)			":" + source + " INVITE " + target + " :" + channel + "\r\n"

//#define CLIENT_ID(nickname, username) (":" + nickname + "!" + username + "@localhost")

///*********************  GLOBAL ERRORS   **********************/

//#define ERR_NEEDMOREPARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters"
//#define ERR_NOTREGISTERED(source)						"451 " + source + " :You have not registered"
//#define ERR_UNKNOWNCOMMAND(source, command)				"421 " + source + " " + command + " :Unknown command"
//#define ERR_NOSUCHNICK(source, name)					"401 " + source + " " + name + " :No such nick/channel"
//#define ERR_INVITEONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)"

///*********************  CMDS NUMERIC REPLIES   **********************/

//// REGISTRATION
//# define RPL_WELCOME(CLIENT_ID, nickname) 				"001 " + nickname + " :Welcome to the Internet Relay Network " + CLIENT_ID + "\r\n"
//# define RPL_YOURHOST(client, servername, version) 		      "002 " + client + " :Your host is " + servername + " (localhost), running version " + version + "\r\n"
//# define RPL_CREATED(client, datetime) 					"003 " + client + " :This server was created " + datetime + "\r\n"
//# define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) ("004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
#define RPL_MOTDSTART(client, servername) 				"375 " + client + " :- " + servername + " Message of the day - \r\n"
#define RPL_MOTD(client, motd_line) 					"372 " + client + " :" + motd_line + "\r\n"
#define RPL_ENDOFMOTD(client) 							"376 " + client + " :End of /MOTD command.\r\n"

////INVITE - no localhost before RPL_INVITE
//#define RPL_INVITE(nickname, username, target, channel)		(CLIENT_ID(nickname, username) + " INVITE " + target + " " + channel + "\r\n")
//# define RPL_INVITING(CLIENT_ID, client, nick, channel)           "341 " + client + " " + nick + " #" + channel + "\r\n"
//#define ERR_NOTONCHANNEL(source, channel)				      "442 " + source + " " + channel + " :You're not on that channel"
//#define ERR_NOSUCHCHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel"
//#define ERR_USERONCHANNEL(source, target, channel)		      "443 " + source + " " + target + " " + channel + " :is already on channel"

//// JOIN
//#define RPL_JOIN(source, channel)						":" + source + " JOIN :" + channel + "\r\n"
//#define ERR_BADCHANNELKEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)"
//#define ERR_BANNEDFROMCHAN(client, channel) 			      "474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n"

//// KICK
//#define RPL_KICK(source, channel, target, reason)		      ":" + source + " KICK " + channel + " " + target + " :" + reason
//#define ERR_USERNOTINCHANNEL(source, nickname, channel)		"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"

//// KILL
//# define ERR_NOPRIVILEGES(client) 						"481 " + client + " :Permission Denied- You're not an IRC operator\r\n"
//# define RPL_KILL(CLIENT_ID, killed, comment) 		            ":" + CLIENT_ID+ " KILL " + killed + " " + comment + "\r\n"

//// LIST
//#define RPL_LIST(source, channel, nbUsers, topic)		      "322 " + source + " " + channel + " " + nbUsers + " :" + topic
//#define RPL_LISTEND(source)								"323 " + source + " :End of LIST"

//// MODE Channel
//#define ERR_INVITONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)"
//#define ERR_CHANOPRIVSNEEDED(source, channel)			      "482 " + source + " " + channel + " :You're not channel operator"
//#define MODE_CHANNELMSG(channel, mode) 					"MODE #" + channel + " " + mode + "\r\n"
//#define MODE_CHANNELMSGWITHPARAM(channel, mode, param) 	      "MODE #" + channel + " " + mode + " " + param + "\r\n"
//#define RPL_CHANNELMODEIS(client, channel, mode) 		      "324 " + client + " #" + channel + " " + mode + "\r\n"
//#define RPL_CHANNELMODEISWITHKEY(client, channel, mode, password) "324 " + client + " #" + channel + " " + mode + " " + password + "\r\n"
//#define ERR_CANNOTSENDTOCHAN(client, channel) 			      "404 " + client + " #" + channel + " :Cannot send to channel\r\n"
//#define ERR_CHANNELISFULL(client, channel) 				"471 " + client + " #" + channel + " :Cannot join channel (+l)\r\n"
//#define ERR_INVALIDMODEPARAM(client, channel, mode, password)     "696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n"
//// RPL_ERR a broadcoast quand user pas +v ou operator veut parler
//      // dans notre cas c'était tiff (client) qui voulait send a message
//      // :lair.nl.eu.dal.net 404 tiff #pop :Cannot send to channel
//#define RPL_ADDVOICE(nickname, username, channel, mode, param) ":" + nickname + "!" + username + "@localhost MODE #" + channel + " " + mode + " " + param + "\r\n"

//// MODE User
//#define RPL_MODE(source, channel, modes, args)			":" + source + " MODE " + channel + " " + modes + " " + args
//#define MODE_USERMSG(client, mode) 						":" + client + " MODE " + client + " :" + mode + "\r\n"
//#define ERR_UMODEUNKNOWNFLAG(client) 					"501 " + client + " :Unknown MODE flag\r\n"
//#define ERR_USERSDONTMATCH(client) 						"502 " + client + " :Cant change mode for other users\r\n"
//#define RPL_UMODEIS(client, mode) 						"221 " + client + " " + mode + "\r\n"

//// NAMES
//#define RPL_NAMREPLY(source, channel, users)			      "353 " + source + " = " + channel + " :" + users + "\r\n"
//#define RPL_ENDOFNAMES(source, channel)					"366 " + source + " " + channel + " :End of /NAMES list.\r\n"

//// NICK
//#define ERR_NONICKNAMEGIVEN(source)						"431 " + source + " :Nickname not given\r\n"
//#define ERR_NICKNAMEINUSE(source, nickname)				"433 " + source + " " + nickname + " :Nickname is already in use\r\n"
# define RPL_NICK(oclient, uclient, client)                       ":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n"

//// NOTICE - no localhost before reply
//#define RPL_NOTICE(source, target, message)				":" + source + " NOTICE " + target + " :" + message + "\r\n"

//// OPER	
//# define ERR_NOOPERHOST(client) 						"491 " + client + " :No O-lines for your host\r\n"
//# define RPL_YOUREOPER(client) 						"381 " + client + " :You are now an IRC operator\r\n"

//// PART - no localhost before reply
//#define RPL_PART(source, channel)						":" + source + " PART " + channel + "\r\n"
//#define RPL_PART_REASON(source, channel, reason)		      ":" + source + " PART " + channel + " :" + reason + "\r\n"

//// PASS
//#define ERR_PASSWDMISMATCH(source)						"464 " + source + " :Password incorrect"

//// PING - no localhost before reply
//#define RPL_PONG(nickname, username,token)                        (CLIENT_ID(nickname, username) + " PING : " + token + "\r\n")   

//// PRIVMSG - no localhost before reply
//#define RPL_PRIVMSG(nick, username, target, message) 	            ":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n"
# define ERR_NORECIPIENT(client)                                  "411 " + client + " :No recipient given PRIVMSG\r\n"
//# define ERR_NOTEXTTOSEND(client)                                 "412 " + client + " :No text to send\r\n"
////#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message

//// QUIT - no localhost before reply
//#define RPL_QUIT(source, username, message)			      CLIENT_ID(source, username) + " QUIT :" + message

////TOPIC
//#define RPL_NOTOPIC(source, channel)					"331 " + source + " " + channel + " :No topic is set"
//#define RPL_TOPIC(source, channel, topic)				      "332 " + source + " " + channel + " :" + topic

//// USER
//#define ERR_ALREADYREGISTERED(source)					"462 " + source + " :You may not reregister"

//// WHO
//#define RPL_WHOREPLY(source, channel, username, hostname, serverhostname, nickname, realname)	"352 " + source + " " + channel + " " + username + " " + hostname + " " + serverhostname + " " + nickname + " H :0 " + realname
//#define RPL_ENDOFWHO(source, channel)					"315 " + source + " " + channel + " :End of WHO list"
//
#endif