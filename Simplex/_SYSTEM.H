/*---------------------------------------------------------+
|                                                          |
|  Пpогpамма pешения задачи Лин-ого Пpогpаммиpования       |
|     Симплексным методом.                                 |
|                                                          |
|                                                          |
|  /см. Пpикладнyю матем./                                 |
|       Автоp -- Я .            December 1996              |
|                                                          |
|                                                          |
|                                                          |
|                                                          |
+---------------------------------------------------------*/

#ifndef __SIMPLEX_SYSTEM_H
#define __SIMPLEX_SYSTEM_H

#include <iostream.h>

/*----------    Максимальная pазмеpность матpицы     ----------*/ 
#define MAX1  50
#define MAX2  30

//-------------------------------------------------------------------
class Simplex
{
    public:
        float goal;               // значение целевой функции
        Simplex(  int nVariables, int nEquations);
        
        void get(istream &is);
        void put(ostream &os);
        int Solve();             // number of iterations or 0
        float calculate_goal();  // расчет текущ. знач. целевой f.
//        int Next();            // next iteration
        friend ostream& operator <<(ostream& os, Simplex &s);
        friend istream& operator >>(istream& is, Simplex &s);
    
    protected:
        virtual int Error(char *msg, int err_code); // 0 - if fatal
        void getz(istream &is);
        void putz(ostream &os);
        void print_price(ostream &os); // вывод оценок
        void Calculate( int record );
        
//-------------------------------------------------------------------
   public:
      int nEqs, nVars, nTotalVars;  // TotalVars=nEqs+nVars
      float matr   [MAX1][MAX2];    // симплекс матрица
      float z      [MAX2];          // коэф-ты целевой функции[nVars]
      float prices [MAX2];          // оценки [nVars]
      float alfa   [MAX1];          // вспомог. массив для поиска строки
      float beta   [MAX1];          // ------------ для рассчета  [nEqs]
//-------------------------------------------------------------------
   private:
      int error;
      void find_basis();         // поиск базисных переменных
      void find_bkoef();         // поиск базисных коэффициентов
      void calculate_prices();   // расчет оценок
      int find_field();          // поиск столбца из оценок
      int test_alfa( int _field );   //  !!!!!!!!!!!
              // расчет вспомог. массива 
      void calculate_alfa( int _field ); 
      int find_record();
      void   calculate_beta( int _field, int _record );
      float find_min(float array[], int size );
};
//--------------------------------------------------------------------------
//

#endif
