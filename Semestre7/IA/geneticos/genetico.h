#ifndef GENETICO_H
#define GENETICO_H
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <fstream>
#include "Individuo.h"
#include "graficador.h"
#include "sad.h"


//TODO INicializar el srand(time(NULL)) en el main

using namespace std;

class Genetico{
	public:
		Genetico(){};
		Genetico(int sizePoblacion, int sizeCromo, Val probMutacion, FunFitness funFit, string points);
		Individuo run(int iteraciones, string prefixImg1, string prefixImg2);
		int binary(Val e, int ini, int end);
		void selection();
		Pareja emparejar();
		void crossover(int madre, int padre, vector<Individuo> &res);
		void mutarPobl(vector<Individuo> &nextGeneration);
		void mutar(Individuo &in);
		void masacre(vector<Individuo> &nextGeneration);
		void print();
		void print(vector<Individuo> &nextGeneration);
		void savePoints();
		void graficar(string prefix, int num, string angle1, string angle2);
		FunFitness funFit;
		vector<Individuo> poblacion;
		Val probMutacion;
		string points;
		int size;
		int sizeCromo;
};

Genetico::Genetico(int sizePoblacion, int sizeCromo, Val probMutacion, FunFitness funFit, string points){
	this->size = sizePoblacion;
	this->sizeCromo = sizeCromo;
	this->probMutacion = probMutacion;
	this->funFit = funFit;
	this->points = points;
	for(int i = 0; i < size; i++){
		poblacion.push_back(Individuo(sizeCromo,funFit));
	}
}

void Genetico::savePoints(){
	ofstream archivo(points.c_str());
	for(Individuo i : poblacion){
		archivo<<i.fenotipo[0]<<" "<<i.fenotipo[1]<<" "<<i.fitness<<endl;
	}
	archivo.close();
}

int Genetico::binary(Val e, int ini, int end){
	if(ini == end){
		if(poblacion[ini].probSelecAcum >= e) return ini;
		return ini + 1; 
	}
	if(end - ini == 1){
		if(poblacion[end].probSelecAcum <= e) return end + 1;
		if(poblacion[ini].probSelecAcum < e) return end;
		return ini;
	}
	int med = (end + ini) / 2;
	if(poblacion[med].probSelecAcum == e) return med;
	if(poblacion[med].probSelecAcum > e) return binary(e,ini,med - 1);
	return binary(e,med,end);
}

Pareja Genetico::emparejar(){
	Val randMadre = ((Val) rand() / RAND_MAX);
	Val randPadre = ((Val) rand() / RAND_MAX);
	int madre = binary(randMadre, 0, size - 1);
	int padre = binary(randPadre, 0, size - 1);
	cout<<randMadre<<" "<<randPadre<<" "<<madre<<" "<<padre<<endl;
	return make_tuple(madre,padre);
}

void Genetico::crossover(int madre, int padre, vector<Individuo> &res){
	int corte1 = rand() % (sizeCromo - 1) + 1;
	int corte2 = sizeCromo - corte1;
	cout<<"Corte1->"<<corte1<<endl<<"Corte2->"<<corte2<<endl;
	char * cortesMadre[NUMBER_CROM][NUMBER_DIV];
	char * cortesPadre[NUMBER_CROM][NUMBER_DIV];
	for(int i = 0; i < NUMBER_CROM; i++){
		cortesMadre[i][0] = (char *) malloc(corte1 + 1);
		cortesMadre[i][1] = (char *) malloc(corte2 + 1);
		cortesPadre[i][0] = (char *) malloc(corte1 + 1);
		cortesPadre[i][1] = (char *) malloc(corte2 + 1);
		memcpy(cortesMadre[i][0],poblacion[madre].cromosoma[i].vector,corte1);
		memcpy(cortesMadre[i][1],&(poblacion[madre].cromosoma[i].vector[corte1]),corte2);
		memcpy(cortesPadre[i][0],poblacion[padre].cromosoma[i].vector,corte1);
		memcpy(cortesPadre[i][1],&(poblacion[padre].cromosoma[i].vector[corte1]),corte2);
		cortesMadre[i][0][corte1] = '\0';
		cortesMadre[i][1][corte2] = '\0';
		cortesPadre[i][0][corte1] = '\0';
		cortesPadre[i][1][corte2] = '\0';
	}
	if(madre == padre){
		res.push_back(Individuo(sizeCromo, cortesMadre[0][0], cortesPadre[1][1], cortesMadre[1][0], cortesPadre[0][1], funFit));
		res.back().print();
		res.push_back(Individuo(sizeCromo, cortesMadre[0][0], cortesPadre[1][1], cortesMadre[1][0], cortesPadre[0][1], funFit));
		res.back().print();	
	}
	else{
		res.push_back(Individuo(sizeCromo, cortesMadre[0][0], cortesPadre[0][1], cortesMadre[1][0], cortesPadre[1][1], funFit));
		res.back().print();
		res.push_back(Individuo(sizeCromo, cortesPadre[0][0], cortesMadre[0][1], cortesPadre[1][0], cortesMadre[1][1], funFit));
		res.back().print();	
	}
	
	for(int i = 0; i < NUMBER_CROM; i++){
		for(int j = 0; j < NUMBER_DIV; j++){
			free(cortesMadre[i][j]);
			free(cortesPadre[i][j]);
		}
	}
	

}

