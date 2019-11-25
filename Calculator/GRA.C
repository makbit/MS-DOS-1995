#include "calculat.h"
#include <stdio.h>

 //-----------------------------------------------------  
void pixel(int x, int y, char color)
{
    if( x>=320 || x<0 || y<0 || y>=200 ) return;
    char far* video=(char far*)0xA0000000;
    video[320*y+x]=color;
}
//-----------------------------------------------------\\
void main(int argc, char **argv)
{
    
    puts("\n Graphics painter utility. (C)1996.");
    if( argc!=2 ) 
    {
        puts("Usage: gra.exe  sin(x*x)+sqrt(x)");
        return;
    }
    asm
    {
        mov ax,13h
        int 10h
    }
 //-----------------------------------------------------  
 // axis
 int i;
 for(  i=0; i<200; i++)pixel(50,i,15);
 for(  i=0; i<320; i++)pixel(i,150,15);
 //-----------------------------------------------------  
    
    char buf[240];
    puts(argv[1]);
    asm
    {
        mov ah,1
        int 21h
    }
    double x,y;
    for( x=0.01; x<20; x+=.02)
    {
        sprintf(buf, "x=%f",x);
        calculate(buf);
        sprintf(buf, "%s" ,argv[1] );    //!!!!
        y=calculate(buf);
        pixel(50+x*30, 150-y*30,12);
    }
    
    
//------------------------------------------------------    
    asm
    {
        mov ah,1
        int 21h
        mov ax,3
        int 10h
    }
    puts("\n Graphics painter utility. (C)1996.");
}
        
