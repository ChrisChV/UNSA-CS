#ifndef COMPILADORAFNTOAFD_H
#define COMPILADORAFNTOAFD_H


#include <iostream>
#include "../Automata/automata.h"


using namespace std;

class CompiladorAFNToAFD{
public:
    CompiladorAFNToAFD(){};
    Automata run(string inFile, string outFile);

    IdEstado estadoActual;
};

Automata CompiladorAFNToAFD::run(string inFile, string outFile){
    Automata automata(inFile);
    Automata res;
    estadoActual = 0;
    list<Estado *> pila;
    res.inicial = new Estado(estadoActual,automata.e_clausura({automata.inicial}));
    res.estados.push_back(res.inicial);
    res.entradas = automata.entradas;
    res.expresionRegular = automata.expresionRegular;
    pila.push_front(res.estados.back());
    Estado * actual = nullptr;
    Estado * temp = nullptr;
    estadoActual++;
    while(!pila.empty()){
        actual = pila.front();
        pila.pop_front();   
        for(char caracter : automata.entradas){
            vector<Estado *> subConjunto = automata.e_clausura(automata.findTransiciones(actual->subEstados,caracter));
            if(!subConjunto.empty()){
                temp = res.findSubConjunto(subConjunto);
                if(temp == nullptr){
                    temp = new Estado(estadoActual,subConjunto);
                    estadoActual++;
                    res.estados.push_back(temp);
                    pila.push_front(temp);
                }
                res.transiciones.push_back(make_tuple(actual,caracter,temp));
                actual->transiciones.push_back(make_tuple(nullptr,caracter,temp));
            }
            
        }
    }
    for(Estado * estado : res.estados){
        for(Estado * subEstado : estado->subEstados){
            if(automata.esAceptacion(subEstado->id)){
                res.aceptacion.push_back(estado);
                break;
            }
        }   
    }

    if(outFile == "cout") res.printAutomata(cout, true);
    else{
        ofstream out(outFile.c_str());
        res.printAutomata(out, true);
    }
    return res;
}


#endif