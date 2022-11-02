# Simple-Chat-Room
The repo contains a Multi-Threaded Simple Chat Room developed in C/C++ using TCP/IP protocol of socket programming.
This project is created for educational purpose to help understand the concept of,
1. Socket Programming 
2. TCP/IP protocol
3. Server-Client Applications
4. Multithreading Programming 

There will two separate programs.
1. Server
2. Clients

*** Server *** 

Server is a program which will run and wait for the clients to connect to it. After a client connect with server, it will be added in the list of clients.

Any message received from any client will be sent to all other clients present in the client list.

*** Client *** 

Client is a program which will run and connect with the server. After successfull connection established with server, client is able to sent message to the room.

# IDE used 
VS Code: Both client and server programs built and tested on VS Code.

# Libraries 
please link the following libraries before building the programs.
-lwsock32
-lws2_32
-lpthread

# Compiler 
GCC 10.3.0

# Command to Build Server Program
g++.exe -g Server.h Server.cpp main.cpp -L<PATH_TO_LIB_FOLDER> -lwsock32 -lws2_32 -lpthread -o Server.exe