#include <stdio.h>
#include <ctype.h>

int preanalisis;

void parea(int);
void error();
void Resto();
void Term();

void Exp(){
	if(isdigit(preanalisis)){
		Term();
		Resto();
	}
	else error();
}

void Resto(){
	if(preanalisis == '+'){
		parea('+');
		Term();
		printf("+");
		Resto();
	}
	else if(preanalisis == '-'){
		parea('-');
		Term();
		printf("-");
		Resto();
	}
	else;
}

void Term(){
	if(isdigit(preanalisis)){
		printf("%c",preanalisis);
		parea(preanalisis);
	}
	else error();
}

void error(){
	printf("Error de sintaxis");
}

void parea(int t){
	if(preanalisis == t) preanalisis = getchar();
	else error();
}

int main(){
	preanalisis = getchar();
	Exp();	
	printf("\n");
}