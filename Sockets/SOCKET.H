#ifndef SOCKET_H__
#define SOCKET_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <nspapi.h>
#include <stdio.h>
#include <conio.h>

extern "C" void ShowLastError(HWND hwnd);
extern "C" void WSAShowLastError(HWND hwnd);
extern "C" void print_sockname(SOCKET s);
extern "C" int check(int result, char *str);
//--------------------------------------------------------------------------//
class WSAInitializer
{
public:
	WSAInitializer();
	~WSAInitializer();
	int operator !() { return bError; }
private:
	int bInit;
	int bError;
};

//--------------------------------------------------------------------------//
class Socket
{
	SOCKET s;
	sockaddr_in sa;
	char szHostName[80];
public:
	Socket(int port=0, int nReuseAddr=0);
	~Socket();
	operator SOCKET() { return s; }
	int operator !() { return (s==INVALID_SOCKET); }
	void getaddr(sockaddr *psa) { memcpy(psa, &sa, sizeof(sa)); }
};

//--------------------------------------------------------------------------//
DWORD WINAPI ConnectionProc(void *);

class Connection
{
	HANDLE hThread;
	DWORD dwThreadId;
public:
	SOCKET s;
	Connection(SOCKET s);
	~Connection();
};

#endif