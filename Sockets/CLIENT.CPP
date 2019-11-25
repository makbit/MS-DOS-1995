#include "socket.h"
#include <stdlib.h>

int main(int argc, char * argv[])
{
	printf("----======= CLIENT =========---------\n");
	WSAInitializer winsock;
	if( !winsock ) return -1;
	int port = (argc==1) ? IPPORT_ECHO : atoi(argv[1]);
	Socket client(0, TRUE);
	if( !client ) return -1;
	print_sockname(client);

	char buffer[128];
	printf("Input server IP address: ");
	scanf("%s", buffer);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(buffer);
	if(	connect(client, (sockaddr *)&addr, sizeof(addr))==SOCKET_ERROR )
	{
		WSAShowLastError(0);
		printf("Failed to connect\n");
		return -1;
	}
	int n = 0;
	while( n!=27 ) // not escape
	{
		if( kbhit() )
		{
			n = getch();
			if( send(client, (char *)&n, 4, MSG_DONTROUTE)==SOCKET_ERROR )
			{
				WSAShowLastError(0);
				printf("Failed to send");
				break;
			}
		}
		u_long size = 0;
		ioctlsocket(client, FIONREAD, &size);
		while( size ) 
		{
			char ch;
			recv(client, &ch, 1, 0);
			putch(ch);
			if( ch=='\r' ) putch('\n');
			size--;
		}
	}
	shutdown(client, SD_SEND);
	return 0;
}

