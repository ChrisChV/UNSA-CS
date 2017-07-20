#ifndef STENO_H
#define STENO_H

#include <iostream>
#include <fstream>
#include <vector>
#include "../Genetico/Genetico.h"
#include "LSB.h"
#include "../Utils/Utils.h"

using namespace std;

/**
	#define L_BLOCK 6
	#define N_LSB 3

	#define N_PROBLACION 30
	#define ITERATIONS 20 <-----
	20 2 primeros si se llega a la igualdad total.
	10 1 si llega igualdad total, 2 no llega igualdad total.

**/


#define NUMBER_CROM 1
#define NUMBER_DIV 2
#define NUMBER_FEN 1

#define L_BLOCK 8
#define N_LSB 3

#define N_PROBLACION 30
#define PROB_MUTACION 0.3
#define ITERATIONS 20


vector<string> vSTRl;
vector<string> vSTRm;
string STRm_actual;
int SIZE_CROM = 0;

void segmentation(string STRl, string STRm);
Val toDecimal(string num);
Val * funFen(Cromosoma * cromosomas);
Val funFit(Val * fenotipos);
void funMutar(Individuo & in);
vector<Individuo> funCross(Individuo madre, Individuo padre);
string hide(string msg, string fileImg);
string keyGeneration(vector<int> vKey);


struct ChoseOneExeption{
	ChoseOneExeption(string er){e = er;};
	string e;
};

void segmentation(string STRl, string STRm){
	string temp = "";
	int count = 0;
	for(char c : STRl){
		count++;
		temp.push_back(c);
		if(count == L_BLOCK){
			vSTRl.push_back(temp);
			temp.clear();
			count = 0;
		}
	}
	if(count != 0){
		vSTRl.push_back(temp);
		temp.clear();
		count = 0;
	}
	for(char c : STRm){
		count++;
		temp.push_back(c);
		if(count == L_BLOCK){
			vSTRm.push_back(temp);
			temp.clear();
			count = 0;
		}	
	}
	if(count != 0){
		vSTRm.push_back(temp);
		temp.clear();
		count = 0;
	}
}

Val toDecimal(string num){
	Val res = 0;
	for(int i = 0; i < num.size(); i++){
		if(num[i] == '1') res += pow(2,num.size() - 1 - i);
	}
	return res;
}

Val * funFen(Cromosoma * cromosomas){
	Val * res = new Val[1];
	res[0] = cromosomas[0].binaryToDecimal();
	return res;
}

Val funFit(Val * fenotipos){
	Val res = 0;
	if(fenotipos[0] >= vSTRl.size() - 1) return -1;
	string a(vSTRl[fenotipos[0]]);
	for(int i = 0; i < L_BLOCK; i++){
		if(a[i] == STRm_actual[i]) res++;
	}
	return res;
}

void funMutar(Individuo & in){
	int bit = rand() % SIZE_CROM;
	if(in.cromosomas[0].vector[bit] == '0'){
		in.cromosomas[0].vector[bit] = '1';
	}
	else in.cromosomas[0].vector[bit] = '0';
}

vector<Individuo> funCross(Individuo madre, Individuo padre){
	vector<Individuo> res;
	int corte1 = rand() % (SIZE_CROM - 1) + 1;
	int corte2 = SIZE_CROM - corte1;
	char * cortesMadre[NUMBER_CROM][NUMBER_DIV];
	char * cortesPadre[NUMBER_CROM][NUMBER_DIV];

	for(int i = 0; i < NUMBER_CROM; i++){
		cortesMadre[i][0] = (char *) malloc(corte1 + 1);
		cortesMadre[i][1] = (char *) malloc(corte2 + 1);
		cortesPadre[i][0] = (char *) malloc(corte1 + 1);
		cortesPadre[i][1] = (char *) malloc(corte2 + 1);
//		cout<<"VECM->"<<madre.cromosomas[i]<<endl;
//		cout<<"VECP->"<<padre.cromosomas[i]<<endl;
		memcpy(cortesMadre[i][0],madre.cromosomas[i].vector,corte1);
		memcpy(cortesMadre[i][1],&(madre.cromosomas[i].vector[corte1]),corte2);
		memcpy(cortesPadre[i][0],padre.cromosomas[i].vector,corte1);
		memcpy(cortesPadre[i][1],&(padre.cromosomas[i].vector[corte1]),corte2);
		cortesMadre[i][0][corte1] = '\0';
		cortesMadre[i][1][corte2] = '\0';
		cortesPadre[i][0][corte1] = '\0';
		cortesPadre[i][1][corte2] = '\0';
	}



	Cromosoma * cromosomas1 = new Cromosoma[NUMBER_CROM];

	cromosomas1[0] = Cromosoma(SIZE_CROM,cortesPadre[0][0], cortesMadre[0][1]);
	res.push_back(Individuo(cromosomas1,NUMBER_CROM,NUMBER_FEN,funFen,funFit));

	
	for(int i = 0; i < NUMBER_CROM; i++){
		for(int j = 0; j < NUMBER_DIV; j++){
			free(cortesMadre[i][j]);
			free(cortesPadre[i][j]);
		}
	}

	
	return res;
}


