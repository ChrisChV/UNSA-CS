#ifndef YY_parse_h_included
#define YY_parse_h_included
/*#define YY_USE_CLASS 
*/
#line 1 "/usr/share/bison++/bison.h"
/* before anything */
#ifdef c_plusplus
 #ifndef __cplusplus
  #define __cplusplus
 #endif
#endif


 #line 8 "/usr/share/bison++/bison.h"

#line 140 "Main/sintactico.y"
typedef union {
	int ival;
	float fval;
	char *sval;
} yy_parse_stype;
#define YY_parse_STYPE yy_parse_stype
#ifndef YY_USE_CLASS
#define YYSTYPE yy_parse_stype
#endif

#line 21 "/usr/share/bison++/bison.h"
 /* %{ and %header{ and %union, during decl */
#ifndef YY_parse_COMPATIBILITY
 #ifndef YY_USE_CLASS
  #define  YY_parse_COMPATIBILITY 1
 #else
  #define  YY_parse_COMPATIBILITY 0
 #endif
#endif

#if YY_parse_COMPATIBILITY != 0
/* backward compatibility */
 #ifdef YYLTYPE
  #ifndef YY_parse_LTYPE
   #define YY_parse_LTYPE YYLTYPE
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
  #endif
 #endif
/*#ifdef YYSTYPE*/
  #ifndef YY_parse_STYPE
   #define YY_parse_STYPE YYSTYPE
  /* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
   /* use %define STYPE */
  #endif
/*#endif*/
 #ifdef YYDEBUG
  #ifndef YY_parse_DEBUG
   #define  YY_parse_DEBUG YYDEBUG
   /* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
   /* use %define DEBUG */
  #endif
 #endif 
 /* use goto to be compatible */
 #ifndef YY_parse_USE_GOTO
  #define YY_parse_USE_GOTO 1
 #endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_parse_USE_GOTO
 #define YY_parse_USE_GOTO 0
#endif

#ifndef YY_parse_PURE

 #line 65 "/usr/share/bison++/bison.h"

#line 65 "/usr/share/bison++/bison.h"
/* YY_parse_PURE */
#endif


 #line 68 "/usr/share/bison++/bison.h"

#line 68 "/usr/share/bison++/bison.h"
/* prefix */

#ifndef YY_parse_DEBUG

 #line 71 "/usr/share/bison++/bison.h"

#line 71 "/usr/share/bison++/bison.h"
/* YY_parse_DEBUG */
#endif

#ifndef YY_parse_LSP_NEEDED

 #line 75 "/usr/share/bison++/bison.h"

#line 75 "/usr/share/bison++/bison.h"
 /* YY_parse_LSP_NEEDED*/
#endif

/* DEFAULT LTYPE*/
#ifdef YY_parse_LSP_NEEDED
 #ifndef YY_parse_LTYPE
  #ifndef BISON_YYLTYPE_ISDECLARED
   #define BISON_YYLTYPE_ISDECLARED
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;
  #endif

  #define YY_parse_LTYPE yyltype
 #endif
#endif

/* DEFAULT STYPE*/
#ifndef YY_parse_STYPE
 #define YY_parse_STYPE int
#endif

/* DEFAULT MISCELANEOUS */
#ifndef YY_parse_PARSE
 #define YY_parse_PARSE yyparse
#endif

#ifndef YY_parse_LEX
 #define YY_parse_LEX yylex
#endif

#ifndef YY_parse_LVAL
 #define YY_parse_LVAL yylval
#endif

#ifndef YY_parse_LLOC
 #define YY_parse_LLOC yylloc
#endif

#ifndef YY_parse_CHAR
 #define YY_parse_CHAR yychar
#endif

#ifndef YY_parse_NERRS
 #define YY_parse_NERRS yynerrs
#endif

#ifndef YY_parse_DEBUG_FLAG
 #define YY_parse_DEBUG_FLAG yydebug
#endif

#ifndef YY_parse_ERROR
 #define YY_parse_ERROR yyerror
#endif

