/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 11:52:09 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/16 15:48:23 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// for all the different automatic responses server and client have to send
// http://chi.cs.uchicago.edu/chirc/irc_examples.html
// http://chi.cs.uchicago.edu/chirc/irc_examples.html#logging-into-an-irc-server
// https://gist.github.com/proxypoke/2264878

#ifndef REPLIES_HPP
# define REPLIES_HPP

/*********************   ERRORS   **********************/
//If a user tries to register with a nick that is already taken, code 433
# define ERR_NICKNAMEINUSE (username, wrong_nickname) (":localhost 001 " + username + " : " + nickname + " is already in use.\r\n")

# define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " " + command + " :Unknown command\r\n")

# define ERR_FULL_SERV "[Server] You cannot join, the server is already full"

/*********************   NUMERIC REPLIES   **********************/
// server reply to a successfull client connection
# define RPL_WELCOME(user_id, nickname)(":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")



#endif