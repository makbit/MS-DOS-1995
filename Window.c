#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//--------------------------------------------------------------------------//
static HINSTANCE hThisInstance;
static HWND hMainWindow;
static char szClassName[]="My:Window";

extern LRESULT WINAPI 
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//--------------------------------------------------------------------------//
#ifdef __BORLANDC__
	#pragma argsused
#endif
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow )
{
	MSG msg;
	WNDCLASS wc;
	hThisInstance = hInst;
	
	wc.style          = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance      = hInst;
	wc.lpfnWndProc    = (WNDPROC)(WndProc);
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hIcon          = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor        = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName   = NULL;
	wc.lpszClassName  = szClassName;

	if( !RegisterClass(&wc) ) return 0;

	hMainWindow = CreateWindow( szClassName, "Test Program", WS_OVERLAPPEDWINDOW,
	                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	                            NULL, (HMENU)NULL, hInst, NULL );
	ShowWindow( hMainWindow, nCmdShow );
	UpdateWindow( hMainWindow );

	while( GetMessage(&msg,NULL,0,0)!=0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//--------------------------------------------------------------------------//
LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	switch( msg )
	{
		case WM_PAINT:
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default: break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

