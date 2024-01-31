/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 11:52:09 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/31 14:20:52 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// for all the different automatic responses server and client have to send
// http://chi.cs.uchicago.edu/chirc/irc_examples.html
// http://chi.cs.uchicago.edu/chirc/irc_examples.html#logging-into-an-irc-server
// https://gist.github.com/proxypoke/2264878

#ifndef REPLIES_HPP
# define REPLIES_HPP

#define CLIENT_ID(nickname, username, host, command)(":" + nickname + "!" + username + "@" + host + " " + command + " ")

/*********************   ERRORS   **********************/

#define ERR_NEEDMOREPARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters"
#define ERR_NOTONCHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANNELISFULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_BADCHANNELKEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_INVITONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)"
#define ERR_NOSUCHCHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel"
#define ERR_CHANOPRIVSNEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator"
#define ERR_NONICKNAMEGIVEN(source)						"431 " + source + " :Nickname not given"
#define ERR_NICKNAMEINUSE(source, nickname)				"433 " + source + " " + nickname + " :Nickname is already in use"
#define ERR_ALREADYREGISTERED(source)					"462 " + source + " :You may not reregister"
#define ERR_PASSWDMISMATCH(source)						"464 " + source + " :Password incorrect"
#define ERR_NOTREGISTERED(source)						"451 " + source + " :You have not registered"
#define ERR_UNKNOWNCOMMAND(source, command)				"421 " + source + " " + command + " :Unknown command"
#define ERR_USERONCHANNEL(source, target, channel)		"443 " + source + " " + target + " " + channel + " :is already on channel"
#define ERR_NOSUCHNICK(source, name)					"401 " + source + " " + name + " :No such nick/channel"
#define ERR_INVITEONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)"

/*********************   NUMERIC REPLIES   **********************/

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + " (localhost), running version " + version + "\r\n")
# define RPL_CREATED(client, datetime) (":localhost 003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
#define RPL_MOTDSTART(client, servername) (":localhost 375 " + client + " :- " + servername + " Message of the day - \r\n")
#define RPL_MOTD(client, motd_line) (":localhost 372 " + client + " :" + motd_line + "\r\n")
#define RPL_ENDOFMOTD(client) (":localhost 376 " + client + " :End of /MOTD command.\r\n")
#define NICK(nickname, username, new_nickname) (CLIENT_ID(nickname, username, "localhost","NICK") + ":" + new_nickname + "\r\n")

#define RPL_NAMREPLY(source, channel, users)			"353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)					"366 " + source + " " + channel + " :End of /NAMES list."
#define RPL_INVITING(source, channel, target)			"341 " + source + " " + channel + " " + target

#define RPL_NOTOPIC(source, channel)					"331 " + source + " " + channel + " :No topic is set"
#define RPL_TOPIC(source, channel, topic)				"332 " + source + " " + channel + " :" + topic

#define RPL_WHOREPLY(source, channel, username, hostname, serverhostname, nickname, realname)	"352 " + source + " " + channel + " " + username + " " + hostname + " " + serverhostname + " " + nickname + " H :0 " + realname
#define RPL_ENDOFWHO(source, channel)					"315 " + source + " " + channel + " :End of WHO list"

#define RPL_LIST(source, channel, nbUsers, topic)		"322 " + source + " " + channel + " " + nbUsers + " :" + topic
#define RPL_LISTEND(source)					"323 " + source + " :End of LIST"

/*********************   COMMAND REPLIES    **********************/

#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args
#define RPL_PING(source, token)						":" + source + " PONG :" + token
#define RPL_QUIT(source, message)					":" + source + " QUIT :" + message
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)					":" + source + " PART " + channel
#define RPL_PART_REASON(source, channel, reason)	":" + source + " PART " + channel + " :" + reason
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason
#define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")
// #define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)			":" + source + " NOTICE " + target + " :" + message
#define RPL_INVITE(source, target, channel)			":" + source + " INVITE " + target + " :" + channel


#endif