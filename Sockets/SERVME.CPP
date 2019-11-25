#include "socket.h"
#include <stdlib.h>

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

	listen(server, SOMAXCONN);
	SOCKET client = accept(server, 0, 0);
	if( check(client, "accept") ) return -1;
	printf("\n Client connected!!!\n");
	print_sockname(client);

	char *prompt = "Hello, this is cos-server!\n";
	send(client, prompt, strlen(prompt)+1, 0);
	int ch = 0;
	while( ch!=27 ) // !ESC
	{
		u_long size = 0;
		ioctlsocket(client, FIONREAD, &size);
		while( size ) 
		{
			recv(client, (char *)&ch, 1, 0);
			putch(ch);
			if( ch=='\r' ) putch('\n');
			size--;
		}
		if( kbhit() )
		{
			ch = getche();
			send(client, (char *)&ch, 4, 0);
		}
	}
	shutdown(client, SD_SEND);
	closesocket(client);
	return 0;
}

//--------------------------------------------------------------------------//
