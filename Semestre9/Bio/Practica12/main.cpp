#include <iostream>
#include "AS1.h"

using namespace std;
				        //       A   B   C   D   E   F   G   H   I   J
matrix costos = 			  {{ 0, 12,  3, 23,  1,  5, 23, 56, 12, 11}, //A
					   		  {12,  0,  9, 18,  3, 41, 45,  5, 41, 27}, //B
					   		  { 3,  9,  0, 89, 56, 21, 12, 48, 14, 29}, //C
					   		  {23, 18, 89,  0, 87, 46, 75, 17, 50, 42}, //D
					   		  { 1,  3, 56, 87,  0, 55, 22, 86, 14, 33}, //E
					   		  { 5, 41, 21, 46, 55,  0, 21, 76, 54, 81}, //F
					   		  {23, 45, 12, 75, 22, 21,  0, 11, 57, 48}, //G
					   		  {56,  5, 48, 17, 86, 76, 11,  0, 63, 24}, //H
					   		  {12, 41, 14, 50, 14, 54, 57, 63,  0,  9}, //I
					   		  {11, 27, 29, 42, 33, 81, 48, 24,  9,  0}};//J

int main(){
	srand(time(NULL));
	double femIni = 1;
	int numH = 8;
	int numIter = 1000;

	double evaporacion = 0.9;
	double alfa = 1;
	double beta = 1;
	double Q = 1;
	double E = 5;


	AS as(costos, femIni, alfa, beta, Q, evaporacion, E, numH, numIter);
	as.run();
}