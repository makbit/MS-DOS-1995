#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>

#define WINDOW_W      116
#define WINDOW_H      30

static HINSTANCE hThisInstance;
static char szClassName[]="Clock:Window";

extern void Draw(HDC, int x, int y, char *str, HBITMAP);
extern void Load(RECT *rc);
extern void Save(const RECT *rc);

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
	HWND hMainWindow;
	hThisInstance = hInst;
	
	wc.style          = CS_VREDRAW|CS_HREDRAW;
	wc.hInstance      = hInst;
	wc.lpfnWndProc    = (WNDPROC)(WndProc);
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hIcon          = LoadIcon(hInst, "clock");
	wc.hCursor        = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName   = NULL;
	wc.lpszClassName  = szClassName;

	if( !RegisterClass(&wc) ) return 0;

	hMainWindow = CreateWindowEx(WS_EX_TOPMOST, szClassName, "", WS_POPUP|WS_VISIBLE,
	                            0, 0, WINDOW_W, WINDOW_H, NULL, (HMENU)NULL, hInst, NULL );
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
	static HBITMAP hBitmap;
	static time_t   t;
	static UINT hTimer;
	static POINT pt;
	switch( msg )
	{
		case WM_CREATE:
		{
			RECT rc;
			hBitmap = LoadBitmap(hThisInstance, "digits");
			hTimer = SetTimer(hwnd, 1, 200, 0);
			Load( &rc );
			MoveWindow(hwnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
		}break;
		case WM_PAINT:
			BeginPaint(hwnd, &ps);
			{
				char buffer[64];
				t = time(0);
				_strtime( buffer );
				Draw(ps.hdc, 2, 2, buffer, hBitmap);
				SetWindowText(hwnd, buffer);
			}
			EndPaint(hwnd, &ps);
			break;

		case WM_LBUTTONDOWN:
			SetCapture(hwnd);
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			break;
		case WM_LBUTTONUP:
			if( GetCapture()==hwnd ) ReleaseCapture();
			break;
		case WM_MOUSEMOVE:
			if( GetCapture()==hwnd )
			{
				POINT ptCursor;
				GetCursorPos(&ptCursor);
				MoveWindow(hwnd, ptCursor.x - pt.x, ptCursor.y - pt.y, 
				           WINDOW_W, WINDOW_H, TRUE);
			}break;
		case WM_RBUTTONUP:
			return SendMessage(hwnd, WM_CLOSE, 0, 0);

		case WM_DESTROY:
		{
			RECT rc;
			GetWindowRect(hwnd, &rc);
			Save( &rc );
			KillTimer(hwnd, 1);
			DeleteObject(hBitmap);
			PostQuitMessage(0);
		} return 0;

		case WM_TIMER:
			if( t != time(0) )
			{
				char buffer[64];
				HDC hdc = GetDC(hwnd);
				t = time(0);
				_strtime( buffer );
				Draw(hdc, 2, 2, buffer, hBitmap);
				SetWindowText(hwnd, buffer);
				ReleaseDC(hwnd, hdc);
			}
			return 0;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
			//break;
	}
	return 0;
}

//--------------------------------------------------------------------------//
extern void Draw(HDC hdc, int x, int y, char *str, HBITMAP hbm)
{
	int nCharWidth = 14;
	int nCurPos = 0;
	while( *str )
	{
		int i;
		switch( *str )
		{
			case '1': i = 0; break;
			case '2': i = 1; break;
			case '3': i = 2; break;
			case '4': i = 3; break;
			case '5': i = 4; break;
			case '6': i = 5; break;
			case '7': i = 6; break;
			case '8': i = 7; break;
			case '9': i = 8; break;
			case '0': i = 9; break;
			case ':': i =13; break;
			default: i=12; // space
		}
		// draw char
		HDC hDCMem = CreateCompatibleDC(hdc);
		HBITMAP hOldBM = (HBITMAP)SelectObject( hDCMem, (HBITMAP)hbm );
		BitBlt( hdc, x, y, nCharWidth, 26, hDCMem, i*nCharWidth, 0, SRCCOPY );
		SelectObject( hDCMem, hOldBM );
		DeleteDC( hDCMem );

		x+=nCharWidth;
		str++;
		nCurPos++;
	}
}

static char* szRegKey = "Software\\Costic\\Clock 1.0";
//--------------------------------------------------------------------------//
extern void Load(RECT *lprc)
{
	SetRect(lprc, 0, 0, WINDOW_W, WINDOW_H);
	HKEY hKey;
	LONG r = RegOpenKeyEx(HKEY_CURRENT_USER, szRegKey, 0, KEY_READ, &hKey);
	if( r==ERROR_SUCCESS )
	{
		DWORD size = sizeof(RECT);
		r = RegQueryValueEx(hKey, "Position", 0, 0, (BYTE*)lprc, &size);
		RegCloseKey(hKey);
	}
}

//--------------------------------------------------------------------------//
extern void Save(const RECT *lprc)
{
	HKEY hKey;
	DWORD dispos;
	LONG r = RegCreateKeyEx(HKEY_CURRENT_USER, szRegKey, 0, "Position", 0,
	                        KEY_WRITE, NULL, &hKey, &dispos);
	if( r==ERROR_SUCCESS )
	{
		DWORD size = sizeof(RECT);
		r = RegSetValueEx(hKey, "Position", 0, REG_BINARY, (BYTE*)lprc, size);
		RegCloseKey(hKey);
	}
}