void Genetico::mutar(Individuo &in){
	int cromosoma = rand() % NUMBER_CROM;
	int bit = rand() % sizeCromo;
	if(in.cromosoma[cromosoma].vector[bit] == '0'){
		in.cromosoma[cromosoma].vector[bit] = '1';
	}
	else in.cromosoma[cromosoma].vector[bit] = '0';
	in.reset(funFit);
}

void Genetico::mutarPobl(vector<Individuo> &nextGeneration){
	float random = 0;
	for(int i = 0 ; i < nextGeneration.size(); i++){
		random = ((Val) rand() / RAND_MAX);
		if(random <= probMutacion){
			mutar(nextGeneration[i]);	
		} 
	}
}

void Genetico::masacre(vector<Individuo> &nextGeneration){
	sort(nextGeneration.begin(), nextGeneration.end(), funSortIn);
	cout<<endl;
	print(nextGeneration);
	auto ini = nextGeneration.begin();
	auto end = nextGeneration.begin() + size;
	poblacion = vector<Individuo>(ini,end);
}

void Genetico::selection(){
	Val fitnessTotal = 0;
	Val probSelecTotal = 0;
	vector<Individuo> nextGeneration;
	for(Individuo i : poblacion){
		fitnessTotal += i.fitness;
	}
	for(int i = 0; i < size; i++){
		poblacion[i].probSelec = poblacion[i].fitness / fitnessTotal;
		probSelecTotal += poblacion[i].probSelec;
		poblacion[i].probSelecAcum = probSelecTotal;
	}
	print();
	for(int i = 0; i < size; i++){
		int padre = 0;
		int madre = 0;
		tie(madre,padre) = emparejar();
		crossover(madre,padre,nextGeneration);
	}
	cout<<endl;
	print(nextGeneration);
	mutarPobl(nextGeneration);
	masacre(nextGeneration);
	cout<<endl;
	print();
}


Individuo Genetico::run(int iteraciones, string prefixImg1, string prefixImg2){
	int count = 0;
	savePoints();
	graficar(prefixImg1,count, "40", "30");
	graficar(prefixImg2,count, "10", "30");
	for(int i = 0; i < iteraciones; i++){
		cout<<"I->"<<i<<endl;
		selection();
		savePoints();
		count++;
		graficar(prefixImg1,count, "40" ,"30");
		graficar(prefixImg2,count, "10" ,"30");
	}
	return poblacion[0];
}

void Genetico::graficar(string prefix, int num, string angle1, string angle2){
	string postfix = "";
	if(num < 10) postfix = "00" + to_string(num);
	else if(num < 100) postfix = "0" + to_string(num);
	else postfix = to_string(num);
	string img = prefix + postfix + ".png";
	graficarGauss(img, points, angle1, angle2);
}

void Genetico::print(){
	for(Individuo i : poblacion){
		i.print();
	}
}

void Genetico::print(vector<Individuo> &nextGeneration){
	for(Individuo i : nextGeneration){
		i.print();
	}	
}


#endif