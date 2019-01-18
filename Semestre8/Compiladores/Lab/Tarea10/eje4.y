%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
%}

%token num
%left '+'
%left '-'

%%
	exp : exp '+' term {printf("+");} ;
	exp : exp '-' term {printf("-");} ;
	exp : term ;
	term : '(' exp ')' ;
	term : num {printf("%d", $1);} ;
%%

int yylex(){
	int c;
	do c = getchar(); while (c == ' ' || c == '\t');
	if(c == '\n') return EOF;
	if(isdigit(c)){
		ungetc(c,stdin);
		scanf("%d", &yylval);
		return num;
	}
	return c;
}

void yyerror(char * m){
	printf("%s\n", m);
	exit(0);
}

int main(){
	while(1){
		yyparse();
		printf("\n");
	}
}