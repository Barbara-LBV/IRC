/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 11:52:09 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/05 14:04:24 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// for all the different automatic responses server and client have to send
// http://chi.cs.uchicago.edu/chirc/irc_examples.html

#ifndef REPLIES_HPP
# define REPLIES_HPP

// server reply to a successfull client connection
# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
//If a user tries to register with a nick that is already taken, code 433
# define ERR_NICKNAMEINUSE (username, wrong_nickname) (":localhost 001 " + username + " : " + nickname + " is already in use.\r\n")

# define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " " + command + " :Unknown command\r\n")

# define ERR_FULL_SERV "[Server] You cannot join, the server is already full"

#endif