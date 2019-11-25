#ifndef ENGINE_H
#define ENGINE_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef WIN32
#define WIN32
#endif

#define D3D_OVERLOADS

#include <assert.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include <d3dxmath.h>

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

#define MAX_Z_VALUE     100.0F
extern IDirectDraw7    *g_pDirectDraw;
///////////////////////////////////////////////////////////////////////////////
struct TEXTURE
{
	TEXTURE              *next;
	IDirectDrawSurface7  *pdds;
	char                 *pszTexName;
	char                 *pszFileName;

	TEXTURE();
	~TEXTURE();
//	int operator < ()

};

struct SPRITE
{
	SPRITE               *next;
	IDirectDrawSurface7  *pdds;
	SIZE                  size;
	POINT                 loc;
	dword                 dwNumFrames;
	dword                 dwDelay;

};

class BILLBOARD
{
	BILLBOARD *next;
	D3DVECTOR vLoc;
	RECT      rcSize;
	TEXTURE   *pTexture;

	void Rotate( const D3DVECTOR *pEye );
	void Render( const IDirect3DDevice7 * );

};

///////////////////////////////////////////////////////////////////////////////
class GEngine
{
	HWND                 m_hMainWindow;
	int                  m_nScreenWidth;
	int                  m_nScreenHeight;
	IDirect3D7*          m_pD3D;
	IDirect3DDevice7*    m_pD3DDevice;
	IDirectDrawSurface7* m_pddsFront;
	IDirectDrawSurface7* m_pddsBack;
	IDirectDrawSurface7* m_pddsZBuffer;

	D3DXVECTOR3          m_vEye;
	D3DXVECTOR3          m_vUp;
	D3DXVECTOR3          m_vLookAt;
	D3DXMATRIX           m_mProj;
	D3DXMATRIX           m_mView;
public:
	GEngine(HWND hwnd);
	~GEngine();
	bool SetMode(int w, int h, int bpp);
	bool Init3D(bool bHardware=true);

	void Flip();
	void Release();

	void Move(float dx, float dy, float dz);
	void Rotate(float yawY, float pitchX, float rollZ);

	virtual void Render();

	void DrawFPS(int x=0, int y=0);
/*****
	void Pause(bool b=true);
	void Restore();
******/
};

///////////////////////////////////////////////////////////////////////////////
extern "C" void _Assert(const char* szFile, const char* szExpr, int line);
#ifndef NDEBUG
	#define ASSERT(arg) if( !(void *)(arg) ) _Assert(__FILE__, #arg, __LINE__)
#else
	#define ASSERT(arg)
#endif

#endif // end of file
