#ifndef COLORES_H
#define COLORES_H

#include <iostream>
#include <map>

using namespace std;

string getColor(int i){
	switch(i){
		case 0: return "black";
		case 1: return "red";
		case 2: return "blue";
		case 3: return "green";
		case 4: return "yellow";
		case 5: return "salmon";
		case 6: return "blueviolet";
		default: return "black";
	}
}

#endif