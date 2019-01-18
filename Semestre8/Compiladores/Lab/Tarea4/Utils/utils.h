#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>

using namespace std;

//Precedencia
// * +
// .
// |

enum Operaciones {OR,CONCAT,ESTRELLA,SUMA};

int esOperador(char e){
	if(e == '|') return 1;
	else if(e == '.') return 2;
	else if(e == '*' or e == '+') return 3;
	return -1;
}

int esOperadorOp(char e){
	if(e == '|') return OR;
	else if(e == '.') return CONCAT;
	else if(e == '*') return ESTRELLA;
	else if(e == '+') return SUMA;
	return -1;
}

string leerER(string fileName){
	ifstream file(fileName.c_str());
	string er = "";
	string temp = "";
	while(file>>temp){
		er = er + temp;
	}
	return er;
}


#endif