string hide(string msg, string fileImg, string keyFile){
	srand(time(NULL));
	vector<int> resKey;
	vector<bool> repeat;
	string STRl = "";
	string key = "";
	string temp = "";
	string img0 = "";
	string img1 = "";
	string img2 = "";
	tie(img0, img1, img2) = getLSBfromImg(fileImg, N_LSB);
	segmentation(img0, getStringMSG(msg));
	temp = toBinary(vSTRl.size());
	SIZE_CROM = temp.size();
	vector<int> sizeCromos = {SIZE_CROM};
	repeat = vector<bool>(vSTRl.size());
	Genetico gen;
	int choseOne = 0;
	int noChoseOneCount = 0;
	bool flag = 0;
	for(int i = 0; i < vSTRm.size(); i++){
		STRm_actual = vSTRm[i];
		gen = Genetico(N_PROBLACION, sizeCromos, NUMBER_FEN, PROB_MUTACION, funFen, funFit, funMutar, funCross);
		gen.run(ITERATIONS);
		for(int j = 0; j < N_PROBLACION; j++){
			choseOne = gen.poblacion[j].fenotipos[0];
			if(repeat[choseOne] == 0){
				repeat[choseOne] = 1;
				vSTRl[choseOne] = vSTRm[i];
				flag = 0;
				break;
			}
			else{
				if(gen.poblacion[j].fitness == L_BLOCK){
					flag = 0;
					break;		
				}
			}
			flag = 1;
		}
		if(flag == 1){
			flag = 0;
			noChoseOneCount++;
			for(int j = 0; j < repeat.size(); j++){
				if(repeat[j] == 0){
					flag = 1;
					choseOne = j;
					repeat[choseOne] = 1;
					vSTRl[choseOne] = vSTRm[i];
					break;
				}
			}
			if(flag == 0){
				throw(ChoseOneExeption(vSTRm[i]));
			}
		}
		resKey.push_back(choseOne);
	}
	
	//for(int i = 0; i < vSTRm.size(); i++){
		//vSTRl[resKey[i]] = vSTRm[i];
	//}

	for(string s : vSTRl){
		STRl += s;
	}

	string outImg = "out" + fileImg;

	changeImg(make_tuple(STRl,img1,img2),fileImg,N_LSB,outImg);

	for(int i : resKey){
		key += to_string(i) + " ";
	}

	key += "$";

	ofstream archivo(keyFile.c_str());
	archivo<<key<<endl;
	archivo.close();


	cout<<"No Chose One->"<<noChoseOneCount<<endl;

	return key;
}

string reveal(string keyFile, string fileImg){
	vector<int> key;
	ifstream archivo(keyFile);
	string temp = "";
	string res = "";
	while(archivo>>temp){
		if(temp == "$") break;
		key.push_back(stoi(temp));
	}
	archivo.close();
	string img0 = "";
	string img1 = "";
	string img2 = "";
	tie(img0, img1, img2) = getLSBfromImg(fileImg, N_LSB);
	segmentation(img0, "");

	string msgBin = "";

	for(int i : key){
		msgBin += vSTRl[i];
	}

	int count = 0;
	temp = "";

	for(char c : msgBin){
		count++;
		temp.push_back(c);
		if(count == BITS_PER_PIXEL){
			res.push_back(toDecimal(temp));
			temp.clear();
			count = 0;
		}
	}

	return res;



}



#endif
