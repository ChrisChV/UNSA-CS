#ifndef MAPPING_H
#define MAPPING_H

#include <iostream>
#include <string>

using namespace std;

enum PROCESS_ENUM {PROCESO_LOTES,PROCESO_SISTEMA,PROCESO_NORMAL};
enum ALGORITMOS_ENUM {FIFO, FIFO2, RR};

int getProssesCode(string word){
	if(word == "Lotes") return PROCESO_LOTES;
	if(word == "Sistema") return PROCESO_SISTEMA;
	if(word == "Normal") return PROCESO_NORMAL;
	return -1;
}


int getAlgorithmCode(string word){
	if(word == "FIFO") return FIFO;
	if(word == "FIFO2") return FIFO2;
	bool flag = false;
	int i = 0;
	string temp = "";
	string cuantum = "";
	for(i; i < word.size(); i++){
		if(word[i] == '-') flag = true;
		else if(!flag) temp.push_back(word[i]);
		else cuantum.push_back(word[i]);
	}
	if(flag and temp == "RR"){
		cout<<cuantum<<endl;
		int cu = stoi(cuantum);
		if(cu != 0) return RR + cu;
	}
	return -1;
}

#endif