#include "table.h"
#include "errhand.h"

#include <string.h>    
#include <math.h>

//----------------------------------------------------------\\
//  Математические функции с обpаботкой ощибок
//----------------------------------------------------------\\
extern   double     m_sqrt   ( double    arg );
extern   double     m_tan    ( double    arg );
extern   double     m_log    ( double    arg );
extern   double     m_ln     ( double    arg );
extern   double     m_acos   ( double    arg );
extern   double     m_asin   ( double    arg );

//-----------------------------------------------------------\\
//   Name table implementation                               \\     
//-----------------------------------------------------------\\     

void _Name_Table::reset()
{
    typedef double (*mathf)(double);
    
    char *names[]={ "sin", "cos", "sqrt", "tan", 
                    "abs", "log", "ln", "exp",
                    "asin", "acos", "atan" };
    mathf ptrf[]={   sin,     cos,    m_sqrt,   m_tan, 
                     fabs,    m_log,   m_ln,    exp,
                     m_asin,  m_acos, atan };
    
    memset( vars, 0, sizeof(_Variable)*__MAXVARS );
    
    insert("PI", M_PI);
    
    for( int i=0; i<__MAXFUNCT; i++ )
    {
        fs[i].f=ptrf[i];
        strcpy( fs[i].name, names[i]);
    }
}
    
//------------------------------------------------------------\\    
_Variable* _Name_Table::operator[]( char *name)
{
    for(int i=0; i<nVars; i++)
         if( !strcmp(vars[i].name,name)) return &vars[i];
    
    return insert(name, 0);
}

//------------------------------------------------------------\\    
_Function* _Name_Table::lookf( char *name)
{
    for(int i=0; i<__MAXFUNCT; i++)
       if( !strcmp( fs[i].name , name) ) return &fs[i];
    
    return 0;
}    

//------------------------------------------------------------\\    
_Variable* _Name_Table::insert( char *name, float value)
{
    strncpy( vars[nVars].name, name, 7 );
    vars[nVars].value=value;
    if( nVars==__MAXVARS-1 ) return &vars[nVars];
    return &vars[nVars++];
}

//----------------------------------------------------------\\
//  Математические функции с обpаботкой ощибок
//----------------------------------------------------------\\
extern double m_sqrt   ( double   arg )
{
    if( arg<0 )
    {
        error->badarg("sqrt");
        return arg;
    }
    return sqrt(arg);
}
        
//----------------------------------------------------------\\
extern double m_tan    ( double    arg )
{
    if( arg==M_PI/2 ) 
    {
        error->badarg("tan");
        return arg;
    }
    return tan(arg);
}
    
//----------------------------------------------------------\\
extern double m_log    ( double    arg )
{
    if( arg<0 ) 
    {
        error->badarg("log");
        return arg;
    }
    return log10(arg);
}
//----------------------------------------------------------\\
extern double m_ln     ( double    arg )
{
    if( arg<0 ) 
    {
        error->badarg("ln");
        return arg;
    }
    return log(arg);
}

//----------------------------------------------------------\\
extern   double     m_acos   ( double    arg )
{
    if( arg<-1 || arg>1 ) 
    {
        error->badarg("acos");
        return arg;
    }
    return acos(arg);
}
//----------------------------------------------------------\\
extern   double     m_asin   ( double    arg )
{
    if( arg<-1  || arg>1 ) 
    {
        error->badarg("asin");
        return arg;
    }
    return asin(arg);
}

