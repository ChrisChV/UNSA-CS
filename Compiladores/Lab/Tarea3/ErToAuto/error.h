#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <cstdio>

using namespace std;

#define TO_POSFIX_PARENTESIS_ERROR 1
#define FORMAT_ER_INICIO_CON_OPERADOR 2
#define FORMAT_ER_OP_PUNTO 3

class Error{
public:
	Error(int e, string l){
		error = e;
		linea = l;
	}
	int error;
	string linea;
};


void manejarError(Error e){
	switch(e.error){
		case TO_POSFIX_PARENTESIS_ERROR:
			fprintf(stderr, "Flata un parentesis en la expresion regular:%s\n", e.linea.c_str());
			break;
		case FORMAT_ER_INICIO_CON_OPERADOR:
			fprintf(stderr, "Una expresion regular no puede comenzar con operador:%s\n", e.linea.c_str());
			break;
		case FORMAT_ER_OP_PUNTO:
			fprintf(stderr, "Ńo se puede poner . en una expresion regular:%s\n", e.linea.c_str());
			break;
	}
}

#endif