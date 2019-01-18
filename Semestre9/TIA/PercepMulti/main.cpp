#include <iostream>
#include <iomanip>
#include "RN.h"
#include "functions.h"

using namespace std;


//Signoide 0
//Tangente 1
//ReluFunc 2

int main(int argc, char ** argv){
	cout<<setprecision(15);
	if(argc != 5){
		cout<<"Faltan Argumentos <fileConfig> <fileTraining> <fileTest> <iteraciones>"<<endl;
		return 0;
	}
	string fileConfigName(argv[1]);
	string fileTrainingName(argv[2]);
	string fileTestName(argv[3]);
	string s_iter(argv[4]);
	int numIter = stoi(s_iter);

	vector<ActsFuns> funciones = {SignoideFunc, TangenteHFunc, ReluFunc};

	ifstream fileConfig(fileConfigName);
	int numInputs = 0;
	int numClass = 0;
	int numCapas = 0;
	int capaActual = 0;
	int funActual = 0;
	fileConfig>>numInputs;
	fileConfig>>numClass;
	fileConfig>>numCapas;
	vector<int> capas;
	vector<ActsFuns> actVec;
	vector<string> classNames;
	string tempS = "";
	for(int i = 0; i < numCapas; i++){
		fileConfig>>capaActual;
		capas.push_back(capaActual);
	}
	for(int i = 0; i < numCapas; i++){
		fileConfig>>funActual;
		actVec.push_back(funciones[funActual]);
	}
	for(int i = 0; i < numClass; i++){
		fileConfig>>tempS;
		classNames.push_back(tempS);
	}
	RedNeuronal RN(numInputs, capas, actVec);
	clock_t ini = clock();
	RN.run(fileTrainingName, numClass , numIter);
	RN.verify(fileTestName, classNames);
	clock_t end = clock();
	double seconds = (double) (end - ini) / CLOCKS_PER_SEC;
	cout<<"Time->"<<seconds<<endl;
}

/*
int main(){
	cout<<setprecision(15);
	//Input in(6);
	//vector<Val> ov = {1,0,0,1,0};
	//Input out(ov);
	//vector<int> capas = {6,5};
	//RedNeuronal rn(in,out,capas);
	//rn.run();

	int numInputs = 5;
	int numClass = 3;
	
	
	//vector<int> capas = {8,3};
	//vector<ActsFuns> actVec = {ReluFunc, ReluFunc};
	

	vector<int> capas = {8,5,3};
	vector<ActsFuns> actVec = {ReluFunc, SignoideFunc, SignoideFunc};

	RedNeuronal RN(numInputs, capas, actVec);
	clock_t ini = clock();
	RN.run("irisN.dat", numClass , 1000);
	RN.verify("testIrisN.dat", numClass);
	clock_t end = clock();
	double seconds = (double) (end - ini) / CLOCKS_PER_SEC;
	cout<<"Time->"<<seconds<<endl;
}
*/
