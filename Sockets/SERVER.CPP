#include "socket.h"
#include <stdlib.h>

#define MAX_CLIENTS   2
static int nClients = 0;
static Connection* apClients[MAX_CLIENTS];
static void CloseConnections();

//--------------------------------------------------------------------------//
int main(int argc, char *argv[])
{
	int err;
	printf("----======= SERVER =========---------\n");
	WSAInitializer winsock;
	if( !winsock ) return -1;
	int port = (argc==1) ? IPPORT_ECHO : atoi(argv[1]);
	Socket server(port, TRUE);
	if( !server ) return -1;
	print_sockname(server);
	while( nClients < MAX_CLIENTS )
	{
		if( kbhit() && getch()==27 )
		{
			CloseConnections();
			return 0;
		}
		listen(server, SOMAXCONN);
		SOCKET client = accept(server, 0, 0);
		if( check(client, "accept") ) return -1; //continue;
		printf("\n Client connected!!!\n");
		print_sockname(client);

		char *prompt = "Hello, this is cos-server!\n";
		send(client, prompt, strlen(prompt)+1, 0);

		apClients[nClients++] = new Connection(client);
	}
	printf("---=== No more connections accepted! ===---\n");
	while( !(kbhit() && getch()==27) )
	{
		Sleep(10);
	}
	CloseConnections();
	return 0;
}

//--------------------------------------------------------------------------//
void CloseConnections()
{
	printf("---=== Closing all connections! ===---\n");
	for(int i=nClients; i>0; i--) delete apClients[i-1];
}
