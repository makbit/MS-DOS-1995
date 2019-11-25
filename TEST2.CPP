#include "calculat.h"
#include "errhand.h"
#include <iostream.h>
//------------------------------------------------------------\\
//                                                            \\
//                                                            \\
//     Simple test program: calculator with user defined      \\
//                          error handling functions          \\
//                                                            \\
//                                                            \\
//                                                            \\
//------------------------------------------------------------\\

class MyErrors: public _ParserErrorHandler
{
    public:
       MyErrors():_ParserErrorHandler(){}
       void out( char *s1, char *s2, char *s3)
              { cout<<"\n Ошибка:"<<'\a'<<s1<<s2<<s3
                    <<"\n Повтоpите ввод!";
              }
              
};


unsigned _stklen=10000;

void main()
{
  char buf[200];
  MyErrors handler;
  
  SetParserErrorHandler( &handler);
  while(1)  
  {
    
    cout<<"\nYes Master:";
    cin>>buf;
    cout<< calculate(buf);
  }
}
