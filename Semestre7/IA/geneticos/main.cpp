#include <iostream>
#include <cstring>
#include <cstdlib>
#include "genetico.h"
#include "graficador.h"

#define A 10000
#define centerX 4096
#define centerY 4096
#define oX 1500
#define oY 1500

using namespace std;

Val gauss(Val x, Val y){
	return A * exp(-( pow(x-centerX,2) / (2*pow(oX,2)) + pow(y-centerY,2) / (2*pow(oY,2))));
}

int main(){
	srand(time(NULL));
//	vector<float> vec = {0.52,0.63,0.64,0.75,1};
	//Val x = ((Val) rand() / RAND_MAX);
	//cout<<x<<endl;
//	cout<<binary(vec,0.75,0,vec.size()-1)<<endl;
	Genetico gen(30,13,0.3,gauss,"points.tsv");
	gen.run(20,"imgA","imgB");
	system("convert -delay 50 -loop 0 imagenes/*.png vid.gif");	
}