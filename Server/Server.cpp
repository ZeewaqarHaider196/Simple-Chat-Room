#include "Server.h"


Connection::Connection(SOCKET ClientSocket, sockaddr_in ClientSocketAddress)
{
    this->ClientSocket = ClientSocket;
    this->ClientSocketAddress = ClientSocketAddress;    
	this->client_name = std::to_string(ClientSocket);
}

Server_Socket::Server_Socket(int RequestVersion)		
{
	ServerSocket = INVALID_SOCKET;
	int ret = WSAStartup(MAKEWORD(RequestVersion, 0), &wsaData); 
	if(ret == SUCCESS)		// Check required version
	{
		if(LOBYTE(wsaData.wVersion < RequestVersion))
		{
			throw std::runtime_error("Requested version is not available.");
		}
	}
	else
		throw std::runtime_error("Startup failed! WSAStartup failed. Error Code: " + std::to_string(ret));
}

Server_Socket::~Server_Socket()
{
	WSACleanup();
	
	// Closing all client Sockets, if any remains
	for (auto i : connections)
	{
		if (i.first != INVALID_SOCKET)
			closesocket(i.first);	
	}
	
	if (ServerSocket != INVALID_SOCKET)
		closesocket(ServerSocket);
}

void Server_Socket::SetServerSockAddr(sockaddr_in *ServerSocketAddress, int PortNumber)
{
	ServerSocketAddress->sin_family = AF_INET;
	ServerSocketAddress->sin_port = htons(PortNumber);
	ServerSocketAddress->sin_addr.S_un.S_addr = INADDR_ANY;			// Listen on host IP
}

void Server_Socket::start_server(int PortNumber)
{	
	// Create server socket
	if((ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		throw std::runtime_error("Could not create socket.");
	SetServerSockAddr(&ServerSocketAddress, PortNumber);	
	
    // Bind the created socket with Socket Address
	if(bind(ServerSocket, (sockaddr*)(&ServerSocketAddress), sizeof(ServerSocketAddress)) != SUCCESS)
		throw std::runtime_error("Could not bind socket.");
	
	// Put the Server Socket on listening. It will accept SOMAXCON
    if(listen(ServerSocket, SOMAXCONN) != SUCCESS)			//SOMAXCONN is maximum no. of connections
		throw std::runtime_error("Could not put the socket in listening mode.");
	
   	cout << "Server is started." << endl; 
	cout << "Address (" << inet_ntoa(ServerSocketAddress.sin_addr) << ":" << ntohs(ServerSocketAddress.sin_port) << ")" << endl;
}

void Server_Socket::initiate_chat_room()
{
	std::thread init_thread([&]()	
		{	// lambda funtion to handle the new connection request 
			while (true)
			{
				SOCKET ClientSocket;
				sockaddr_in ClientAddr;
				int SizeAddr = sizeof(ClientAddr);
				ClientSocket = accept(ServerSocket, (sockaddr *)(&ClientAddr), &SizeAddr);			// Accepting the new client(person) in chat room
				Connection c(ClientSocket, ClientAddr);
				this->add_client_to_room(c);
			}	
		}
	);
	// detaching the thread to run independently.
	init_thread.detach();
	cout << "Chat room has been created." << endl; 
} 

void Server_Socket::add_client_to_room(Connection &c)
{
    std::thread t(&Server_Socket::read_message, this, c);
	t.detach();
	connections.insert(std::pair<SOCKET, Connection>(c.ClientSocket, c));
}

void Server_Socket::read_message(Connection c)
{
    bool disconnect = true;
	SOCKET ClientSocket = c.ClientSocket;
	std::string broadcast_msg = "";
    char message_buffer[ONE_KB];
	// Getting Name of Client, that will be send by client Application.
	int message_size = recv(ClientSocket, message_buffer, sizeof(message_buffer), 0);
	if(message_size > 0)		
	{
		message_buffer[message_size] = 0; 
		c.client_name = std::string(message_buffer);
		disconnect = false;		
	}		
	else
	{
		cout << "SERVER MSG: " << c.client_name << " is unable to Start Communication" << endl;		
	}
	// Starting communication loop	
	while (!disconnect)
    {
		message_size = recv(ClientSocket, message_buffer, sizeof(message_buffer), 0);
        if(message_size == 0)		
        {
            disconnect = true;		
            cout << "SERVER MSG: " << c.client_name << ": closed the connection." << endl;		
        }		
        else if(message_size == SOCKET_ERROR)		
        {				
            disconnect = true;
            cout << "SERVER MSG: " << c.client_name << ": seems to be offline." << endl;		
        }		
        else		
        {		
            message_buffer[message_size] = 0;
            cout << c.client_name << ": " << message_buffer << endl;			
			// sending message to other people in the chat room
			broadcast_msg = c.client_name + ": " + std::string(message_buffer);
			for (auto i : connections)
			{
				if (i.first != ClientSocket)
				{
					if(send(i.first, broadcast_msg.c_str(), broadcast_msg.length(), 0) == SOCKET_ERROR)
						cout << "SERVER MSG: " << "Forwarding message to socket: [" << i.first << " " << i.second.client_name << "] failed" << endl;
				}
			}
		}	       
    }
	// removing the socket of disconnected person from chat room
	connections.erase(ClientSocket);
	// closing the connection with client socket from my side(server)
	closesocket(ClientSocket);
	std::cout << "SERVER MSG: " << c.client_name << ": removed from chat room." << endl;
}

void Server_Socket::write_message()
{
	// This method is not implemented. It will be used for sending server side notifications to all clients.
}
