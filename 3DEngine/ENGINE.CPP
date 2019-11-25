#include "engine.h"
#include <math.h>
#include <fstream.h>
#include <time.h>

//#include "unit3do.h"

//static Unit3DO unit( "bomb.3do" );

///////////////////////////////////////////////////////////////////////////////
static HRESULT WINAPI enum_zbuffer_proc( DDPIXELFORMAT* pddpf, VOID* ptr )
{
	if( pddpf->dwFlags == DDPF_ZBUFFER )
	{
		memcpy( ptr, pddpf, sizeof(DDPIXELFORMAT) );
		return D3DENUMRET_CANCEL;
	}
    return D3DENUMRET_OK;
}

static bool is_mmx_processor();
///////////////////////////////////////////////////////////////////////////////
GEngine::GEngine(HWND hwnd):
    m_hMainWindow(hwnd),
    m_pD3D(0),
	m_pD3DDevice(0),
	m_pddsFront(0),
	m_pddsBack(0),
	m_pddsZBuffer(0),

    m_vEye(0, 0, 0),
    m_vUp(0, 1, 0),
    m_vLookAt(0, 0, 1)
{
	DWORD dwFlags = DDSCL_FULLSCREEN|DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_ALLOWMODEX;
	g_pDirectDraw->SetCooperativeLevel( m_hMainWindow, dwFlags );
}

///////////////////////////////////////////////////////////////////////////////
bool GEngine::SetMode(int w, int h, int bpp)
{
	HRESULT hr;
	DDSCAPS2 ddscaps = { DDSCAPS_BACKBUFFER, 0, 0, 0 }; 
	DDSURFACEDESC2 desc;
	ASSERT( g_pDirectDraw );
	ASSERT( m_pddsFront==0 && m_pddsBack==0 );
	m_nScreenWidth = w;
	m_nScreenHeight = h;
	hr = g_pDirectDraw->SetDisplayMode( m_nScreenWidth, m_nScreenHeight, bpp, 0, 0 );
	if( hr!=DD_OK )
	{
		m_nScreenWidth = 640;
		m_nScreenHeight = 480;
		if( bpp > 16 ) bpp = 16;
		hr = g_pDirectDraw->SetDisplayMode( 640, 480, bpp, 0, 0 );
		if( hr!=DD_OK ) return false;
	}
	ZeroMemory( &desc, sizeof(desc) );
	desc.dwSize = sizeof( desc );
	desc.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
	desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_COMPLEX|DDSCAPS_FLIP|DDSCAPS_3DDEVICE;
	desc.dwBackBufferCount = 1;
	hr = g_pDirectDraw->CreateSurface( &desc, &m_pddsFront, NULL );
	ASSERT( hr==DD_OK );
	hr = m_pddsFront->GetAttachedSurface( &ddscaps, &m_pddsBack );
	ASSERT( hr==DD_OK );
	g_pDirectDraw->FlipToGDISurface();
	return true;
}

///////////////////////////////////////////////////////////////////////////////
bool GEngine::Init3D(bool bHardware)
{
	HRESULT hr;
	DDCAPS  caps;
	GUID    guid;
	DWORD   dwZBufferMemory;
	ASSERT( g_pDirectDraw );
	ASSERT( m_pddsFront && m_pddsBack );
	ASSERT( m_pddsZBuffer==0 );
	ASSERT( m_pD3D==0 );
	ASSERT( m_pD3DDevice==0 );
	ZeroMemory( &caps, sizeof(DDCAPS) );
	caps.dwSize = sizeof(DDCAPS);
	hr = g_pDirectDraw->GetCaps( &caps, NULL );
	if( bHardware && hr==DD_OK && (caps.dwCaps & DDCAPS_3D) && (caps.ddsCaps.dwCaps & DDSCAPS_ZBUFFER) )
	{
		dwZBufferMemory = DDSCAPS_VIDEOMEMORY;
		guid = IID_IDirect3DHALDevice;
	}
	else
	{
		dwZBufferMemory = DDSCAPS_SYSTEMMEMORY;
		guid = IID_IDirect3DRGBDevice;
	}
    hr = g_pDirectDraw->QueryInterface( IID_IDirect3D7, (void **)&m_pD3D );
	ASSERT( hr==DD_OK );

	DDSURFACEDESC2 desc;
	DDPIXELFORMAT ddpf;
	ZeroMemory( &ddpf, sizeof(DDPIXELFORMAT) );
	m_pD3D->EnumZBufferFormats( guid, enum_zbuffer_proc, &ddpf );
	ASSERT( ddpf.dwSize!=0 && ddpf.dwSize==sizeof(ddpf) );
	ZeroMemory( &desc, sizeof(desc) );
	desc.dwSize         = sizeof( desc );
	desc.dwFlags        = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PIXELFORMAT;
	desc.ddsCaps.dwCaps = DDSCAPS_ZBUFFER|dwZBufferMemory;
	desc.dwWidth        = m_nScreenWidth;
	desc.dwHeight       = m_nScreenHeight;
	memcpy( &desc.ddpfPixelFormat, &ddpf, sizeof(DDPIXELFORMAT) );
	hr = g_pDirectDraw->CreateSurface( &desc, &m_pddsZBuffer, NULL );
	ASSERT( hr==DD_OK );
	m_pddsBack->AddAttachedSurface( m_pddsZBuffer );

	hr = m_pD3D->CreateDevice( guid, m_pddsBack, &m_pD3DDevice );
	if( hr!=D3D_OK )
	{
		hr = m_pD3D->CreateDevice( IID_IDirect3DRGBDevice, m_pddsBack, &m_pD3DDevice );
		ASSERT( hr==D3D_OK );
	}
    D3DVIEWPORT7 viewport = { 0, 0, m_nScreenWidth, m_nScreenHeight, 0.0f, 1.0f };
	m_pD3DDevice->SetViewport( &viewport );

	D3DXMatrixPerspectiveFovLH( &m_mProj, D3DX_PI*0.72, float(m_nScreenHeight)/float(m_nScreenWidth), 0.01, MAX_Z_VALUE );
	m_pD3DDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, (D3DMATRIX *)&m_mProj );

	m_pD3DDevice->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffffffff );
	m_pD3DDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, 0 ); //!!!!!!
