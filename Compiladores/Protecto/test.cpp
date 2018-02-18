#include <stdio.h>

typedef union {
		int entero;
		float real;
		int boleano;
		double realLargo;
		long enteroLargo;
} TipoValor;

void * getVal(TipoValor val, int tipo){
		void * res = NULL;
		switch(tipo){
			case 0: res = (void *) &(val.entero); break;
			case 1: res = (void *) &(val.real); break;
			case 2: res = (void *) &(val.boleano); break;
			case 3: res = (void *) &(val.enteroLargo); break;
			case 4: res = (void *) &(val.realLargo); break;	
		}
		return res;
	}

void setVal(TipoValor * val, void * num, int tipo){
		switch(tipo){
			case 0: val->entero = *((int *) num); break;
			case 1: val->real = *((float *) num); break;
			case 2: val->boleano = (*((int *) num)) != 0; break;
			case 3: val->enteroLargo = *((long *) num); break;
			case 4: val->realLargo = *((double *) num); break;
		}
	}


int main(){
	/*
	TipoValor a1;
	TipoValor a2;
	TipoValor a3;
	a1.entero = 10;
	a2.real = 15.78;
	void * valA1 = getVal(a1, 0);
	void * valA2 = getVal(a2, 1);
	int num = *valA1 + *valA2;
	
	
	printf("%d\n", num);
	*/
	int  number;

		printf("Type in a number \n");
		scanf("%d", &number);
		printf("The number you typed was %d\n", number);
}