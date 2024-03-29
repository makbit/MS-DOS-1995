#ifndef __CALCULATE_ERRHAND_H
#define __CALCULATE_ERRHAND_H

#include <iostream.h>
#include <string.h>

/*------------------------------------------------------------
         ����:          table.h
         ��������p:    Borland C++ 3.x 4.x
         ���:          14.01.1997
         
         
         �p���砭��:    ��p����稪 �訡�� ��� ᨭ⠪��᪮�� 
                        ��������p� ��p������, ����騩 �� 
                        �������� ����� � ���饭��� ��p��p�-
                        ������ ��p�㠫쭮� �㭪樨 �뢮��.
                   
                   

                   
--------------------------------------------------------------*/


class _ParserErrorHandler
{
   public: 
      _ParserErrorHandler();
      
      void division();
      void missing();
      void unsupfnct(char *fnct_name="");
      void fnctmiss(char *fnct_name="");
      void syntax();
      void badnum();
      void badarg(char *fnct_name="");
      
   protected:
      virtual void out(char *s1, char *s2="", char *s3="");
   
   // Data
      char szDivision[40];
      char szMissing[40];
      char szUnsupFnct[40];
      char szFnctMiss[40];
      char szSyntax[40];
      char szBadNum[40];
      char szBadArg[40];
};

static _ParserErrorHandler     parser_error;
static _ParserErrorHandler*    error=&parser_error;
//------------------------------------------------------------//
//      Inlines  
//------------------------------------------------------------\\
//  ����饭�� �� �訡��� 
//------------------------------------------------------------//
inline _ParserErrorHandler::_ParserErrorHandler()
{
   strcpy(szDivision,  "Division by 0."           );
   strcpy(szMissing,   "Missing ')'."             );
   strcpy(szUnsupFnct, "Unsupported function."    );
   strcpy(szFnctMiss,  "Function ')' missing."    );
   strcpy(szSyntax,    "Syntax error."            );
   strcpy(szBadNum,    "Bad number."              );
   strcpy(szBadArg,    "Bad argument in function:");
}

inline void _ParserErrorHandler::division()
            { out(szDivision);   }
inline void _ParserErrorHandler::missing()
            { out(szMissing);    }
inline void _ParserErrorHandler::unsupfnct(char *fnct_name)
            { out(szUnsupFnct,fnct_name);  }
inline void _ParserErrorHandler::fnctmiss(char *fnct_name)
            { out(szFnctMiss, fnct_name);   }
inline void _ParserErrorHandler::syntax()
            { out(szSyntax);     }
inline void _ParserErrorHandler::badnum()
            { out(szBadNum);     }
inline void _ParserErrorHandler::badarg(char *fnct_name)
            { out(szBadArg,fnct_name); }
      
inline void _ParserErrorHandler::out(char *s1, char *s2, char *s3)
{ cerr<<"\nError:"<<s1<<s2<<s3<<'\n'; }



#endif
