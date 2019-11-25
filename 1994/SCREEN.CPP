#include "screen.h"

#define RATIO 2
//////////////////////////////////////////////////////
Screen::~Screen()
{
    asm{ mov ax, 3;
         int 10h
        }
}
//////////////////////////////////////////////////////
void Screen::FastPixel(int x, int y, long color)
{
    long addr=long(MaxX)*(long)y+(long)x;
    register int bank=HIWORD(addr);
    if( bank!=CurrBank ) SetBank(bank);
    *(video+LOWORD(addr))=char(color);
}
//////////////////////////////////////////////////////
void Screen::PutPixel(int x, int y, long color)
{
    if( x<0 || x>MaxX || y<0 || y>MaxY ) return;
    long addr=long(MaxX)*(long)y+(long)x;
    register int bank=HIWORD(addr);
    if( bank!=CurrBank ) SetBank(bank);
    *(video+LOWORD(addr))=char(color);
}
//////////////////////////////////////////////////////
void Screen::Line(int x1, int y1, int x2, int y2, long color)
{   // Brezenham's algorithm

    int dx=ABS(x2-x1);
    int dy=ABS(y2-y1);
    int sx= ( x2>=x1 ) ? 1 : -1;
    int sy= ( y2>=y1 ) ? 1 : -1;

    if( dy<=dx )
    {
        int d= ( dy << 1) - dx;
        int d1= dy << 1;
        int d2= ( dy - dx ) << 1;

        PutPixel(x1,y1,color);
        for( int x=x1+sx, y=y1, i=1; i<=dx; i++, x+=sx )
        {
            if( d>0 )
            {
                d+=d2; y+=sy;
            }
            else
                d+=d1;
            PutPixel(x,y,color);
        }
    }
    else
    {
        int d= ( dx << 1) - dy;
        int d1= dx << 1;
        int d2= ( dx - dy ) << 1;

        PutPixel(x1,y1,color);
        for( int x=x1, y=y1+sy, i=1; i<=dy; i++, y+=sy )
        {
            if( d>0 )
            {
                d+=d2; x+=sx;
            }
            else
                d+=d1;
            PutPixel(x,y,color);
        }
    }
}
//////////////////////////////////////////////////////
void Screen::HLine(int left, int right, int y, long color)
{
    if( left > right )
    {
        register int tmp=left;
        left=right;
        right=tmp;
    }
    for( register int x=left; x <= right; x++)
            PutPixel( x, y, color);
}
//////////////////////////////////////////////////////
void Screen::VLine(int top, int bottom, int x, long color)
{
    if( top > bottom )
    {
        register int tmp=top;
        top=bottom;
        bottom=tmp;
    }

    for( register int y=top; y <= bottom; y++)
            PutPixel(x,y, color);
}
//////////////////////////////////////////////////////
void Screen::Rectangle(int left, int top, int right,
                  int bottom, long color)
{
    HLine(left, right, top, color);
    HLine(left, right, bottom, color);

    VLine(top+1, bottom-1, left, color);
    VLine(top+1, bottom-1, right, color);
}
//////////////////////////////////////////////////////
void Screen::Box(int left, int top, int right,
                  int bottom, long color)
{
    if( left > right )
    {
       register int tmp=left;
       left=right;
       right=tmp;
    }
    if( top > bottom )
    {
        register int tmp=top;
        top=bottom;
        bottom=tmp;
    }
    for( register int y=top; y<=bottom; y++)
        for( register int x=left; x<=right; x++)
            PutPixel(x,y,color);
}
//////////////////////////////////////////////////////
void Screen::Circle(int x, int y, int radius, long color)
{
   int lx = 0, ly = radius, ld = 2 * (1 - radius);
   while(ly > 0)
    {
      PutPixel(x + lx, y + ly,color);
      PutPixel(x + lx, y - ly,color);
      PutPixel(x - lx, y + ly,color);
      PutPixel(x - lx, y - ly,color);
      if((ld+ly) > 0) {ly -= 1; ld -= (ly * RATIO ) - 1;}
      if(lx > ld) {lx += 1;ld += (2 * lx) + 1;}
     }
    PutPixel(x - lx, y,color);
    PutPixel(x + lx, y,color);
}
//////////////////////////////////////////////////////
/********************
void BrSolidCircle(unsigned int xc, unsigned int yc, unsigned int r,uchar ink)
{ int lx = 0, ly = r, ld = 2 * (1 - r);
   while(ly > 0)
	{ HLine(xc - lx, yc + ly, lx + lx,ink);
	  HLine(xc - lx, yc - ly, lx + lx,ink);
	 if((ld+ly) > 0) {ly -= 1; ld -= (ly * HV_GRADE2) - 1;}
   if(lx > ld) {lx += 1; ld += (2 * lx) + 1;}
	}
  HLine(xc - lx, yc, lx + lx,ink);
 }
****************/
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////  VGA standard /////////////////////////////
//////////////////////////////////////////////////////
 VGAScreen::VGAScreen():Screen()
 {
    asm{
        mov ax, 13h
        int 10h
    }
    MaxX=320;
    MaxY=200;
}
//////////////////////////////////////////////////////
void VGAScreen::PutPixel(int x, int y, long color)
{
    if( x<0 || x>320 || y<0 || y>200 ) return;
    *(video+320*y+x)=char(color);
}

//////////////////////////////////////////////////////
void VGAScreen::FastPixel(int x, int y, long color)
{
    *(video+320*y+x)=char(color);
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////  Cirrus Logic /////////////////////////////
//////////////////////////////////////////////////////
CirrusScreen::CirrusScreen(int mode):Screen()
{
    asm{
        mov ax, mode
        int 10h
        mov dx, 3C4h
        mov al, 6
        out dx, al
        inc dx
        mov al, 12h
        out dx, al
    }
    CurrBank=0;
    MaxX=800;
    MaxY=600;
}

//////////////////////////////////////////////////////
void CirrusScreen::SetBank(int start)
{
    CurrBank=start;
    asm{
        mov dx, 3CEh
        mov al, 9
        mov ah, byte ptr start
        mov cl, 4
        shl ah, cl
        out dx, ax
    }
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/////////////  Trident ///////////////////////////////
//////////////////////////////////////////////////////
TridentScreen::TridentScreen( int mode ):Screen()
{
    asm{
        mov ax, mode
        int 10h

        mov dx, 3CEh
        mov al, 6
        out dx, al
        inc dx
        in al, dx
        dec dx
        or al, 4
        mov ah, al
        mov al, 6
        out dx, ax

        mov dx, 3C4h
        mov al, 0Bh
        out dx, al
        inc dx
        in al, dx
    }
    MaxX=640;
    MaxY=480;
}
//////////////////////////////////////////////////////
void TridentScreen::SetBank(int start)
{
    CurrBank=start;
    asm{
        mov dx, 3C4h
        mov al, 0Bh
        out dx, al
        inc dx
        mov al, 0
        out dx, al
        in al, dx
        dec dx

        mov al, 0Eh
        mov ah, byte ptr start
        xor ah, 2
        out dx, ax
    }
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
