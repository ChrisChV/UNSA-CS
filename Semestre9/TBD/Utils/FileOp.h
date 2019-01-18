#ifndef FILEOP_H
#define FILEOP_H

#include <iostream>
#include <fstream>
#include <string>
#include "Utils.h"

vector<ValVec> getBdVals(string file, int n){
	ifstream valsFile(file.c_str());
	vector<ValVec> vals(n);
	string word = "";
	int count = 0;
	while(valsFile>>word){	
		if(word == "-") vals[count].push_back(NULL_VAL);
		else vals[count].push_back(stoVT(word));
		count++;
		if(count == n) count = 0;
	}
	valsFile.close();
	return vals;
}

vector<string> getBdNames(string file){
	vector<string> names;
	ifstream namesFile(file.c_str());
	string word = "";
	while(namesFile>>word){
		names.push_back(word);
	}
	namesFile.close();
	return names;
}





/*
template <class Register>
void setInRegister(Register & tupleR, vector<string> & s_vec, const int index){
	int size = tuple_size<Register>::value;
	if(index == size) return;
	get<index>(tupleR) = sToNum<tuple_element<index,Register>::type>(s_vec[index]);
	setInRegister<Register>(tupleR, s_vec, index + 1);
}

template <class Register>
void setInRegister(Register & tupleR, vector<string> & s_vec){
	setInRegister<Register>(tupleR, s_vec, 0);
}

template <class Register>
vector<Register> getBd(string fileName, char delimit){
	vector<Register> res;
	vector<string> tempVectS;
	ifstream bdFile(fileName.c_str());
	string word = "";
	char line[512];
	while(bdFile.getline(line,512)){
		res.push_back(Register());
		word = string(line);
		tempVectS = splitString(word, delimit);
		for(int i = 0; i < tempVectS.size(); i++){
			setInRegister<Register>(res.back(), tempVectS);
			//setInRegister(res.back(), tempVectS[i], i);
			//get<i>(res.back()) = sToNum<tuple_element<i,Register>::type>(tempVectS[i]);
		}
	}
	bdFile.close();
	return res;
}
*/


vector<Registro> getBd(string fileName, char delimit){
	vector<Registro> res;
	ifstream bdFile(fileName.c_str());
	string word = "";
	char line[512];
	while(bdFile.getline(line,512)){
		word = string(line);
		res.push_back(splitString(word, delimit));
	}
	bdFile.close();
	return res;
}

vector<Registro> getBd(string fileName, char delimit, int left_limit, int rigth_limit){
	vector<Registro> res;
	ifstream bdFile(fileName.c_str());
	string word = "";
	char line[512];
	int count = 0;
	while(bdFile.getline(line,512)){
		count++;
		if(count - 1 < left_limit) continue;
		if(count == rigth_limit) break;
		word = string(line);
		res.push_back(splitString(word, delimit));
	}
	bdFile.close();
	return res;
}

#endif