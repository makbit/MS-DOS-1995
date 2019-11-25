#include "calculat.h"
#include <iostream.h>
//------------------------------------------------------------\\
//                                                            \\
//                                                            \\
//     Simple test program: calculator.                       \\
//                                                            \\
//                                                            \\
//                                                            \\
//                                                            \\
//------------------------------------------------------------\\

unsigned _stklen=10000;

void main()
{
  char buf[200];
  while(1)  
  {
    
    cout<<"\nYes Master:";
    cin>>buf;
    cout<< float(calculate(buf));
  }
}
