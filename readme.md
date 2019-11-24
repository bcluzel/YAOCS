# Projet YAOCS : Yet And Other Chat System

## Features

YAOCS est un systéme de chat via pipes.
- Il permet de dialoguer en méme temps avec un nombre théoriquement infini d'utilisateur (Limité par la taille de ram libre sur le pc) puis qu'il gére dynamiquement les utilisateurs connectés.
- Le client/serveur peut recevoir une taille de message théoriquement infini.
- Le nom de d'utilisateur et l'heure précise d'envoi d'un message est visible à l'écran.
- Le client peut accéder à plusieurs commandes : 
    - /help - liste les commandes
    - /changename "nom" - Change le nom du client 
    - /users - liste les utilisateurs connectés
    - /msg "nomClient" "message" - envoi un message privé à "nomClient" [NON IMPLÉMENTÉ]
    - /exit - quitte YAOCS


## Structure du projet
Le projet est divisé en plusieurs dossiers :
- server, le fichier contenant le code et le makefile du coté serveur
- client, le fichier contenant le code et le makefile du coté client
- libs, le fichier contenant le code commun au client et server
- report, le fichier contenant le rapport généré par doxygen

## Compilation
Pour compiler, placez-vous dans la racine du projet et executez ./makeall.sh.

## Execution 
Pour lancer le chat, placez-vous dans la racine du projet et executez ./YAOCS_client dans un shell sur un systéme type unix.
YAOCS_client lancera automatiquement YAOCS_server en daemon.

## En cas de bug
Si les serveurs et clients s'arrétent d'une manière non conventionelle, il faut suprimer les fichiers temporaires (/tmp/lock et /tmp/chat).
