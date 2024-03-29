#ifndef __CALCULAT_H
#define __CALCULAT_H

/*------------------------------------------------------------
         ����:          calculat.h
         ��������p:    Borland C++ 3.x 4.x
         ���:          14.01.1997
         
         
         �p���砭��:    ���⠪��᪨� ��������p ��p������.
                        ��⠥� ��p��� ⨯�: 2+(4*5-sin(3.14)) 
                        �����p������ �� 100 ��p�������,
                        11 ���p������ ��⥬���᪨� �㭪権.

         �맮� �㭪樨: float result = calculate( str, ptrf );
                        ��� 
                        str - ��p��� ���p�� ���� ���᫨��,
                        ptrf - 㪠��⥫� �� �㭪�� �뤠�
                               ᮮ�饭�� �� �騡��.

--------------------------------------------------------------*/

class _ParserErrorHandler;

#ifdef __cplusplus
extern "C" {
#endif


float calculate( char *str);

_ParserErrorHandler* 
  SetParserErrorHandler( _ParserErrorHandler *_new_handler);

#ifdef __cplusplus
}
#endif



#endif
