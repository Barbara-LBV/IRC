/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 17:44:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/05 16:57:26 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Pour gerer les evenements 
L'utilisation de kqueue et struct kevent permet une gestion efficace des 
entrées/sorties événementielles, permettant aux applications de surveiller 
plusieurs descripteurs de fichiers et d'autres événements système en utilisant 
une seule interface. Cela est particulièrement utile pour les serveurs réseau 
et d'autres applications nécessitant une gestion efficace des événements.*/

/*#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>
#include "Server.hpp"
#include "Client.hpp"
#include "IrcLib.hpp"

class Client;

class Event 
{
    private:
        Event(const Event&);
        Event& operator=(const Event&);
        int    _kqueue;
        int    _count; //evnt's id
        void   setEventStruct(int id, short filt, u_short flags, u_int fflags, int data, void*   udata);
        
    protected:
        Event();
        ~Event();
        struct kevent _events[EVENTS_MAX];
        int           _index;
        void          initialize(int socket_fd);
        void          remove(int fd);
        void          add(Client*);
        int           kevent();
}

#endif*/

/*struct kevent 
        {
            uintptr_t ident;    // Identificateur de l'objet (par exemple, le descripteur de fichier ou le numéro de processus)
            short     filter;   // Type d'événement à surveiller (par exemple, EVFILT_READ pour la lecture)
            u_short   flags;    // Drapeaux décrivant le comportement de l'événement
            u_int     fflags;   // Filtre spécifique défini par l'utilisateur (dépend du type de filtre)
            intptr_t  data;     // Données associées à l'événement (par exemple, le nombre d'octets disponibles à la lecture)
            void      *udata;   // Pointeur utilisateur associé à cet événement (valeur définie par l'utilisateur)
        };
}*/