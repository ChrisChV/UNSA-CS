#ifndef BD_H
#define BD_H

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include "Input.h"

using namespace std;

class BD{
	public:
		BD(string file, int numClass);
		tuple<vector<string>,vector<string>> splitLine(string line, int numClass);
		void print(){
			for(Input in : bdI) in.print();
			for(Input in : bdO) in.print();
		}

		vector<Input> bdI;
		vector<Input> bdO;

};

BD::BD(string file, int numClass){
	ifstream archivo(file.c_str());
	string line;
	while(archivo>>line){
		vector<string> SvalsI;
		vector<string> SvalsO;
		tie(SvalsI,SvalsO) = splitLine(line, numClass);
		vector<Val> valsI;
		vector<Val> valsO;
		valsI.push_back(1.0);
		for(string s : SvalsI) valsI.push_back(stod(s));
		bdI.push_back(Input(valsI));
		for(string s: SvalsO) valsO.push_back(stod(s));
		bdO.push_back(Input(valsO));
	}
}


tuple<vector<string>,vector<string>> BD::splitLine(string line, int numClass){
	vector<string> res1;
	vector<string> res2;
	string temp = "";
	string tempClass = "";
	bool flag = true;
	for(char c : line){
		if(c == ','){
			res1.push_back(temp);
			temp.clear();
		}
		else if(c == '\n'){
			tempClass = temp;
			flag = false;
			break;
		}
		else{
			temp.push_back(c);
		}
	}
	if(flag) tempClass = temp;
	for(int i = 0; i < numClass; i++){
		if(i == stoi(tempClass)) res2.push_back("1");
		else res2.push_back("0");
	}
	return make_tuple(res1,res2);
}

#endif