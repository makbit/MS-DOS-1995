#include "socket.h"

//--------------------------------------------------------------------------//
Socket::Socket(int port, int nReuseAddr):s(INVALID_SOCKET)
{
	gethostname(szHostName, sizeof(szHostName));
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( s==INVALID_SOCKET )
	{
		WSAShowLastError(0);
		printf("Unable to use TCP/IP protocol!\n");
		return;
	}
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	hostent *he = gethostbyname(szHostName);
	if( !he )
    {
		printf("Can't get IP address of the computer");
		return;
	}
	sa.sin_addr.s_addr = ((in_addr *)he->h_addr_list[0])->s_addr;
	printf("h_addrtype: %d, h_length:  %d, IP address: %s  \n", he->h_addrtype, he->h_length, inet_ntoa(sa.sin_addr));

	check( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&nReuseAddr, sizeof(int)), "setsockopt" );
	if( check( bind(s, (sockaddr *)&sa, sizeof(sa)), "bind" ) )
	{
		closesocket(s);
		s = INVALID_SOCKET;
		return;
	}
}

//--------------------------------------------------------------------------//
Socket::~Socket()
{
	if( s!=INVALID_SOCKET )
	{
		shutdown(s, SD_BOTH);
		closesocket(s);
	}
}

//--------------------------------------------------------------------------//
WSAInitializer::WSAInitializer():bInit(false),bError(true)
{
    DWORD dwCount = 0;
    EnumProtocols(0, 0, &dwCount);
	if( dwCount==0 ) 
	{
		printf("Network software not installed!\n");
		return;
	}
	// init WinSock.dll
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(1,1), &wsaData);
	if( err!=0 ) 
	{
		printf("Windows Sockets not installed\n");
		return;
	}
	bInit = true;
	// check it's version
	if( LOBYTE(wsaData.wVersion)!=1 || HIBYTE(wsaData.wVersion)!=1 )
	{
		printf("Windows Sockets DLL 1.1 not found\n");
		return;
	}
    bError = false;
}

WSAInitializer::~WSAInitializer()
{
	if( bInit ) WSACleanup();
}



//----------------------------------------------------------------------------//
extern "C" void ShowLastError(HWND hwnd)
{
	LPSTR lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  (LPSTR)&lpMsgBuf, 0, NULL );
	// Display the string.
	MessageBox( hwnd, lpMsgBuf, NULL, MB_OK|MB_SYSTEMMODAL );
	// Free the buffer.
	LocalFree( (LPVOID)lpMsgBuf );
}


