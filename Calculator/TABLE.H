#ifndef __CALCULATE_TABLE_H
#define __CALCULATE_TABLE_H

/*------------------------------------------------------------
         ����:          table.h
         ��������p:    Borland C++ 3.x 4.x
         ���:          14.01.1997
         
         
         �p���砭��:    ������ ���� ��� �������p� (ᨭ�-
                        ���᪮�� ��������p� ��p������),
                        ������ �� ��p������� � �㭪権.
                   
                   
         -= ��� ����p������ �ᯮ�짮����� =-     


                   
--------------------------------------------------------------*/

// ���-�� �㭪権 � ��p�������
#define __MAXFUNCT      11
#define __MAXVARS       100

struct _Variable { char name[8]; float value;  };
struct _Function { char name[8]; double (*f)(double); };

class _Name_Table
{
   public: 
      _Name_Table();
           void     reset        ( );
      _Variable*    operator []  ( char *name );
      _Function*    lookf        ( char *name );
            int     count        ( );
            int     free         ( );
   
   private:
     _Variable vars[__MAXVARS];
     _Function fs[__MAXFUNCT];
     _Variable *insert( char *name, float value=0);
      int nVars;
};

//------------------------------------------------------------//
//      Inlines  
//------------------------------------------------------------//
inline _Name_Table::_Name_Table(){ reset(); }
inline int _Name_Table::count(){ return nVars; }
inline int _Name_Table::free(){ return 100-nVars; }

#endif
