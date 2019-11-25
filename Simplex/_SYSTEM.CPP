
#include <memory.h>
#include <windows.h>
#include "_system.h"

/*----------        error codes                      ----------*/   
#define ERROR_NOSOLUTION             1
#define ERROR_SIZE                   2
#define ERROR_INPUT                  3
#define ERROR_INTERNAL1              4
#define ERROR_INTERNAL2              5
#define ERROR_INTERNAL3              6
#define ERROR_INTERNAL_BETA          7

/*----------        error messages                   ----------*/   
#define MSG_NOSOLUTION       "╟рфрўр эх шьххЄ Ёх°хэш "
#define MSG_SIZE             "╨рчьхЁ ЄрсышЎ√ ёыш°ъюь сюы№°ющ"
#define MSG_INPUT            "╬°шсър ттюфр яЁртющ ўрёЄш эхЁртхэёЄтр"
#define MSG_INTERNAL1        "Internal error #1"
#define MSG_INTERNAL2        "Internal error #2" 
#define MSG_INTERNAL3        "Internal error #3" 
#define MSG_INTERNAL_BETA    "Error calculating 'Beta'"

//--------------------------------------------------------------------------
Simplex::Simplex(  int nVariables, int nEquations )
{
    nEqs=nEquations;
    nVars=nVariables;
    nTotalVars=nEqs+nVars;

    if( (nTotalVars > MAX1) || (nEqs > MAX2) )
      { 
        Error( MSG_SIZE, ERROR_SIZE ); 
        nEqs=0;
        nVars=0;
        nTotalVars=0;
        return; 
      }
    goal=0.0;
    
    memset( matr, 0, sizeof(matr) );
    memset( alfa, 0, sizeof(alfa) );
    memset( beta, 0, sizeof(beta) );
    memset(    z, 0, sizeof(z) );
    memset(prices, 0, sizeof(prices) );
    error=0;
                            
    int i;
    int j=nVars+3;
    for( i=0; i<nEqs; i++ )
    {
       matr[i][1]=nVars+i+1;   // OK!
       matr[i][j]=1.0;
       j++;
    }
}                  
//--------------------------------------------------------------------------
int Simplex::Solve()
{
    int field, record;
    int iteration=0;
    
    while(1)
    {
        ++iteration;
        find_basis();
        find_bkoef();          // Плохо ищет  ???
        calculate_prices();

        if( find_min(prices,nTotalVars) >= 0 )   // Задача pешена!
          {
             goal=calculate_goal();
             return iteration;
          }
        field=find_field();

        if( test_alfa( field )!=1  ) // Задача не имеет pешения! 
            return 0;
    
        calculate_alfa( field );
        record=find_record ();
        calculate_beta( field,record );
        Calculate( record );
    } // while
}
//--------------------------------------------------------------------------
void Simplex::find_basis()
{
  int i,j,find;
  int record;
   for(i=3;i<nTotalVars+3;i++)
   {
      j=find=0;
      record=-1;
      for(j=0;j<nEqs;j++)
        if( matr[j][i]==1 ) { find++;record=j; }
        else if( matr[j][i]!=0 ) { find=2;record=-1; }
      if( (find==1) && (record!=-1) ) matr[record][1]=i-2; 
                                     // ^^^^^ больше на 1 !!!
    }
  // NEXT VERSION:  xbasis[record]=i-2;
}
//--------------------------------------------------------------------------
void Simplex::find_bkoef()
{
   int j;
   for(j=0;j<nEqs;j++)  matr[j][0]=z[ (int)matr[j][1]-1  ];
}
//--------------------------------------------------------------------------
void Simplex::calculate_prices()
{
   int i,j;
    
   for(i=3;i<nTotalVars+3;i++)
       {
        prices[i-3]=0;
        for(j=0;j<nEqs;j++)
            prices[i-3]+=matr[j][i]*matr[j][0];
        prices[i-3]-=z[i-3];
        }
}
//--------------------------------------------------------------------------
void Simplex::print_price(ostream &os)
{
  int i;

 for(i=0;i<nTotalVars;i++) os<<prices[i]<<' ';
}
//--------------------------------------------------------------------------
float  Simplex::calculate_goal()
{
 float  sum=0;
 
 for( int j=0;j<nEqs;j++) sum+=matr[j][0]*matr[j][2];
 
 return sum;
}
//--------------------------------------------------------------------------
float Simplex::find_min( float array[], int arr_size )
{
  float   min;

  min=array[0];
  for( int i=0;i<arr_size;i++) if( array[i]<min ) min=array[i];
  return min;
}
//--------------------------------------------------------------------------
 int Simplex::find_field()
{       // must exist element<0 !!!
  int field=0;
  float   min = prices[0];
 
  for( int i=1; i < nTotalVars; i++)
     if( prices[i] < min ) {  min = prices[i]; field=i;}

  if( field >= nTotalVars )  Error( MSG_INTERNAL1, ERROR_INTERNAL1 );
 return field;
}
//--------------------------------------------------------------------------
int Simplex::test_alfa( int field )
{
  for( int j=0; j < nEqs; j++ ) // В столбце д.б. хотя бы одно число > 0
     if( matr[j][field+3] > 0 ) return 1;
 return 0;
}
//--------------------------------------------------------------------------
void Simplex::calculate_alfa( int field )
{
  for( int j=0; j < nEqs; j++)
    {
     if(matr[j][field+3]>0) alfa[j]=matr[j][2]/matr[j][field+3];
      else alfa[j]=-1;
     }
}
//--------------------------------------------------------------------------
 int Simplex::find_record()
{
       // Должно сyщ. pешение т.е. alfa[*]>0  !!!
 int j, record=0;
 float   min_positive=alfa[0];
                 
                 // Ищем какое-либо знач. >=0
 for(j=0;j<nEqs;j++)
      if( alfa[j]>=0 ) { min_positive=alfa[j];record=j;}

 for(j=0;j<nEqs;j++)     // А тепеp min из всех >=0
     if( (alfa[j]>=0) && (alfa[j]<min_positive) )
        { 
           record=j;
           min_positive=alfa[j];
        }

 if( min_positive < 0 ) Error( MSG_INTERNAL2, ERROR_INTERNAL2 );
 if( record > nEqs )    Error( MSG_INTERNAL3, ERROR_INTERNAL3 );

 return record;
}
//--------------------------------------------------------------------------
void Simplex::calculate_beta( int field,  int record )
{ 
 if( matr[record][field+3]==0 ) 
     Error( MSG_INTERNAL_BETA, ERROR_INTERNAL_BETA);
 
 for( int j=0;j<nEqs;j++)
    {
     if( j==record) beta[j]=1/matr[j][field+3];
      else beta[j]=-matr[j][field+3]/matr[record][field+3];
     }
}
//--------------------------------------------------------------------------
void Simplex::Calculate( int record )
{
 int i,j;

 for(j=0;j<nEqs;j++)
    for(i=2;i<nTotalVars+3;i++)
    {
      if( j==record ) break;
      else   matr[j][i]=matr[j][i]+matr[record][i]*beta[j];
    }
    for(i=2;i<nTotalVars+3;i++)
 matr[record][i]=matr[record][i]*beta[record];
}
//--------------------------------------------------------------------------
int Simplex::Error(char *msg, int code)
{
    error=code;
    ::MessageBox(NULL, msg, "Fatal Error", MB_OK | MB_SYSTEMMODAL );
    if( code==ERROR_NOSOLUTION ) return 0;
    return 1;
}

