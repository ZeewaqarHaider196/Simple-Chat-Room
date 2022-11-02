#ifndef Client
#define Client
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#define SUCCESS 0
#define ONE_KB 1024
#define NAME_LEN 50
using namespace std;
class Client_Socket
{
	public:
		Client_Socket(int RequestVersion);		
		~Client_Socket();
		// Connect to the server running on address(server_ip, server_port)
		void ConnectServer(const char* server_ip, const int server_port);			
		// spawn threads for receiving/sending messages from/to the chat room
		void initiate_chat_room();
	private:
		// struct defined in winsock2.h, Information about Windows Sockets
		WSADATA wsaData;
		// Socket Id defined for Client
		SOCKET ClientSocket;				
		// Socket Address of Server, on which client will connect
		sockaddr_in ClientSocketAddress;	
		
		// Fill the sockAddr structure
		void SetClientSockAddr(const char* server_ip, const int server_port);			
		// Send message to chat room
		void message_send();
		// Receive message from chat room
		void message_read();
};
#endif
