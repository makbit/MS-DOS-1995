#include <windows.h>
#include <nspapi.h>
#include <stdio.h>
#include "error.c"

void main(void)
{
	PROTOCOL_INFO protocols[8];
    DWORD dwCount = sizeof(protocols);
    if( EnumProtocols(0, protocols, &dwCount)==SOCKET_ERROR ) ShowLastError(0);
	dwCount /= sizeof(PROTOCOL_INFO);
    printf("Number of protocols:  %d", dwCount);
	for(int i=0; i<dwCount; i++)
	{
		printf("\n-----------------------------------------------\n");
		printf("#%d  Name:   %s \n", i, protocols[i].lpProtocol);
		printf("ServiceFlag: %d \n", protocols[i].dwServiceFlags);
		printf("AddrFam:     %d \n", protocols[i].iAddressFamily);
		printf("MaxAddr:     %d \n", protocols[i].iMaxSockAddr);
		printf("MinAddr:     %d \n", protocols[i].iMinSockAddr);
		printf("SocketType:  %d \n", protocols[i].iSocketType);
		printf("iProtocol:   %d \n", protocols[i].iProtocol);
	}
}
