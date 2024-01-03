/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 17:44:27 by blefebvr          #+#    #+#             */
/*   Updated: 2024/01/03 17:47:43 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Pour gerer les evenements 
L'utilisation de kqueue et struct kevent permet une gestion efficace des 
entrées/sorties événementielles, permettant aux applications de surveiller 
plusieurs descripteurs de fichiers et d'autres événements système en utilisant 
une seule interface. Cela est particulièrement utile pour les serveurs réseau 
et d'autres applications nécessitant une gestion efficace des événements.*/

#ifndef EVENT_HPP
#define EVENT_HPP

#include <sys/event.h>
#include <sys/types.h>
#include "client.hpp"

# define DEFAULT "\001\033[0;39m\002"
# define RED "\001\033[1;91m\002"
# define YELLOW "\001\033[1;93m\002"
# define MAGENTA "\001\033[1;95m\002"
# define GRAY "\001\033[1;89m\002"
# define GREEN "\001\033[1;92m\002"
# define BLUE "\001\033[1;36m\002"

struct kevent {
    uintptr_t ident;    // Identificateur de l'objet (par exemple, le descripteur de fichier ou le numéro de processus)
    short     filter;   // Type d'événement à surveiller (par exemple, EVFILT_READ pour la lecture)
    u_short   flags;    // Drapeaux décrivant le comportement de l'événement
    u_int     fflags;   // Filtre spécifique défini par l'utilisateur (dépend du type de filtre)
    intptr_t  data;     // Données associées à l'événement (par exemple, le nombre d'octets disponibles à la lecture)
    void      *udata;   // Pointeur utilisateur associé à cet événement (valeur définie par l'utilisateur)
};


#endif