# ft_irc

FT_IRC est un serveur IRC implémenté en C++ suivant la norme C++98. <br>

Ce projet a été conçu pour gérer la communication en temps réel entre clients IRC, <br>
en utilisant TCP/IP v4 pour les connexions réseau. <br>
<br>
Le projet se concentre sur la création d'un serveur robuste capable de gérer plusieurs connexions<br> simultanées sur un même port sans recourir directement à la création de nouveaux processus (forking).<br>

# how does it work ? : 

    ./ircserv <port> <password>

port : Le numéro du port sur lequel votre serveur acceptera les connexions entrantes. <br>
<br>
password : Le mot de passe permettant de s’identifier auprès de votre serveur IRC, <br>
et qui devra être fourni par tout client IRC souhaitant s’y connecter. <br>

# faculties : 

Gestion de multiples connexions simultanées sur le même port. <br>
Utilisation d'une seule instruction select pour la multiplexation des E/S. <br>
Le client IRC Irssi est utilisé comme référence pour le développement et les tests. <br>
Connexion TCP/IP v4. <br>
Support pour définir un pseudonyme (nickname) et un utilisateur (user). <br>
Capacité à rejoindre des canaux (channels) et à envoyer/recevoir des messages privés. <br>
Gestion correcte des canaux. <br>



# TODO         
changer le mode des fctnl <br>
changer le mode afficher par un user lorsquil join <br>
Tester <br>






