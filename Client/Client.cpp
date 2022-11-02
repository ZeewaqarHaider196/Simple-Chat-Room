#include "Client.h"
Client_Socket::Client_Socket(int RequestVersion)		// Constructor call
{
	ClientSocket = INVALID_SOCKET;
	int ret = WSAStartup(MAKEWORD(RequestVersion, 0), &wsaData);
	if (ret == SUCCESS)
	{
		if(LOBYTE(wsaData.wVersion < RequestVersion))
		{
			throw std::runtime_error("Requested version is not available.");
		}
	}
	else
		throw std::runtime_error("Startup failed! WSAStartup failed. Error Code: " + std::to_string(ret));
}
Client_Socket::~Client_Socket()		
{
	WSACleanup();

	if(ClientSocket != INVALID_SOCKET)	// closing Client Socket 
		closesocket(ClientSocket);
}
void Client_Socket::SetClientSockAddr(const char* server_ip, const int server_port)
{
    ClientSocketAddress.sin_addr.S_un.S_addr = inet_addr(server_ip);
	ClientSocketAddress.sin_family = AF_INET;						// Use TCP/IP protocol
	ClientSocketAddress.sin_port = htons(server_port);
}
void Client_Socket::ConnectServer(const char* server_ip, const int server_port)
{
	SetClientSockAddr(server_ip, server_port);		
	
	if ((ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		throw std::runtime_error("Could not create socket.");
	
	cout << "Attempting to connect to Server..." << endl;
	
	if (connect(ClientSocket, (sockaddr*)(&ClientSocketAddress), sizeof(ClientSocketAddress)) != SUCCESS)		// Connect to the server
		throw std::runtime_error("Could not connect");

	this->initiate_chat_room();
}

void Client_Socket::initiate_chat_room()
{
	cout << "you are added to chat room" << endl << endl;	
	cout << "please enter your name: ";
	
	char *name = new char[NAME_LEN];	
	cin.getline(name, NAME_LEN, '\n');
	
	// sending your name to server.
	if(send(ClientSocket, name, strlen(name), 0) == SOCKET_ERROR)
	{	
		cout << "Server probably down. Unable to initiate chat room. :(" << endl;
		return;
	}

	std::thread thread_for_message_read(Client_Socket::message_read, this);
	thread_for_message_read.detach();
	Sleep(500); // sleep for 500ms
	std::thread thread_for_message_send(Client_Socket::message_send, this);
	thread_for_message_send.detach();
} 

void Client_Socket::message_read()
{
	char *message_buffer = new char[ONE_KB];	// 1 KB message size at max.
	bool disconnect = false;
	int message_size = 0;
	while(!disconnect)
	{
		message_size = recv(ClientSocket, message_buffer, ONE_KB, 0);
		if(message_size <= 0)
		{
			cout << "The connection is closed, reason: " << message_size << ", server down" << endl;
			disconnect = true;
		}
		message_buffer[message_size] = 0;
		cout << "\r\r\r\r\r" << message_buffer << endl; // \r is moving one char backward
		cout << "You: ";
	}
}

void Client_Socket::message_send()
{
	char *message_buffer = new char[ONE_KB];	// 1 KB message size at max.
	bool disconnect = false;
	while(!disconnect)
	{
		cout << "You: ";
		cin.getline(message_buffer, ONE_KB, '\n');
		if(send(ClientSocket, message_buffer, strlen(message_buffer), 0) == SOCKET_ERROR)
		{	
			cout << "Server probably down. " << endl;
			disconnect = true;
		}
	}
}


