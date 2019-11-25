#include "calculat.h"
#include "table.h"
#include "errhand.h"

#include <strstrea.h> 
#include <ctype.h>    
#include <math.h>


//----------------------------------------------------------\\
typedef enum{ FINISHED=0, PLUS='+', MINUS='-', MUL='*', DIV='/',
              OR='|', AND='&', NOT='!', POW='^', LESS='<',     
              GREATER='>', ASSIGN='=', LP='(', RP=')', 
              ID=257, NUMBER=258, BAD=0,
              EQUAL=260, NE=261, LE=262, GE=263 } _token_value;

//----------------------------------------------------------\\
static                   char    lexeme[80];
static           _token_value    token;
static            istrstream*    is;
static            _Name_Table    name_table;

//----------------------------------------------------------\\
static        float       Expr   ();
static        float       Term   ();
static        float       Factor ();
static        float       Power  ();
static _token_value       scan   ();


//----------------------------------------------------------\\
//        Code...
//----------------------------------------------------------\\
#ifdef __cplusplus
extern "C"{
#endif
float calculate( char *str)
{
  float v;
  
  istrstream istr(str);
  
  is=&istr;
  token=scan();
  do
   v=Expr();
  while( token!=FINISHED );
  return v;
}

//-----------------------------------------------------------\\
_ParserErrorHandler*                                         
SetParserErrorHandler( _ParserErrorHandler *_new_handler)
{
   _ParserErrorHandler* old_handler=error;
   error=_new_handler;
   return old_handler;
}

#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------\\
float Expr()
{
   float left=Term();
   while(1)
   switch( token )
   {
     case PLUS:
         token=scan();
         left+=Term();
         break;
     case MINUS:
         token=scan();
         left-=Term();
         break;
     case AND:
         token=scan();
		 left = ( left && Term() );
         break;
     case OR:
         token=scan();
         left = ( left || Term() );
         break;
     case NOT:
         token=scan();
         left = !Term();
         break;
     case LESS:
         token=scan();
         left = ( left < Term() );
         break;
     case GREATER:
         token=scan();
         left = ( left > Term() );
         break;
	 case EQUAL:
         token=scan();
         left = ( left==Term() );
         break;
     case NE:
         token=scan();
         left = ( left!=Term() );
         break;
     case LE:
         token=scan();
         left = ( left <= Term() );
         break;
     case GE:
         token=scan();
         left = ( left >= Term() );
         break;
     default: return left;
    }
}

//---------------------------------------------------\\

float Term()
{
  float left=Power();
  float right;
  while(1)
  switch( token )
  {
     case MUL:
        token=scan();
        left*=Power();
        break;
     case DIV:
        token=scan();
        right=Power();
		if( right==0 )
        {
          error->division();
          return 0;
         }
         left/=right;
         break; 
     default: return left;
  }
}

//-----------------------------------------------------------\\
float Power()
{
  float left=Factor();
  while(1)
  switch( token )
  {
	 case POW:
		token=scan();
		left=pow( left,Factor() );
		break;
	 default: return left;
  }
}

//-----------------------------------------------------------\\

float Factor()
{
  float value=0;
  switch( token )
  {
	case MINUS:
		token=scan();
		return -Factor();
	case PLUS:
		token=scan();
		return Factor();
	case LP:
		token=scan();
		value=Expr();
		if( token!=RP )
		{
		  error->missing();
		  return 0;
		}
		token=scan();
		return value;
	case FINISHED:
		return value;
	case NUMBER:
		token=scan();
		value=atof(lexeme);
		return value;
	case ID:
	   {
		 _Variable *var;
		 _Function *function;
		 token=scan();
		 switch( token )
		 {
			case '(':  // function
				function=name_table.lookf(lexeme);
				token=scan();
				if( !function )
				{
				   error->unsupfnct(function->name);
				   return 0;
				}
				value=(*function->f)(Expr());
				if( token!=RP )
					error->fnctmiss(function->name);
				token=scan();  // eat ')'
				return value;
			case '=':   // assign
				var=name_table[lexeme];
				token=scan();
				return var->value=Expr();
			default:
				var=name_table[lexeme];
				return var->value;
		 } // end of switch
	   } // end case ID
	default:
	  token=scan();
	  error->syntax();
	  return 0;
  }
}

//-----------------------------------------------------------\\

_token_value scan()
{
  int c;
  
  while(1)
  switch( c=is->get() )
  {
    case '+': case '-': case '*': case '/': case '^':
    case '&': case '|': case '(': case ')':
       return (_token_value)c;
    case ' ': case '\t':
       continue;
    case '\n': case '\0': case EOF: case 255: case ';':
       return FINISHED;
    case '<':
       c=is->get();
       if( c=='=' ) return LE;
       else  is->putback(c);
       return LESS;
    case '>':
       c=is->get();
       if( c=='=' ) return GE;
       else  is->putback(c);
       return GREATER;
    case '!':
       c=is->get();
       if( c=='=' ) return NE;
       else  is->putback(c);
       return NOT;
    case '=':
       c=is->get();
       if( c=='=' ) return EQUAL;
       else is->putback(c);
       return ASSIGN;
    default:
       if( isdigit(c) || (c=='.') )
       {
          int flag=0;
          char *s=lexeme;
          do
          {
            if( c=='.' )
                if( flag )
                {
                    error->badnum();
                    return BAD;
                }
                else flag++;
            *s++=c;
              // count++;
          } while( isdigit(c=is->get()) || c=='.' ); 
          *s='\0';
          is->putback(c);
          return NUMBER;
        }
        if( isalpha(c) )
        {
          char *s=lexeme;
          do
          {
             *s++=c;
             // count++;
          } while( isalnum(c=is->get()) ); 
          *s='\0';
          is->putback(c);
          return ID;
        } 
        return BAD;
  } // end of switch
}


