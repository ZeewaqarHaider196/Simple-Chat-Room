#ifndef Server
#define Server
#define WIN32_LEAN_AND_MEAN		//to speed up built process and to exclude some unnecessary files
#define SUCCESS 0 				// Success message code 

#include <windows.h>
#include <winsock2.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>

using namespace std;

class Connection	// This is a connection class and will be used in Server_Socket class to maintain all connected clients
{
	public:
		SOCKET ClientSocket;
		sockaddr_in ClientSocketAddress;
		std::string client_name; 		// Name of person connected to the chat room 
		Connection(const SOCKET ClientSocket, const sockaddr_in ClientSocketAddress);
		
};

class Server_Socket
{
	private:
		WSADATA wsaData;		// struct defined in winsock2.h							
		SOCKET ServerSocket;	// Socket defined for Server to accept the client
		sockaddr_in ServerSocketAddress;	// Socket Address for Server. On which server will be listening
		void SetServerSockAddr(sockaddr_in *sockAddr, int PortNumber);	

		std::map<SOCKET, Connection> connections;	// This is a list of all connected clients
		// read_message from the connection and send the message to other clients connected
		void read_message(Connection c);
		void write_message();
		void add_client_to_room(Connection &c);

		
	public:
		Server_Socket(int RequestVersion);		
		~Server_Socket();
		void start_server(int PortNumber);			// to run the server
		void initiate_chat_room();

};

#endif
