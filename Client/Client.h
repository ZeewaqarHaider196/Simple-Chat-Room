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
		Client_Socket(int RequestVersion);		//one argument constructor
		~Client_Socket();							//destructor
		void ConnectServer(const char* server_ip, const int server_port);			// function to connect to the server
		void initiate_chat_room();
	private:
		WSADATA wsaData;			//struct defined in WinSock2.h
		SOCKET ClientSocket;				//struct defined in WinSock2.h
		sockaddr_in ClientSocketAddress;		//windows extended data type "
		void SetClientSockAddr(const char* server_ip, const int server_port);			// to fill the sockAddr structure
		void message_send();
		void message_read();
};
#endif