#ifndef YY_parse_PARSE_PARAM
 #ifndef __STDC__
  #ifndef __cplusplus
   #ifndef YY_USE_CLASS
    #define YY_parse_PARSE_PARAM
    #ifndef YY_parse_PARSE_PARAM_DEF
     #define YY_parse_PARSE_PARAM_DEF
    #endif
   #endif
  #endif
 #endif
 #ifndef YY_parse_PARSE_PARAM
  #define YY_parse_PARSE_PARAM void
 #endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

 #ifndef YY_parse_PURE
  #ifndef yylval
   extern YY_parse_STYPE YY_parse_LVAL;
  #else
   #if yylval != YY_parse_LVAL
    extern YY_parse_STYPE YY_parse_LVAL;
   #else
    #warning "Namespace conflict, disabling some functionality (bison++ only)"
   #endif
  #endif
 #endif


 #line 169 "/usr/share/bison++/bison.h"
#define	T_INT	258
#define	T_FLOAT	259
#define	T_BOOL	260
#define	T_DOUBLE	261
#define	T_LONG	262
#define	T_VOID	263
#define	ID	264
#define	NUM_INT	265
#define	NUM_FLOAT	266
#define	NUM_DOUBLE	267
#define	NUM_LONG	268
#define	PARENTESIS_IZQUIERDO	269
#define	PARENTESIS_DERECHO	270
#define	LLAVE_IZQUIERDA	271
#define	LLAVE_DERECHA	272
#define	COMA	273
#define	PUNTO_Y_COMA	274
#define	FOR	275
#define	WHILE	276
#define	DO	277
#define	IF	278
#define	ELSE	279
#define	BREAK	280
#define	CONTINUE	281
#define	GOTO	282
#define	ASIGNACION	283
#define	ASIGNACION_MAS	284
#define	ASIGNACION_MENOS	285
#define	ASIGNACION_POR	286
#define	ASIGNACION_ENTRE	287
#define	OR_LOGICO	288
#define	AND_LOGICO	289
#define	MAS	290
#define	MENOS	291
#define	POR	292
#define	ENTRE	293
#define	ELEVADO	294
#define	MODULO	295
#define	IGUALDAD	296
#define	MAYOR	297
#define	MAYOR_IGUAL	298
#define	MENOR	299
#define	MENOR_IGUAL	300
#define	DIFERENTE	301
#define	NEGACION	302
#define	INCREMENTO	303
#define	DECREMENTO	304
#define	RETURN	305
#define	B_TRUE	306
#define	B_FALSE	307
#define	PRINT	308
#define	IN	309


#line 169 "/usr/share/bison++/bison.h"
 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
 #ifndef YY_parse_CLASS
  #define YY_parse_CLASS parse
 #endif

 #ifndef YY_parse_INHERIT
  #define YY_parse_INHERIT
 #endif

 #ifndef YY_parse_MEMBERS
  #define YY_parse_MEMBERS 
 #endif

 #ifndef YY_parse_LEX_BODY
  #define YY_parse_LEX_BODY  
 #endif

 #ifndef YY_parse_ERROR_BODY
  #define YY_parse_ERROR_BODY  
 #endif

 #ifndef YY_parse_CONSTRUCTOR_PARAM
  #define YY_parse_CONSTRUCTOR_PARAM
 #endif
 /* choose between enum and const */
 #ifndef YY_parse_USE_CONST_TOKEN
  #define YY_parse_USE_CONST_TOKEN 0
  /* yes enum is more compatible with flex,  */
  /* so by default we use it */ 
 #endif
 #if YY_parse_USE_CONST_TOKEN != 0
  #ifndef YY_parse_ENUM_TOKEN
   #define YY_parse_ENUM_TOKEN yy_parse_enum_token
  #endif
 #endif

class YY_parse_CLASS YY_parse_INHERIT
{
public: 
 #if YY_parse_USE_CONST_TOKEN != 0
  /* static const int token ... */
  
 #line 212 "/usr/share/bison++/bison.h"
static const int T_INT;
static const int T_FLOAT;
static const int T_BOOL;
static const int T_DOUBLE;
static const int T_LONG;
static const int T_VOID;
static const int ID;
static const int NUM_INT;
static const int NUM_FLOAT;
static const int NUM_DOUBLE;
static const int NUM_LONG;
static const int PARENTESIS_IZQUIERDO;
static const int PARENTESIS_DERECHO;
static const int LLAVE_IZQUIERDA;
static const int LLAVE_DERECHA;
static const int COMA;
static const int PUNTO_Y_COMA;
static const int FOR;
static const int WHILE;
static const int DO;
static const int IF;
static const int ELSE;
static const int BREAK;
static const int CONTINUE;
static const int GOTO;
static const int ASIGNACION;
static const int ASIGNACION_MAS;
static const int ASIGNACION_MENOS;
static const int ASIGNACION_POR;
static const int ASIGNACION_ENTRE;
static const int OR_LOGICO;
static const int AND_LOGICO;
static const int MAS;
static const int MENOS;
static const int POR;
static const int ENTRE;
static const int ELEVADO;
static const int MODULO;
static const int IGUALDAD;
static const int MAYOR;
static const int MAYOR_IGUAL;
static const int MENOR;
static const int MENOR_IGUAL;
static const int DIFERENTE;
static const int NEGACION;
static const int INCREMENTO;
static const int DECREMENTO;
static const int RETURN;
static const int B_TRUE;
static const int B_FALSE;
static const int PRINT;
static const int IN;


#line 212 "/usr/share/bison++/bison.h"
 /* decl const */
 #else
  enum YY_parse_ENUM_TOKEN { YY_parse_NULL_TOKEN=0
  
