// Description: Source code for mouse support
// Compiler: Any C++ with inline assembler
// Written by Maximov K.M.  (C) 1994-1996

#include "mouse.h"

/////////////////////////////////////////////////////////////////////
void far MouseStub()
{
    CurrHandler;
    asm{
        push ds;
        push ax;
        mov ax, seg CurrHandler;
        mov ds, ax;
        pop ax;
        push dx;
        push cx;
        push bx;
        push ax;
        call CurrHandler;
        add sp, 8;
        pop ds;
    }
}
///////////////////////////////////////////////
Mouse::Mouse()
{
     CurrHandler=NULL;
     MousePresent=ResetMouse();
}

Mouse::~Mouse()
{
    if( CurrHandler ) RemoveHandler();
}
///////////////////////////////////////////////
int Mouse::ResetMouse()
{
    asm{
        xor ax, ax;
        int 33h;
    }
    return _AX==0xFFFF;
}
///////////////////////////////////////////////
void Mouse::Show()
{
    if( !MousePresent ) return;
    asm{
        mov ax, 1;
        int 33h;
    }
}
///////////////////////////////////////////////
void Mouse::Hide()
{
    if( !MousePresent ) return;
    asm{
        mov ax, 2;
        int 33h;
    }
}
///////////////////////////////////////////////
void Mouse::Move(int x, int y)
{
    if( !MousePresent ) return;
    asm{
        mov ax, 4;
        mov cx, x;
        mov dx, y;
        int 33h;
    }
}
///////////////////////////////////////////////
void Mouse::ReadState( MouseState &state)
{
    if( !MousePresent ) return;
    asm{
        mov ax, 3;
        int 33h;
    }
    asm{
        push es;
        push di;
        les di, dword ptr state;
        mov es:[di  ], cx;
        mov es:[di+2], dx;
        mov es:[di+4], bx;
        pop di;
        pop es;
    }
}
///////////////////////////////////////////////
void Mouse::SetShape(CursorShape &shape)
{
    if( !MousePresent ) return;
    asm{
        push es;
        push di;
        les di, dword ptr shape;
        mov bx, es:[di+16];
        mov cx, es:[di+18];
        mov dx, di;
        mov ax, 9;
        int 33h;
        pop di;
        pop es;
    }
}
///////////////////////////////////////////////
void Mouse::SetHandler( MouseHandler handler, int event)
{
    if( !MousePresent ) return;
    void far * p=MouseStub;
    CurrHandler=handler;
    asm{
        push es;
        mov ax, 0Ch
        mov cx, event;
        les dx, p;
        int 33h;
        pop es;
    }
}
///////////////////////////////////////////////
void Mouse::RemoveHandler()
{
    if( !MousePresent ) return;
    CurrHandler=NULL;
    asm{
        mov ax, 0Ch;
        mov cx, 0;
        int 33h;
    }
}
///////////////////////////////////////////////
void Mouse::SetHorzRange(int xmin, int xmax)
{
    if( !MousePresent ) return;
    asm{
        mov ax, 7;
        mov cx, xmin;
        mov dx, xmax;
        int 33h;
    }
}
///////////////////////////////////////////////
void Mouse::SetVertRange(int ymin, int ymax)
{
    if( !MousePresent ) return;
    asm{
        mov ax, 8;
        mov cx, ymin;
        mov dx, ymax;
        int 33h;
    }
}
///////////////////////////////////////////////
void Mouse::SetHideRange( int x1, int y1, int x2, int y2)
{
    asm{
        push si
        push di
        mov ax, 10h

        mov cx, x1
        mov dx, y1
        mov si, x2
        mov di, y2
        int 33h

        pop di
        pop si
    }
}
////////////////////////////////////////////////////////////////
