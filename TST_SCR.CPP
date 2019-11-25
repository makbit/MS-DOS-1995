#include "screen.cpp"
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

extern int test1( Screen *scr);
extern int test2( Screen *scr);
extern int test3( Screen *scr);
extern int test4( Screen *scr);
extern int test5( Screen *scr);
extern int test6( Screen *scr);

extern Screen *select();

int randoms[800];
int maxx, maxy;

void main()
{
    Screen *scr;
    int time1, time2, time3;
    int time4, time5, time6;

    scr=select();
    maxx=scr->GetMaxX();
    maxy=scr->GetMaxY();

    for( int i=0; i<800; i++ ) randoms[i]=random(maxx);

    time1=test1(scr);
    time2=test2(scr);
    time3=test3(scr);
    time4=test4(scr);
    time5=test5(scr);
    time6=test6(scr);

    delete scr;
    printf("\n Total test time=%d", time1+time2+time3+time4+time5+time6);
    printf("\n PutPixel test time=%d", time1);
    printf("\n Line test time=%d", time2);
    printf("\n Rectangle test time=%d", time3);
    printf("\n Box test time=%d", time4);
    printf("\n VertLine & HorzLine test time=%d", time5);
    printf("\n Circle test time=%d", time6);

    getch();

}

///////////////////////////////////////////////////////////////
Screen *select()
{
    puts("\n Select Video Card to test:");
    puts("\n\t 1) VGA 320x200x256");
    puts("\n\t 2) Cirrus Logic 800x600x256");
    puts("\n\t 3) Trident 800x600x256");

    int key=getch();
    switch( key )
    {
        case '2': return new CirrusScreen;
        case '3': return new TridentScreen;
        default: break;
    }
    return new VGAScreen;
}
///////////////////////////////////////////////////////////////
int test1( Screen *scr)
{
    int x, y;
    struct time time1, time2;
    gettime( &time1 );


     for( x=maxx; x>0; x--)
        for( y=maxy; y>0; y--)
              scr->PutPixel(x,y, x);
     for( y=0; y<maxy; y++)
        for( x=0; x<maxx; x++)
            scr->PutPixel(x,y,y);
    gettime( &time2 );
    return (time2.ti_sec-time1.ti_sec);
}
///////////////////////////////////////////////////////////////
int test2( Screen *scr)
{
    int i;
    struct time time1, time2;
    gettime( &time1 );

    for( i=0; i < maxx/2; i++) scr->Line(0,randoms[i*2],randoms[i*2+1],maxy, i);
    for( i=0; i < maxy/2; i++) scr->Line(randoms[i*2],0,maxx,randoms[i*2+1], i);

    for( i=0; i < maxx/4; i++) scr->Line(randoms[4*i],randoms[4*i+1],
                                      randoms[4*i+2],randoms[4*i+3],i);
    gettime( &time2 );
    return (time2.ti_sec-time1.ti_sec);
}
///////////////////////////////////////////////////////////////
int test3( Screen *scr)
{
    int x,y;
    struct time time1, time2;
    gettime( &time1 );

        for( x=0; x<200; x++)
           scr->Rectangle(randoms[4*x],randoms[4*x+1],
                          randoms[4*x+2],randoms[4*x+3],x);
    gettime( &time2 );
    return (time2.ti_sec-time1.ti_sec);
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
int test4( Screen *scr)
{
    int x,y;
    struct time time1, time2;
    gettime( &time1 );

        for( x=0; x<10; x++)
           scr->Box(randoms[4*x],randoms[4*x+1],
                          randoms[4*x+2],randoms[4*x+3],x);
    gettime( &time2 );
    return (time2.ti_sec-time1.ti_sec);
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
int test5( Screen *scr)
{
    int x,y;
    struct time time1, time2;
    gettime( &time1 );

    for( y=0; y<maxy/2; y++)
           scr->HLine(randoms[2*y],randoms[2*y+1], y, y);
    for( x=0; x<maxx/2; x++)
           scr->VLine(randoms[2*x],randoms[2*x+1], x, x);
    gettime( &time2 );
    return (time2.ti_sec-time1.ti_sec);
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
int test6( Screen *scr)
{
    int x,y;
    struct time time1, time2;
    gettime( &time1 );

      for( x=0; x<maxx/3; x++)
           scr->Circle(randoms[3*x],randoms[3*x+1], maxy/4, x);
    gettime( &time2 );
    return (time2.ti_sec-time1.ti_sec);
}
///////////////////////////////////////////////////////////////
