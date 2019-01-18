%{
	#include "y.h"
	#include <stdlib.h>
	#include <stdio.h>
%}



%option yylineno
lineComm \/\/.*\n
white [ \t\r\f\n]+
integer [0-9]+
floating [0-9]+\.[0-9]+
real [+-][0-9]+\.[0-9]+
identifier [a-zA-Z]+[0-9]*
blockComm \/\*(.*[\n\t\r\f]*)*\*\/

%%


{lineComm}  {}
{blockComm} {}
{white} 	{}

"int"		{yylval.ival = T_INT; return T_INT;};
"float"		{yylval.ival = T_FLOAT; return T_FLOAT;};
"bool"		{yylval.ival = T_BOOL; return T_BOOL;}
"double"	{yylval.ival = T_DOUBLE; return T_DOUBLE;}
"long"		{yylval.ival = T_LONG; return T_LONG;}
"void"		{yylval.ival = T_VOID; return T_VOID;}
"("			return PARENTESIS_IZQUIERDO;
")"			return PARENTESIS_DERECHO;
"{"			return LLAVE_IZQUIERDA;
"}"			return LLAVE_DERECHA;
","			return COMA;
";"			return PUNTO_Y_COMA;
"for"		return FOR;
"while"		return WHILE;
"do"		return DO;
"if"		return IF;
"else"		return ELSE;
"break"		return BREAK;
"continue"	return CONTINUE;
"goto"		return GOTO;
"true"		return B_TRUE;
"false"		return B_FALSE;
"print"		return PRINT;
"in"		return IN;
"=="		return IGUALDAD;
">="		return MAYOR_IGUAL;
">"			return MAYOR;
"<="		return MENOR_IGUAL;
"<"			return MENOR;
"!="		return DIFERENTE;
"="			return ASIGNACION;
"+="		return ASIGNACION_MAS;
"-="		return ASIGNACION_MENOS;
"*="		return ASIGNACION_POR;
"/="		return ASIGNACION_ENTRE;
"or"		return OR_LOGICO;
"and"		return AND_LOGICO;
"!"			return NEGACION;
"++"		return INCREMENTO;
"--"		return DECREMENTO;
"+"			return MAS;
"-"			return MENOS;
"*"			return POR;
"/"			return ENTRE;
"%"			return MODULO;
"^"			return ELEVADO;
"return"	return RETURN;
{real}		{yylval.fval = atof(yytext); return NUM_INT;}
{floating} 	{yylval.fval = atof(yytext); return NUM_FLOAT;}
{integer} 	{yylval.ival = atoi(yytext); return NUM_INT;}
{identifier} {yylval.sval = strdup(yytext); return ID;}
.			{printf("Error en la línea: %d \n", yylineno);}
%%
