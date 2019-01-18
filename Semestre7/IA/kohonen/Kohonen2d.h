#ifndef KOHONEN_H
#define KOHONEN_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <fstream>
#include <omp.h>

#define RANDOM_RANGE 2
#define DIMENSIONES 2

omp_lock_t mutex;

using namespace std;

typedef double Val;

typedef struct{
	int dim[DIMENSIONES];
}Winner;

class NeuroMap{
	public:
		NeuroMap(int dimension, int numInputs, Val radioInicial, Val LearningInicial, int numIterations, 
			string file, string test);
		NeuroMap(string pesosFile, string clasesFile, string learning, string test);
		void changeColor(Val radio, Winner win, int index);  
		Winner getWin(Val * intput);
		Val getDistance(Val * a, Val * b);
		Val getDistance(Winner a, Winner b);
		void print();
		void printWin(Winner win);
		void printTraining();
		void printImg();
		void printTrainingImg(vector<vector<string>>  vec);
		void changePesos(Winner win, Val * in, int index);
		void changeTest(string test);
		vector<vector<string>> run();
		Winner test();
		Val * splitLine(string line);
		vector<Val *> getTraining(string file);
		Val getNewRadius();
		Val getNewLearning();
		Val getTheta(Winner win, Winner actual, Val radio);
		string ** allocMatrix();
		void runTests();
		void printClases();
		void save(string pesos, string clases);
		void load(string pesos, string clases);

		int NMsize;
		int Isize;
		int numIterations;
		int iteracionActual;
		Val LearningInicial;
		Val radioInicial;
		Val lambda;
		Val *** cube;
		vector<Val *> training;
		vector<Val *> tests;
		vector<int> colores;
		vector<vector<int>> clases;
};


void NeuroMap::save(string pesos, string clases){
	ofstream archivoPesos(pesos.c_str());
	ofstream archivoClases(clases.c_str());
	archivoPesos<<NMsize<<" "<<Isize<<" "<<radioInicial<<" "<<LearningInicial;
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < Isize; k++){
				archivoPesos<<cube[i][j][k]<<endl;
			}
		}
	}
	archivoPesos.close();
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			archivoClases<<this->clases[i][j]<<endl;
		}
	}
	archivoClases.close();
}

void NeuroMap::load(string pesos, string clases){
	ifstream archivoPesos(pesos.c_str());
	ifstream archivoClases(clases.c_str());
	archivoPesos>>NMsize;
	archivoPesos>>Isize;
	archivoPesos>>radioInicial;
	archivoPesos>>LearningInicial;
	cube = (Val ***) malloc(sizeof(void **) * NMsize);
	for(int i = 0; i < NMsize; i++){
		cube[i] = (Val **) malloc(sizeof(void *) * NMsize);
		for(int j = 0; j < NMsize; j++){
			cube[i][j] = (Val *) malloc(sizeof(Val) * Isize);
		}
	}
	this->clases = vector<vector<int>>(NMsize);
	for(int i = 0; i < NMsize; i++){
		this->clases[i] = vector<int>(NMsize);
	}
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < Isize; k++){
				archivoPesos>>cube[i][j][k];
			}
		}
	}
	archivoPesos.close();
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			archivoClases>>this->clases[i][j];
		}
	}
	archivoClases.close();
}

void NeuroMap::printClases(){
	for(vector<int> v : clases){
		for(int i : v){
			cout<<i<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void NeuroMap::runTests(){
	Winner win;
	Val * in;
	int matrix[7][8];
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 8; j++){
			matrix[i][j] = 0;
		}
	}
	for(int i = 0; i < tests.size(); i++){
		in = tests[i];
		win = getWin(in);
		int indexI = colores[training.size() + i];
		int indexJ = clases[win.dim[0]][win.dim[1]];
		if(indexJ == 0) matrix[indexI - 1][7]++;
		else matrix[indexI - 1][indexJ - 1]++;
	}
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 8; j++){
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
}

void NeuroMap::changeTest(string test){
	tests = getTraining(test);
}


NeuroMap::NeuroMap(string pesosFile, string clasesFile, string learning, string test){
	load(pesosFile,clasesFile);
	lambda = (double) (numIterations) / radioInicial;
	iteracionActual = 0;
	training = getTraining(learning);
	tests = getTraining(test);
}

NeuroMap::NeuroMap(int dimension, int numInputs, Val radioInicial, Val LearningInicial, int numIterations,
				 string file, string test){
	srand(time(NULL));
	clases = vector<vector<int>>(dimension);
	for(int i = 0; i < dimension; i++){
		clases[i] = vector<int>(dimension);
	}
	for(int i = 0; i < dimension; i++){
		for(int j = 0; j < dimension; j++){
			clases[i][j] = 0;
		}
	}
	NMsize = dimension;
	Isize = numInputs;
	this->radioInicial = radioInicial;
	this->numIterations = numIterations;
	this->LearningInicial = LearningInicial;
	this->training = training;
	lambda = (double) (numIterations) / radioInicial;
	iteracionActual = 0;
	training = getTraining(file);
	tests = getTraining(test);
	cube = (Val ***) malloc(sizeof(void **) * dimension);
	for(int i = 0; i < NMsize; i++){
		cube[i] = (Val **) malloc(sizeof(void *) * dimension);
		for(int j = 0; j < NMsize; j++){
			cube[i][j] = (Val *) malloc(sizeof(Val) * Isize);
		}
	}
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < Isize; k++){
				cube[i][j][k] = rand() % RANDOM_RANGE;
			}
		}
	}
}