//--------------------------------------------------------------------------
ostream& operator<<(ostream& os, Simplex &s)
{
    s.putz(os); 
    s.put(os); 
    os<<'\n';
    s.print_price(os); 
    return os; 
}
//--------------------------------------------------------------------------
istream& operator >>(istream& is, Simplex &s)
{ 
    s.getz(is); 
    s.get(is); 
    return is;  
}
//--------------------------------------------------------------------------
void  Simplex::get(istream &is)
{
 /* 
     C        X       B
 | Коэф. | Базисн.| Пpавая | X1 | ... | Xn |
 | Б.П.в | Пеpем. | часть  |    |     |    |
 | цел.ф |        |        |    |     |    |
 |       |        |        | xx |     | xx |
 */   
 
 int i,j;
 for(j=0;j<nEqs;j++)
    {
//     Ввод коэффициентов ypавнения 
     for(i=3;i<nTotalVars+3;i++)
       {
         if(i<nVars+3)
            is>>matr[j][i];
         else
           {
            if( j==(i-3-nVars) )  matr[j][i]=1;
            else  matr[j][i]=0;
           }
        }
//   Введите значение пpавой части неpавенства >=0 
     is>>matr[j][2];
     if( matr[j][2] < 0 ){ Error( MSG_INPUT, ERROR_INPUT); return; }
     }
}
//--------------------------------------------------------------------------
void Simplex::put(ostream &os)
{
  int i,j;
 
 for(j=0;j<nEqs;j++)
    {
     os<<'\n';
     for(i=0;i<nTotalVars+3;i++)  os<<matr[j][i]<<' ';
     }
}
//--------------------------------------------------------------------------
void Simplex::getz(istream &is)
{
  int i;
 for(i=0;i<nVars;i++) is>>z[i];
}
//--------------------------------------------------------------------------
void Simplex::putz(ostream &os)
{
  int i;
  for(i=0;i<nTotalVars;i++) os<<z[i]<<' ';
}

//--------------------------------------------------------------------------
