#ifndef SERVER
#define SERVER
#define WIN32_LEAN_AND_MEAN		//to speed up built process and to exclude some unnecessary files
#define SUCCESS 0 				// Success message code 
#define ONE_KB 1024

#include <windows.h>
#include <winsock2.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>

using namespace std;

// This is a connection class and will be used in Server_Socket class to maintain all connected clients
class Connection
{
	public:
		// Socket Id defined for client connection 
		SOCKET ClientSocket;
		// Socket Address of Client
		sockaddr_in ClientSocketAddress;
		// Name of person connected to the chat room 
		std::string client_name;
		
		Connection(const SOCKET ClientSocket, const sockaddr_in ClientSocketAddress);
};

class Server_Socket
{
	private:
		// struct defined in winsock2.h, Information about Windows Sockets
		WSADATA wsaData;									
		// Socket Id defined for Server to accept the client
		SOCKET ServerSocket;	
		// Socket Address for Server, on which server will be listening
		sockaddr_in ServerSocketAddress;	
		// This is a map of all connected clients to store client information
		std::map<SOCKET, Connection> connections;	
		
		// Set Address(IP:PORT) on which server will be listening
		void SetServerSockAddr(sockaddr_in *sockAddr, int PortNumber);	
		// Read message from the connection and send the message to other clients connected to the chat room
		void read_message(Connection c);
		// Send any admin level message to the connected clients(people in the chat room).  
		void write_message();
		// Spawn a thread for new person and to add new person into the connection record  
		void add_client_to_room(Connection &c);
		
	public:
		Server_Socket(int RequestVersion);		
		~Server_Socket();
		// Start the server to listen on provided port 
		void start_server(int PortNumber);
		// Spawn a thread to initiate a chat room and to add any new person into the chat room.  
		void initiate_chat_room();
};

#endif