//	m_pD3DDevice->SetRenderState( D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME );
//!!!!!!!!!!

	DWORD dwFlags = (m_pddsZBuffer) ? D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER : D3DCLEAR_TARGET;
	m_pD3DDevice->Clear( 0, 0, dwFlags, 0xFF, MAX_Z_VALUE, 0 );

	return true;
}

///////////////////////////////////////////////////////////////////////////////
void GEngine::Flip()
{
	while(1)
	{
		HRESULT ddrval = m_pddsFront->Flip(NULL, 0);
		if(ddrval == DD_OK) break;
		if(ddrval == DDERR_SURFACELOST)
		{
			ddrval = m_pddsFront->Restore();
			if( m_pddsZBuffer ) m_pddsZBuffer->Restore();
			if(ddrval != DD_OK) break; 
		}
		if(ddrval != DDERR_WASSTILLDRAWING) break; 
	}
}

///////////////////////////////////////////////////////////////////////////////
void GEngine::Release()
{
	if( m_pD3DDevice ) { m_pD3DDevice->Release(); m_pD3DDevice = 0; }
	if( m_pddsZBuffer) { m_pddsZBuffer->Release();m_pddsZBuffer = 0;}
	if( m_pD3D )       { m_pD3D->Release();       m_pD3D = 0;       }
	if( m_pddsFront )  { m_pddsFront->Release();  m_pddsFront = 0;  }
}

///////////////////////////////////////////////////////////////////////////////
GEngine::~GEngine()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
void GEngine::Render()
{
	static D3DVECTOR v1( 0.0,  0.5, 1 );
	static D3DVECTOR v2( 0.5, -0.5, 1 );
	static D3DVECTOR v3(-0.5, -0.5, 1 );
	static D3DLVERTEX aTr[3] =
	{
		D3DLVERTEX( v1, 0xFF0000, 0, 0, 0 ), // Z-values!!!
		D3DLVERTEX( v2, 0x00FF00, 0, 0, 0 ),
		D3DLVERTEX( v3, 0xFFFF00, 0, 0, 0 )
	};
    HRESULT hr;
	DWORD dwFlags = (m_pddsZBuffer) ? D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER : D3DCLEAR_TARGET;
	hr = m_pD3DDevice->Clear( 0, 0, dwFlags, 0xFF, MAX_Z_VALUE, 0 );
	ASSERT( hr==D3D_OK );
	hr = m_pD3DDevice->BeginScene();
	ASSERT( hr==D3D_OK );
	if( hr==D3D_OK )
	{
		hr = m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, (LPVOID)aTr, 3, 0 );
		ASSERT( hr==D3D_OK );

//	unit.Render( m_pD3DDevice );

		m_pD3DDevice->EndScene();
	}
}

///////////////////////////////////////////////////////////////////////////////
void GEngine::Move( float dx, float dy, float dz )
{
	m_vEye.x += dx; m_vLookAt.x += dx; m_vUp.x += dx;
	m_vEye.y += dy; m_vLookAt.y += dy; // ALWAYS UP (or rotated)
	m_vEye.z += dz; m_vLookAt.z += dz; m_vUp.z += dz;
	D3DXMatrixLookAtLH( &m_mView, &m_vEye, &m_vLookAt, &m_vUp );
	m_pD3DDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, (D3DMATRIX *)&m_mView );
}

///////////////////////////////////////////////////////////////////////////////
void GEngine::Rotate( float yaw, float pitch, float roll ) // Y, X, Z
{
//!!!!!!!!!!!
	D3DXMATRIX m, mTr, mRot;
	D3DXVECTOR3 v1 = m_vLookAt;
	D3DXVECTOR3 v2 = m_vUp;
	D3DXMatrixTranslation( &mTr, m_vEye.x, m_vEye.y, m_vEye.z );
	D3DXMatrixRotationYawPitchRoll( &mRot, yaw, pitch, roll );
	m = mTr * mRot;
	D3DXVec3TransformCoord( &m_vLookAt, &v1, &m );
	D3DXVec3TransformCoord( &m_vUp,     &v2, &m );

	D3DXMatrixLookAtLH( &m_mView, &m_vEye, &m_vLookAt, &m_vUp );
	m_pD3DDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, (D3DMATRIX *)&m_mView );

//MessageBeep(-1);

}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
static bool is_mmx_processor()
{
	bool mmx = false;
#ifdef MMX
	_asm
	{
		push eax
		push edx
		mov eax, 1
		_emit 0x0F
		_emit 0xA2 // CPUID
		test edx, 0x00800000 // Is MMX technology bit set (Bit 23 of EDX)
		jz not_found
		mov mmx, 1
	not_found:
		pop edx
		pop eax
	}
#endif
	return mmx;
}

///////////////////////////////////////////////////////////////////////////////
extern "C" void _Assert(const char* szFile, const char* szExpr, int line)
{
	ofstream file("debug.err", ios::out|ios::app);
	if( file )
	{
		file << "---------------------------------------------------" << endl;
		time_t t = time(0);
		file << "Time: " << ctime( &t ) << endl;
		file << "Assertion failed in source file: " << szFile << endl;
		file << "Expression: " << szExpr << endl;
		file << "Line No: " << line << endl;
	}
	exit(1);
}
