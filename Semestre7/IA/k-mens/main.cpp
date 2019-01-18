#include <iostream>
#include "k_means.h"

using namespace std;

Points getPoints(string file, int n, int dim){
	Points res(n,dim);
	ifstream archivo(file.c_str());
	float p = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < dim; j++){
			archivo>>p;
			res[i][j] = p;
		}
	}
	archivo.close();
	return res;
}

int main(){
	srand(time(NULL));
	//Points points = generateRandom(100, 2);
	
	Points points = getPoints("iris.dat", 150, 4);
	points.print();

	int * colors;
	Points centroids;
	tie(colors,centroids) = k_means(points,3);
	savePoints(points,colors,"points.dat");
	system("python2 graficador.py 3");
}