#include "matrix.cpp"
#include <fstream.h>
#include <time.h>

// Borland C++ 5.2 compiler produces fater code (10%) than 
// MS-Visual C++ 5.0.
// (may be because of memory routines

void main()
{
    cout.flags( ios::fixed);//|ios::showpos ); // '+'
    cout.precision(4);
 try
 {
		Matrix Z;
		ifstream file("matr"); file>>Z;

		cout<<"Matrix Z: "<<Z<<endl;
time_t begin = time(0);
		Matrix Zi = Z;
		Zi.inverse();
		Matrix Ze = Z * Zi;
time_t end = time(0);
		cout<<"Matrix Ze:  "<<Ze<<endl;
		cout<<"Time:  "<<end-begin<<endl;
 }
 catch(const char *str)
 {
    cout<<"\nException:"<<str;   
 }
 catch(Matrix::Exception &x)    
 {
    cout<<"\nException:"<<x;   
 }
}
