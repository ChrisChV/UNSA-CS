#include <iostream>
#include <iomanip>
#include "RN.h"

using namespace std;



int main(){
	cout<<setprecision(30);
	//Input in(6);
	//vector<Val> ov = {1,0,0,1,0};
	//Input out(ov);
	//vector<int> capas = {6,5};
	//RedNeuronal rn(in,out,capas);
	//rn.run();

	int numInputs = 5;
	int numClass = 3;
	vector<int> capas = {8,3};
	RedNeuronal RN(numInputs,capas);
	clock_t ini = clock();
	RN.run("irisN.dat",numClass ,20000);
	RN.verify("testIrisN.dat", numClass);
	clock_t end = clock();
	double seconds = (double) (end - ini) / CLOCKS_PER_SEC;
	cout<<"Time->"<<seconds<<endl;


}


	