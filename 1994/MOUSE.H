// Description: Mouse support
// Compiler: Any C++ with inline assembler (Borland 3.x 4.x)
// Written by Maximov K.M.  (C) 1994-1996
// Comment: Класс и его фyнкции можно использовать даже
//          если мышь или дpайвеp не yстановлеы в системе
//          но мышь конечно же не появится! 

#ifndef __DOS_MOUSE_H
#define __DOS_MOUSE_H

#ifdef __WINDOWS_H
 #error Don't use this file under MS_Windows!
#endif

#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
 #error Please, change memory model!( to Compact,Large or Hude )
#endif

#ifndef NULL
  #define NULL 0L
#endif

/* Mouse handler events */
#define MOUSE_MOVE_MASK             0x01
#define MOUSE_LBUTTON_PRESS         0x02
#define MOUSE_LBUTTON_RELEASE       0x04
#define MOUSE_RBUTTON_PRESS         0x08
#define MOUSE_RBUTTON_RELEASE       0x10
#define MOUSE_MBUTTON_PRESS         0x20
#define MOUSE_MBUTTON_RELEASE       0x40
#define MOUSE_ALL_EVENTS            0x7F

/* Mouse buttons states */
#define MOUSE_LBUTTON_DOWN          0x01
#define MOUSE_RBUTTON_DOWN          0x02
#define MOUSE_MBUTTON_DOWN          0x04

struct MouseState
{
    int x,y;
    int Buttons;
};

struct CursorShape
{
    unsigned int AndMask[16];
    unsigned int XorMask[16];
    int HotX, HotY;
};

typedef void (*MouseHandler)(int mask,int Buttons,int x,int y);
static MouseHandler CurrHandler=NULL;

class Mouse
{
    public:
        Mouse();

        int  ResetMouse();
        void Show();
        void Hide();
        void Move(int x, int y);
        void ReadState( MouseState &);

        void SetShape(CursorShape &);

        // MouseHandler - Always void _far_ function!!!
        // Exaple: void far func(int,int,int,int){}
        void SetHandler( MouseHandler, int=MOUSE_ALL_EVENTS);
        void RemoveHandler();
        void SetHorzRange(int xmin, int xmax);
        void SetVertRange(int ymin, int ymax);
        void SetHideRange( int x1, int y1, int x2, int y2);

        ~Mouse();
     private:
        int MousePresent;
};

 // Internal use only!
static void far MouseStub();

#endif
