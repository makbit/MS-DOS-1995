
#define HIWORD(arg) ( (int)((arg)>>16) )
#define LOWORD(arg)  ( (int)(arg) )

#define ABS(arg)    ( (arg<0) ? -arg : arg )

class Screen
{
    protected:
        int CurrBank;
        char far* video;
        int MaxX, MaxY;
        virtual void SetBank( int start )=0;
    public:
      Screen(){ video=(char far*)0xA0000000; }
        virtual int GetMaxX(){ return MaxX; }
        virtual int GetMaxY(){ return MaxY; }

        virtual void FastPixel(int x,int y, long color);
        virtual void PutPixel(int x,int y, long color);
        virtual void Line(int x1, int y1, int x2, int y2, long color);
        virtual void HLine(int left, int right, int y, long color);
        virtual void VLine(int top, int bottom, int x, long color);
        virtual void Rectangle(int x1, int y1, int x2, int y2, long color);
        virtual void Box(int x1, int y1, int x2, int y2, long color);
        virtual void Circle(int x, int y, int radius, long color);
        virtual ~Screen();
};
/////////////////////////////////////////////////////////
class VGAScreen: virtual public Screen
{
        virtual void SetBank( int start ){}
    public:
        VGAScreen();
        virtual void FastPixel(int x,int y, long color);
        virtual void PutPixel(int x,int y, long color);

};
/////////////////////////////////////////////////////////
class CirrusScreen: virtual public Screen
{
        virtual void SetBank( int start );
    public:
        CirrusScreen(int mode=0x5C ); // 800x600x256
};

/////////////////////////////////////////////////////////
class TridentScreen: virtual public Screen
{
        virtual void SetBank( int start );
    public:
        TridentScreen( int mode=0x5D ); // 800x600x256
};
/////////////////////////////////////////////////////////
