#include "Server.h"
#include <iostream>
using namespace std;
#define REQ_WINSOCK_VER 2
#define LISTENING_PORT 27014
int main()
{
	try
	{
		Server_Socket MyServer(REQ_WINSOCK_VER);
		MyServer.start_server(LISTENING_PORT);
		MyServer.initiate_chat_room();
		while(true)
		{
			Sleep(500);
		}
	}
	catch(std::exception &ex)
	{
		cout << "\nError: " << ex.what();
	}
	return 0;
}
