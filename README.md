# ft_irc

FT_IRC est un serveur IRC implémenté en C++ suivant la norme C++98. Ce projet a été conçu pour gérer la communication en temps réel entre clients IRC, en utilisant TCP/IP v4 pour les connexions réseau. Le projet se concentre sur la création d'un serveur robuste capable de gérer plusieurs connexions simultanées sur un même port sans recourir directement à la création de nouveaux processus (forking).

# how does it work ? : 

    ./ircserv <port> <password>

    port : Le numéro du port sur lequel votre serveur acceptera les connexions entrantes.
    password : Le mot de passe permettant de s’identifier auprès de votre serveur
        IRC, et qui devra être fourni par tout client IRC souhaitant s’y connecter.

# faculties : 

    Gestion de multiples connexions simultanées sur le même port.
    Utilisation d'une seule instruction select pour la multiplexation des E/S.
    Le client IRC Irssi est utilisé comme référence pour le développement et les tests.
    Connexion TCP/IP v4.
    Support pour définir un pseudonyme (nickname) et un utilisateur (user).
    Capacité à rejoindre des canaux (channels) et à envoyer/recevoir des messages privés.
    Gestion correcte des canaux.



# TODO 
    Operateur                     
    Commande operateur :          
    Different mode de channel     
    changer le mode des fctnl
    Tester






