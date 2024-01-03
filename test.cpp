#include <iostream>
#include <string>
#include <vector>
#include <poll.h>

class Server {
public:
    void acceptConnexions(void);

private:
    // ... (autres membres)
    std::string _servInput;
};

void Server::acceptConnexions(void)
{
    // ... (autre code)

    struct pollfd fds[2]; // On surveille stdin (0) et _socServ (par exemple)
    int timeout = 100;    // Délai en millisecondes

    fds[0].fd = STDIN_FILENO; // stdin
    fds[0].events = POLLIN;

    fds[1].fd = _socServ;     // Votre socket server (_socServ)
    fds[1].events = POLLIN;

    while (1)
    {
        int pollResult = poll(fds, 2, timeout);

        if (pollResult > 0)
        {
            // L'événement est prêt à être traité

            if (fds[0].revents & POLLIN)
            {
                // L'entrée standard (stdin) est prête à être lue
                std::getline(std::cin, _servInput);
                // Traiter l'entrée utilisateur
                // ...
            }

            if (fds[1].revents & POLLIN)
            {
                // Votre socket server (_socServ) a des données à lire
                // Traiter les données du client
                // ...
            }
        }
        else if (pollResult < 0)
        {
            // Gestion d'erreur
            // ...
        }

        // Continuer à accepter des connexions clientes
        // ...
    }

    // ... (fermer le socket serveur et autres opérations de nettoyage)
}

int main() {
    Server server;
    server.acceptConnexions();
    return 0;
}