vector<Val *> NeuroMap::getTraining(string file){
	vector<Val *> Vres;
	ifstream archivo(file.c_str());
	string temp = "";
	while(archivo>>temp){
		Val * res = (Val *) malloc(sizeof(Val) * Isize);
		res[0] = stod(temp);
		for(int i = 1; i < Isize; i++){
			archivo>>temp;
			res[i] =stod(temp);
		}
		Vres.push_back(res);
		archivo>>temp;
		colores.push_back(stod(temp));
	}
	archivo.close();
	return Vres;
}

void NeuroMap::print(){
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			for(int k = 0; k < Isize; k++){
					cout<<cube[i][j][k]<<" ";
			}
			cout<<endl;
		}
		cout<<endl<<endl;
	}
	cout<<endl;
}

void NeuroMap::printWin(Winner win){
	cout<<"Dim1->"<<win.dim[0]<<endl;
	cout<<"Dim2->"<<win.dim[1]<<endl;
	for(int i = 0; i < Isize; i++){
		cout<<cube[win.dim[0]][win.dim[1]][i]<<" ";
	}
	cout<<endl;
}

void NeuroMap::printImg(){
	for(int i = 0; i < training.size(); i++){
		cout<<colores[i]<<endl;
	}
}

void NeuroMap::printTraining(){
	for(Val * v : training){
		for(int i = 0; i < Isize; i++){
			cout<<v[i]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void NeuroMap::printTrainingImg(vector<vector<string>> vec){
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
				cout<<vec[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

Val NeuroMap::getDistance(Val * a, Val * b){
	Val res = 0;
	for(int i = 0; i < Isize; i++) res += pow(a[i] - b[i],2);
	return sqrt(res);
}

Val NeuroMap::getDistance(Winner a, Winner b){
	Val res = 0;
	for(int i = 0; i < DIMENSIONES; i++) res += pow(a.dim[i] - b.dim[i],2);
	return sqrt(res);
}

Winner NeuroMap::getWin(Val * inputs){
	Winner res;
	res.dim[0] = -1;
	res.dim[1] = -1;
	Val menor = -1;
	Val temp = 0;
	#pragma omp parallel for num_threads(4) private(temp)
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			temp = getDistance(inputs, cube[i][j]);
			omp_set_lock(&mutex);
			if(menor > temp or menor == -1){
				menor = temp;
				res.dim[0] = i;
				res.dim[1] = j;
			}
			omp_unset_lock(&mutex);
		}
	}
	return res;
}


Val NeuroMap::getNewRadius(){
	return radioInicial * exp( (double) (-iteracionActual) / lambda);
}

Val NeuroMap::getNewLearning(){
	return LearningInicial * exp((double) (-iteracionActual) / lambda);
}

Val NeuroMap::getTheta(Winner win, Winner actual, Val radio){
	Val distancia = getDistance(win, actual);
	return exp(-(distancia*distancia) / (2.0 * radio * radio));
}


void NeuroMap::changeColor(Val radio, Winner win, int index){
	Winner actual;
	int actualColor = colores[index];
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			actual.dim[0] = i;
			actual.dim[1] = j;
			if(getDistance(win, actual) < radio){
				clases[i][j] = actualColor;
			}
		}
	}
}

void NeuroMap::changePesos(Winner win, Val * in, int index){
	Val radio = getNewRadius();
	Val Learning = getNewLearning();
	Val thetha = 0;
	Winner actual;
	changeColor(radio,win, index);
	for(int i = 0; i < NMsize; i++){
		for(int j = 0; j < NMsize; j++){
			actual.dim[0] = i;
			actual.dim[1] = j;
			thetha = getTheta(win, actual, radio);
			if(getDistance(win, actual) <= radio){
				for(int k = 0; k < Isize; k++){
					cube[i][j][k] += thetha * Learning * (in[k] - cube[i][j][k]);
				}
			}
		}
	}
	iteracionActual++;
	cout<<iteracionActual<<"->"<<radio<<endl;
}


vector<vector<string>> NeuroMap::run(){
	srand(time(NULL));

	//string ** res = allocMatrix();
	vector<vector<string>> res(NMsize);
	for(int i = 0; i < NMsize; i++){
		res[i] = vector<string>(NMsize);
		for(int j = 0; j < NMsize; j++){
			res[i][j] = "null";
		}
	}
	Winner win;
	Val * randIn;
	int random = 0;
	for(int i = 0; i < numIterations; i++){
		random = rand() % training.size();
		randIn = training[random];
		win = getWin(randIn);
		//res[win.dim[0]][win.dim[1]] = string(imagenes[random]);
		changePesos(win, randIn, random);
	}
	return res;
}

Winner  NeuroMap::test(){
	srand(time(NULL));
	Val * randIn;
	int random = rand() % tests.size();
	randIn = tests[random];
	return getWin(randIn);
}

#endif