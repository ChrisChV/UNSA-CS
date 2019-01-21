#ifndef COMPILADORERTOAFN_H
#define COMPILADORERTOAFN_H

#include <iostream>
#include <algorithm>
#include <map>
#include <list>
#include "../Automata/automata.h"
#include "../ErToErPosfix/postfix.h"
#include "../ErToErPosfix/format.h"


using namespace std;

class CompiladorErToAuto{
public:
	CompiladorErToAuto(){};
	Automata run(string expresionRegular, string outFile);
	Automata operacionOR(Automata automata1, Automata automata2);
    Automata operacionCONCAT(Automata automata1, Automata automata2);
    Automata operacionSUMA(Automata automata1);
    Automata operacionESTRELLA(Automata automata1);
    void verifyAutomata(Automata & automata);
    
	IdEstado estadoActual;
};


Automata CompiladorErToAuto::run(string expresionRegular, string outFile){
	estadoActual = 0;
	list<Automata> pila;
	cout<<"Expresion Regular: "<<expresionRegular<<endl;
	string fER = formatER(expresionRegular);
	cout<<"Forma con formato: "<<fER<<endl;
	string postfixER = infijaToPosfija(fER);
	cout<<"Forma posfija: "<<postfixER<<endl;
	int op = -1;
	for(char c : postfixER){
		if((op = esOperadorOp(c)) != -1){
			Automata automata1;
			Automata automata2;
	    	switch(op){
				case OR:
					automata2 = pila.front();
					pila.pop_front();
					automata1 = pila.front();
					pila.pop_front();
					pila.push_front(operacionOR(automata1,automata2));
                    break;
                case CONCAT:
                    automata2 = pila.front();
                    pila.pop_front();
                    automata1 = pila.front();
                    pila.pop_front();
                    pila.push_front(operacionCONCAT(automata1,automata2));
                    break;
                case ESTRELLA:
                    automata1 = pila.front();
                    pila.pop_front();
                    pila.push_front(operacionESTRELLA(automata1));
                    break;
                case SUMA:
                    automata1 = pila.front();
                    pila.pop_front();
                    pila.push_front(operacionSUMA(automata1));
                    break;
			}
		}
		else pila.push_front(Automata(c,estadoActual));
	}
    Automata res = pila.front();
    verifyAutomata(res);
    cout<<endl;
    cout<<"Respuesta:";
    if(outFile == "cout"){
        cout<<endl;
        res.printAutomata(cout, false);  
    } 
    else{
        cout<<" Archivo "<<outFile<<endl;
        ofstream temp(outFile.c_str());
        res.printAutomata(temp, false);
    } 
    return res;
}

Automata CompiladorErToAuto::operacionOR(Automata automata1, Automata automata2){
    Automata res;
    res.expresionRegular = automata1.expresionRegular + "|" + automata2.expresionRegular;
    res.estados = automata1.estados;
    res.estados.insert(res.estados.end(), automata2.estados.begin(), automata2.estados.end());
    res.estados.push_back(new Estado(estadoActual));
    res.inicial = res.estados.back();
    estadoActual++;
    res.estados.push_back(new Estado(estadoActual));
    res.aceptacion.push_back(res.estados.back());
    estadoActual++;
    res.entradas = automata1.entradas;
    res.entradas.insert(res.entradas.end(), automata2.entradas.begin(), automata2.entradas.end());
    res.transiciones = automata1.transiciones;
    res.transiciones.insert(res.transiciones.end(), automata2.transiciones.begin(), automata2.transiciones.end());
    for(Estado * estado : automata1.aceptacion){
        res.transiciones.push_back(make_tuple(estado,VACIO,res.aceptacion.front()));
        estado->transiciones.push_back(make_tuple(nullptr,VACIO,res.aceptacion.front()));
    }
    for(Estado * estado : automata2.aceptacion){
        res.transiciones.push_back(make_tuple(estado,VACIO,res.aceptacion.front()));
        estado->transiciones.push_back(make_tuple(nullptr,VACIO,res.aceptacion.front()));
    }
    res.transiciones.push_back(make_tuple(res.inicial,VACIO,automata1.inicial));
    res.transiciones.push_back(make_tuple(res.inicial,VACIO,automata2.inicial));
    res.inicial->transiciones.push_back(make_tuple(nullptr,VACIO,automata1.inicial));
    res.inicial->transiciones.push_back(make_tuple(nullptr,VACIO,automata2.inicial));
    return res;
}

Automata CompiladorErToAuto::operacionCONCAT(Automata automata1, Automata automata2){
    Automata res;
    res.expresionRegular = automata1.expresionRegular + "." + automata2.expresionRegular;
    res.estados = automata1.estados;
    res.estados.insert(res.estados.end(), automata2.estados.begin(), automata2.estados.end());
    res.inicial = automata1.inicial;
    res.aceptacion = automata2.aceptacion;
    res.entradas = automata1.entradas;
    res.entradas.insert(res.entradas.end(), automata2.entradas.begin(), automata2.entradas.end());
    res.transiciones = automata1.transiciones;
    res.transiciones.insert(res.transiciones.end(), automata2.transiciones.begin(), automata2.transiciones.end());
    for(Estado * estado : automata1.aceptacion){
        res.transiciones.push_back(make_tuple(estado,VACIO,automata2.inicial));
        estado->transiciones.push_back(make_tuple(nullptr,VACIO,automata2.inicial));
    }
    return res;
}

Automata CompiladorErToAuto::operacionSUMA(Automata automata1){
    Automata res;
    res.expresionRegular = "(" + automata1.expresionRegular + ")+";
    res.estados = automata1.estados;
    res.estados.push_back(new Estado(estadoActual));
    res.inicial = res.estados.back();
    estadoActual++;
    res.estados.push_back(new Estado(estadoActual));
    res.aceptacion.push_back(res.estados.back());
    estadoActual++;
    res.entradas = automata1.entradas;
    res.transiciones = automata1.transiciones;
    for(Estado * estado : automata1.aceptacion){
        res.transiciones.push_back(make_tuple(estado,VACIO,res.aceptacion.front()));
        res.transiciones.push_back(make_tuple(estado,VACIO,automata1.inicial));
        estado->transiciones.push_back(make_tuple(nullptr,VACIO,res.aceptacion.front()));
        estado->transiciones.push_back(make_tuple(nullptr,VACIO,automata1.inicial));
    }
    res.transiciones.push_back(make_tuple(res.inicial,VACIO,automata1.inicial));
    res.inicial->transiciones.push_back(make_tuple(nullptr,VACIO,automata1.inicial));
    return res;
}

Automata CompiladorErToAuto::operacionESTRELLA(Automata automata1){
    Automata res = operacionSUMA(automata1);
    res.expresionRegular[res.expresionRegular.size() - 1] = '*';
    res.transiciones.push_back(make_tuple(res.inicial,VACIO,res.aceptacion.front()));
    res.inicial->transiciones.push_back(make_tuple(nullptr,VACIO,res.aceptacion.front()));
    return res;
}

void CompiladorErToAuto::verifyAutomata(Automata & automata){
    sort(automata.entradas.begin(),automata.entradas.end());
    automata.entradas.erase(unique(automata.entradas.begin(),automata.entradas.end()),automata.entradas.end());
}



#endif