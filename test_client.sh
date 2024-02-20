# /**
#  * @file test_client.sh
#  * @brief Test du client IRC
#  */

#!/bin/sh

# Fonction pour envoyer une commande avec un délai
send_command_with_delay() {
    sleep 1 # Délai d'une seconde entre chaque envoi de commande
    echo "$1\r\n" 
}
send_command_with_delay_less() {
    sleep 0.2 # Délai d'une seconde entre chaque envoi de commande
    echo "$1\r\n" 
}

# Connexion au serveur IRC
{
send_command_with_delay "PASS a"
send_command_with_delay "NICK client"
send_command_with_delay "USER client 8 * :client"
send_command_with_delay "JO #test"
send_command_with_delay "JOIN #a"

# bug parsing 

send_command_with_delay "PRIVMSG #a :Hello World"
send_command_with_delay "QUIT"

} | nc localhost 6667 > test_client.log
