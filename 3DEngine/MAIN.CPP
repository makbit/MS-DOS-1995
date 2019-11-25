#include "engine.h"
#include <windows.h>
#include <ddraw.h>

#define CLASS_NAME           "Main:Window"

/////////////// G L O B A L S /////////////////////////////////////////////////
extern HWND          g_hMainWindow;
extern IDirectDraw7* g_pDirectDraw;

///////////////////////////////////////////////////////////////////////////////
HWND          g_hMainWindow = NULL;
IDirectDraw7* g_pDirectDraw = NULL;

///////////////////////////////////////////////////////////////////////////////
extern "C" LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
static bool InitApplication(const HINSTANCE hInst, const HINSTANCE hPrev, const char *szTitle);
static bool g_bActive = 1;

static GEngine *pGFX;
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrev, LPSTR, int )
{
	MSG msg;
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height= GetSystemMetrics(SM_CYSCREEN);
	if( !InitApplication(hInstance, hPrev, "The D3D test!") ) return 1;
	GEngine ge( g_hMainWindow );
	if( !ge.SetMode(width, height, 16) )
	{
		DestroyWindow( g_hMainWindow );
		MessageBox(g_hMainWindow, "Failed to SetMode!", 0, 0);
		return 1;
	}
	if( !ge.Init3D() )
	{
		DestroyWindow( g_hMainWindow );
		MessageBox(g_hMainWindow, "Failed to Init3D!", 0, 0);
		return 1;
	}
	pGFX = &ge;
	ShowCursor( FALSE );
	while( 1 )
	{
		if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if( GetMessage(&msg,NULL,0,0)==0 ) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if( g_bActive )
		{
			ge.Render();
			ge.Flip();
			//MessageBeep(-1);
		}
		else WaitMessage();
	}
	return msg.wParam;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
extern "C" LRESULT WINAPI
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
		case WM_ACTIVATEAPP:
			g_bActive = (bool)wParam;
			ShowCursor( g_bActive ? FALSE : TRUE );
			ShowWindow(hwnd, (g_bActive) ? SW_NORMAL : SW_MINIMIZE);
			break;

		case WM_SYSCOMMAND:
			if( wParam==SC_KEYMENU ) return 0; // Trap 'ALT' key
			break;
		
#define angle  float(5.0 * 3.14 / 180.0)
#define delta  0.05f
		case WM_KEYDOWN:
			switch( wParam )
			{
				case VK_PRIOR:  pGFX->Rotate(0, +angle, 0); break; // X pitch
				case VK_NEXT:   pGFX->Rotate(0, -angle, 0); break;
				case VK_RIGHT:  pGFX->Rotate(+angle, 0, 0); break; // Y yaw
				case VK_LEFT:   pGFX->Rotate(-angle, 0, 0); break;
				case VK_END:    pGFX->Rotate(0, 0, +angle); break; // Z roll
				case VK_DELETE: pGFX->Rotate(0, 0, -angle); break;
			}
			break;
		case WM_CHAR:
			switch( wParam )
			{
				case 'X': pGFX->Move(+delta, 0, 0); break;
				case 'x': pGFX->Move(-delta, 0, 0); break;
				case 'Y': pGFX->Move(0, +delta, 0); break;
				case 'y': pGFX->Move(0, -delta, 0); break;
				case 'Z': pGFX->Move(0, 0, +delta); break;
				case 'z': pGFX->Move(0, 0, -delta); break;
			}
			break;
#undef delta
#undef angle
		case WM_CLOSE:
			DestroyWindow( g_hMainWindow );
			break;

		case WM_DESTROY:
			// Terminate DirectX here!
			if( g_pDirectDraw )
			{
				pGFX->Release();
				g_pDirectDraw->Release();
				g_pDirectDraw = NULL;
			}
			ShowCursor( TRUE );
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
static bool InitApplication(const HINSTANCE hInst, const HINSTANCE hPrev, const char *szTitle)
{
	WNDCLASS wc;
	if( !hPrev )
	{
		wc.style          = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW;
		wc.hInstance      = hInst;
		wc.lpfnWndProc    = (WNDPROC)(MainWndProc);
		wc.cbClsExtra     = 0;
		wc.cbWndExtra     = 0;
		wc.hIcon          = LoadIcon(NULL,IDI_APPLICATION);
		wc.hCursor        = LoadCursor(NULL,IDC_ARROW);
		wc.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName   = NULL;
		wc.lpszClassName  = CLASS_NAME;
		if( !RegisterClass(&wc) ) return false;
	}

	g_hMainWindow = CreateWindowEx(WS_EX_APPWINDOW, CLASS_NAME, szTitle,
	                               WS_CAPTION|WS_SYSMENU|WS_POPUP, 0, 0,
	                               GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
	                               NULL, NULL, hInst, NULL );
	if( !g_hMainWindow ) return false;
	ShowWindow( g_hMainWindow, SW_SHOWNORMAL );
	UpdateWindow( g_hMainWindow );

	HRESULT hr;
	hr = DirectDrawCreateEx( NULL, (void **)&g_pDirectDraw, IID_IDirectDraw7, NULL );
	if( hr!=DD_OK )
	{
		MessageBox( g_hMainWindow, "DirectX 7.0+ drivers not found!\n"
		            "Необходимо установить драйверы DirectX 7.0!", 0,
		            MB_OK|MB_ICONSTOP );
		return false;
	}
	return true;
}
