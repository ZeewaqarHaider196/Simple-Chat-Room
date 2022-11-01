# Simple-Chat-Room
The repo contains the Simple Chat Room in C/C++ using socket programming.
There will two separate projects.
1. Server
2. Clients

***** Server ***** 
Server is a program which will run and wait for the clients to connect to it. After a client connect with server, it will be added in the list of clients.

Any message received from any client will be sent to all other clients present in the client list.

***** Client ***** 
Client is a program which will run and connect with the server. After successfull connection established with server, client is able to sent message to the room.
