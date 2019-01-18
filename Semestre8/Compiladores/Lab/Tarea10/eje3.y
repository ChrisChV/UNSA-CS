%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
%}

%token ID

%%
	S: 'x' {printf("token x \n");} S;
	S: A B 'c' ;
	A: 'a' ;
	B: 'b' ;
%%


int yylex(){
	int c;
	do c = getchar(); while(c == ' ' || c == '\t');
	if(c == '\n') return EOF;
	if(isalpha(c)) return c;
	yyerror("caracter ilegal");
}

void yyerror(char * m){
	printf("%s\n", m);
	exit(0);
}

int main(){
	while(1){
		yyparse();
	}
}