 #line 215 "/usr/share/bison++/bison.h"
	,T_INT=258
	,T_FLOAT=259
	,T_BOOL=260
	,T_DOUBLE=261
	,T_LONG=262
	,T_VOID=263
	,ID=264
	,NUM_INT=265
	,NUM_FLOAT=266
	,NUM_DOUBLE=267
	,NUM_LONG=268
	,PARENTESIS_IZQUIERDO=269
	,PARENTESIS_DERECHO=270
	,LLAVE_IZQUIERDA=271
	,LLAVE_DERECHA=272
	,COMA=273
	,PUNTO_Y_COMA=274
	,FOR=275
	,WHILE=276
	,DO=277
	,IF=278
	,ELSE=279
	,BREAK=280
	,CONTINUE=281
	,GOTO=282
	,ASIGNACION=283
	,ASIGNACION_MAS=284
	,ASIGNACION_MENOS=285
	,ASIGNACION_POR=286
	,ASIGNACION_ENTRE=287
	,OR_LOGICO=288
	,AND_LOGICO=289
	,MAS=290
	,MENOS=291
	,POR=292
	,ENTRE=293
	,ELEVADO=294
	,MODULO=295
	,IGUALDAD=296
	,MAYOR=297
	,MAYOR_IGUAL=298
	,MENOR=299
	,MENOR_IGUAL=300
	,DIFERENTE=301
	,NEGACION=302
	,INCREMENTO=303
	,DECREMENTO=304
	,RETURN=305
	,B_TRUE=306
	,B_FALSE=307
	,PRINT=308
	,IN=309


#line 215 "/usr/share/bison++/bison.h"
 /* enum token */
     }; /* end of enum declaration */
 #endif
public:
 int YY_parse_PARSE(YY_parse_PARSE_PARAM);
 virtual void YY_parse_ERROR(char *msg) YY_parse_ERROR_BODY;
 #ifdef YY_parse_PURE
  #ifdef YY_parse_LSP_NEEDED
   virtual int  YY_parse_LEX(YY_parse_STYPE *YY_parse_LVAL,YY_parse_LTYPE *YY_parse_LLOC) YY_parse_LEX_BODY;
  #else
   virtual int  YY_parse_LEX(YY_parse_STYPE *YY_parse_LVAL) YY_parse_LEX_BODY;
  #endif
 #else
  virtual int YY_parse_LEX() YY_parse_LEX_BODY;
  YY_parse_STYPE YY_parse_LVAL;
  #ifdef YY_parse_LSP_NEEDED
   YY_parse_LTYPE YY_parse_LLOC;
  #endif
  int YY_parse_NERRS;
  int YY_parse_CHAR;
 #endif
 #if YY_parse_DEBUG != 0
  public:
   int YY_parse_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
 #endif
public:
 YY_parse_CLASS(YY_parse_CONSTRUCTOR_PARAM);
public:
 YY_parse_MEMBERS 
};
/* other declare folow */
#endif


#if YY_parse_COMPATIBILITY != 0
 /* backward compatibility */
 /* Removed due to bison problems
 /#ifndef YYSTYPE
 / #define YYSTYPE YY_parse_STYPE
 /#endif*/

 #ifndef YYLTYPE
  #define YYLTYPE YY_parse_LTYPE
 #endif
 #ifndef YYDEBUG
  #ifdef YY_parse_DEBUG 
   #define YYDEBUG YY_parse_DEBUG
  #endif
 #endif

#endif
/* END */

 #line 267 "/usr/share/bison++/bison.h"
#endif
