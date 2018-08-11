#include <iostream>
#include <fstream>
#include <tuple>
#include <cmath>
#include <map>
#include "FileOp.h"
#include "MyTime.h"

int main(int argc, char ** argv){
	
	if(argc != 2){
		cout<<"Faltan argumentos <file>"<<endl;
		return 0;
	}

	string fileName(argv[1]);

	auto bdInter = getBd("../ml/ml_10mP/ra.test", ';');

	cout<<"Generando vals"<<endl;
	map<int,map<int,double>> reales;

	int uid = 0;
	int mid = 0;
	double res = 0;

	for(auto iter = bdInter.begin(); iter != bdInter.end(); ++iter){
		uid = stoi((*iter)[0]);
		mid = stoi((*iter)[1]);
		res = stod((*iter)[2]);
		reales[uid][mid] = res;
	}

	ofstream outFile("resOut");

	ifstream inFile(fileName);

	string word = "";

	map<int,map<int,double>> predicciones;

	while(inFile>>word){
		uid = stoi(word);
		inFile>>word;
		mid = stoi(word);
		inFile>>word;
		res = stod(word);
		predicciones[uid][mid] = res;
	}

	inFile.close();

	double sum = 0;
	double count = 0;

	for(auto iter = predicciones.begin(); iter != predicciones.end(); ++iter){
		for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
			if(reales.find(iter->first) == reales.end()) continue;
			if(reales[iter->first].find(iter2->first) == reales[iter->first].end()) continue;
 			sum += abs(iter2->second - reales[iter->first][iter2->first]);
 			count++;
		}
	}

	double resU = sum / count;

	outFile<<sum<<endl;
	outFile<<count<<endl;
	outFile<<resU;

	outFile.close();

	cout<<sum<<endl;
	cout<<count<<endl;
	cout<<resU<<endl;

}
