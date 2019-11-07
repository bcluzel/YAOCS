# Projet YAOCS : Yet And Other Chat System

Pour compiler utiliser le makeall.sh

Le projet est divisé en trois dossiers :
- server
- client
- libs pour le code commun au client et server



## Connection client - server

CLIENT demande de connection au SERVER (hello()) avec un id $id
CLIENT envoie un message avec son fildes
SERVER répond CLIENT_ACCESS_GRANTED si $id est ok pour SERVER
Sinon SERVER répond ERR_CLIENT_ID id $id est déjà utilisée


## Lists of commands

 - CMD_SERVER 0x2 => following paramters is a server command


 - FILE_DESCRIPTOR_TX 0x1 => used to transmit client fildes during initialisation protocol