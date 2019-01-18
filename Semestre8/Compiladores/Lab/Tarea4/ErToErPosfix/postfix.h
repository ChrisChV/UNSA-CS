#ifndef POSFIX_H
#define POSFIX_H


#include <iostream>
#include <vector>
#include <cctype>
#include <clocale>
#include <list>
#include "../Error/error.h"
#include "../Utils/utils.h"

using namespace std;



string infijaToPosfija(string er){
	list<char> pila;
	string res = "";
	int presedenciaE = -1;
	for(char e : er){
		if((presedenciaE = esOperador(e)) != -1){
			int presedencia2 = -1;
			char temp;
			while(!pila.empty()){
				temp = pila.front();
				if((presedencia2 = esOperador(temp)) == -1) break;
				else if(presedencia2 >= presedenciaE){
					pila.pop_front();
					res.push_back(temp);
				}
				else break;
			}
			pila.push_front(e);
		}
		
		else if(e == '(') pila.push_front(e);
		else if(e == ')'){
			bool flag = false;
			char temp;
			while(!pila.empty()){
				temp = pila.front();
				pila.pop_front();
				if(temp == '('){
					flag = true;
					break;
				}
				res.push_back(temp);
			}
			if(!flag) throw(Error(TO_POSFIX_PARENTESIS_ERROR,er));
		}
		else res.push_back(e);
		
	}
	for(char c : pila){
		res.push_back(c);
	}
	return res;
}




#endif