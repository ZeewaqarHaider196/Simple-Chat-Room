#include "Client.h"
#include <iostream>
using namespace std;
#define REQ_WINSOCK_VER	2
#define PORT 27014
#define IP "127.0.0.1"
int main(int argc, char *argv[])
{
	try
	{ 
		Client_Socket MyClient(REQ_WINSOCK_VER);
		MyClient.ConnectServer(IP, PORT);
		while (true)
		{
			Sleep(1000);
		}
	}
	catch(std::exception &ex)
	{
		cout << "\nError: " << ex.what();
	}
	return 0;
}