//----------------------------------------------------------------------------//
extern "C" void WSAShowLastError(HWND hwnd)
{
  LPSTR pErr = "Unknown network error";
  switch( WSAGetLastError() )
  {
    case WSANOTINITIALISED: pErr = "WSANOTINITIALISED: WinSock did not initialized."; break;
    case WSAENETDOWN:       pErr = "WSAENETDOWN: The network subsystem has failed."; break;
    case WSAEADDRINUSE:     pErr = "WSAEADDRINUSE: The specified address is already in use."; break;
    case WSAEINTR:          pErr = "WSAEINTR: The (blocking) call was canceled."; break;
    case WSAEINPROGRESS:    pErr = "WSAEINPROGRESS: A blocking WinSock 1.1 call is in progress, or the service provider is still processing a callback function."; break;
    case WSAEALREADY:       pErr = "WSAEALREADY: A nonblocking connect call is in progress on the specified socket."; break;
    case WSAEADDRNOTAVAIL:  pErr = "WSAEADDRNOTAVAIL: The specified address is not available from the local machine."; break;
    case WSAEAFNOSUPPORT:	pErr = "WSAEAFNOSUPPORT: Addresses in the specified family cannot be used with this socket."; break;
    case WSAECONNREFUSED:   pErr = "WSAECONNREFUSED: The attempt to connect was forcefully rejected."; break;
    case WSAEFAULT:         pErr = "WSAEFAULT: The name or the namelen argument is not a valid part of the user address space."; break;
    case WSAEINVAL:         pErr = "WSAEINVAL: The parameter s is a listening socket, or the destination address ..."; break;
    case WSAEISCONN:        pErr = "WSAEISCONN: The socket is already connected."; break;
    case WSAENETUNREACH:    pErr = "WSAENETUNREACH: The network cannot be reached from this host at this time."; break;
    case WSAENOBUFS:        pErr = "WSAENOBUFS: No buffer space is available. The socket cannot be connected."; break;
    case WSAENOTSOCK:       pErr = "WSAENOTSOCK: The descriptor is not a socket."; break;
    case WSAETIMEDOUT:      pErr = "WSAETIMEDOUT: Attempt to connect timed out without establishing a connection."; break;
    case WSAEWOULDBLOCK:    pErr = "WSAEWOULDBLOCK: The socket is marked as nonblocking and the connection cannot be completed immediately."; break;
    case WSAEACCES:         pErr = "WSAEACCES: Attempt to connect datagram socket to broadcast address failed because setsockopt SO_BROADCAST is not enabled."; break;
    case WSAENETRESET:      pErr = "WSAENETRESET: The connection has been broken due to the remote host resetting."; break;
    case WSAENOTCONN:       pErr = "WSAENOTCONN: The socket is not connected."; break;
    case WSAEOPNOTSUPP:     pErr = "WSAEOPNOTSUPP: MSG_OOB was specified, but the socket is not stream style such as type SOCK_STREAM, out-of-band data is not supported."; break;
    case WSAESHUTDOWN:      pErr = "WSAESHUTDOWN: The socket has been shut down."; break;
    case WSAEMSGSIZE:       pErr = "WSAEMSGSIZE: The socket is message oriented, and the message is larger than the maximum supported by the underlying transport."; break;
    case WSAEHOSTUNREACH:   pErr = "WSAEHOSTUNREACH: The remote host cannot be reached from this host at this time."; break;
    case WSAECONNABORTED:   pErr = "WSAECONNABORTED: The virtual circuit was terminated due to a time-out or other failure."; break;
    case WSAECONNRESET:     pErr = "WSAECONNRESET: The virtual circuit was reset by the remote side executing a 'hard' or 'abortive' close."; break;
  }
  // Display the string.
  MessageBox( hwnd, pErr, NULL, MB_OK|MB_SYSTEMMODAL );
}

//----------------------------------------------------------------------------//
extern "C" void print_sockname(SOCKET s)
{
	sockaddr_in sa;
	int nSize = sizeof(sa);
	memset(&sa, 0, nSize);
	if( getsockname(s, (sockaddr *)&sa, &nSize)==SOCKET_ERROR )
	{
		printf("Failed to getsockname\n");
		return;
	}
	printf("descr:  %d ", s);
	printf("family: %d ", sa.sin_family);
	printf("port:   %d ", sa.sin_port);
	printf("addr:   %d ", sa.sin_addr.s_addr);
	printf("zero:   %s \n", sa.sin_zero);
}

//----------------------------------------------------------------------------//
DWORD WINAPI ConnectionProc(void *ptr)
{
	Connection *pConn = (Connection *)ptr;
	int n;
	while( 1 )
	{
		u_long size = 0;
		ioctlsocket(pConn->s, FIONREAD, &size);
		if( size ) 
		{
			if( check(recv(pConn->s, (char *)&n, 4, 0), "recv") ) break;
			putch(n);
			if( n=='\r' ) putch('\n');
			if( n==27 ) 
			{
				printf("\a\n Client canceled!\n");
				break;
			}
		}
	}
	if( n!=27 ) WSAShowLastError(0);
	return 1;
}


Connection::Connection(SOCKET sock):s(sock)
{
	hThread = CreateThread(0, 1024, ConnectionProc, (void *)this, 0, &dwThreadId);
}

Connection::~Connection()
{
	shutdown(s, SD_SEND);
	closesocket(s);
	TerminateThread(hThread, 0);
	CloseHandle(hThread);
}

//----------------------------------------------------------------------------//
int check(int result, char *str)
{
	if( result==SOCKET_ERROR )
	{
		WSAShowLastError(0);
		printf("Failed to %s \n", str);
		return 1;
	}
	return 0;
}
