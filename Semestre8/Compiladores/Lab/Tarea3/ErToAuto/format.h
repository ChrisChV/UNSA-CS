#ifndef FORMAT_H
#define FORMAT_H

#include <iostream>
#include <vector>
#include <cctype>
#include <clocale>
#include <list>
#include "error.h"
#include "utils.h"

using namespace std;

string formatER(string er){
	int estado = 0;
	string res = "";
	for(char e : er){
		if(estado == 0){
			if(esOperador(e) != -1) throw(Error(FORMAT_ER_INICIO_CON_OPERADOR, er));
			if(e == '(' or e == ')') estado = 1;
			else estado = 2;
			res.push_back(e);
		}
		else if(estado == 1){
			int op = esOperador(e);
			if(op == 2) throw(Error(FORMAT_ER_OP_PUNTO, er));
			if(op == -1 and e != '(' and e != ')')  estado = 2;
			else if(op == 3) estado = 3;
			res.push_back(e);
		}
		else if(estado == 2){
			int op = esOperador(e);
			if(op == 2) throw(Error(FORMAT_ER_OP_PUNTO, er));
			if(e == '('){
				res.push_back('.');	
				estado = 1;	
			} 
			else if(e == ')') estado = 4;
			else if(op == -1) res.push_back('.');
			else if(op == 3) estado = 3;
			else if(op == 1) estado = 1;
			res.push_back(e);
			
		}
		else if(estado == 3){
			int op = esOperador(e);
			if(op == 2) throw(Error(FORMAT_ER_OP_PUNTO, er));			
			if(op == -1 and e != ')'){
				res.push_back('.');
				estado = 2;
			}
			else if(op == 1) estado = 1;
			res.push_back(e);
		}
		else if(estado == 4){
			int op = esOperador(e);
			if(op == 2) throw(Error(FORMAT_ER_OP_PUNTO, er));
			if(op == -1 and e != ')'){
				res.push_back('.');
				estado = 2;	
			}
			else if(op == 1) estado = 1;
			else if(op == 3) estado = 3;
			res.push_back(e);
		}
	}
	return res;
}

#endif