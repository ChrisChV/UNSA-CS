#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <cstdio>

using namespace std;

#define TO_POSFIX_PARENTESIS_ERROR 1

#define FORMAT_ER_INICIO_CON_OPERADOR 2
#define FORMAT_ER_OP_PUNTO 3

#define READ_AUTOMATA_LEX_ESTADOS 4
#define READ_AUTOMATA_LEX_INICIAL 5
#define READ_AUTOMATA_ESTADO_INICIAL 6
#define READ_AUTOMATA_LEX_ACEPTACION 7
#define READ_AUTOMATA_ESTADO_ACEPTACION 8
#define READ_AUTOMATA_LEX_ENTRADAS 9
#define READ_AUTOMATA_LEX_TRANSICIONES 10
#define READ_AUTOMATA_TRANSICION_ESTADO 11
#define READ_AUTOMATA_TRANSICION_ENTRADA 12
#define READ_AUTOMATA_END 13


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
        case READ_AUTOMATA_LEX_ESTADOS:
            fprintf(stderr, "Error en el archivo de entrada, después de la expresion regular debería ir Estados:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_LEX_INICIAL:
            fprintf(stderr, "Error en el archivo de entrada, después de los estado debería ir Inical:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_ESTADO_INICIAL:
            fprintf(stderr, "Error en el archivo de entrada, el estado inicial no existe en el conjunto de estados:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_LEX_ACEPTACION:
            fprintf(stderr, "Error en el archivo de entrada, después del estado inicial debería ir Aceptacion:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_ESTADO_ACEPTACION:
            fprintf(stderr, "Error en el archivo de entrada, el estado de acpetacion no existe en el conjunto de estados:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_LEX_ENTRADAS:
            fprintf(stderr, "Error en el archivo de entrada, después de los estados de aceptacion debería ir Entradas:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_LEX_TRANSICIONES:
            fprintf(stderr, "Error en el archivo de entrada, después de las entradas debería ir Transiciones:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_TRANSICION_ESTADO:
            fprintf(stderr, "Error en el archivo de entrada, el estado en la transicion no existe en el conjunto de estados:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_TRANSICION_ENTRADA:
            fprintf(stderr, "Error en el archivo de entrada, la entrada en la transicion no existe en el conjunto de estados:%s\n", e.linea.c_str());
            break;
        case READ_AUTOMATA_END:
            fprintf(stderr, "Error en el archivo de entrada, faltan propiedades del automata en el archivo%s\n", e.linea.c_str());
            break;
    }
}

#endif