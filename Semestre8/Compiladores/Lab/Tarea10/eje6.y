%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
%}

%token num
%left '+' '-'
%left '*'

%%

prg 	: exp {printf("%d\n", $1);} ;
exp 	: exp '+' term {$$=$1 + $3;}
		| exp '-' term {$$=$1 - $3;}
		| term 		   {$$=$1;};
term 	: term '*' factor {$$=$1 * $3;}
		| factor 		  {$$=$1;};
factor 	: '(' exp ')' {$$=$2;}
		| num 		  {$$=$1;};
		
%%

int yylex(){
	int c;
	do c = getchar(); while (c == ' ' || c == '\t');
	if(c == '\n') return EOF;
	if(isdigit(c)){
		ungetc(c, stdin);
		scanf("%d", &yylval);
		return num;
	}
	return c;
}

void yyerror(char * m){
	printf("%s", m);
	exit(0);
}

int main(){
	while(1){
		yyparse();
		printf("\n");
	}
}