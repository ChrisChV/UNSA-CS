#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <tuple>
#include <cmath>
#include <array>
#include <fstream>

using namespace std;

#define RANDOM_RANGE 100

typedef float Value;
typedef float Dist;

struct Points{
	Value ** m;
	int fil;
	int col;
	Points(){
		m = nullptr;
		fil = 0;
		col = 0;
	}
	Points(int fil, int col){
		this->fil = fil;
		this->col = col;
		m = new Value *[fil];
		for(int i = 0; i < fil; i++) m[i] = new Value[col];
	}
	void print(){
		for(int i = 0; i < fil; i++){
			for(int j = 0; j < col; j++){
				cout<<m[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	Value *& operator[](int i){ return m[i];}
};

bool verifyCentroids(Points centroids, Points centroidsAns){
	for(int i = 0; i < centroids.fil; i++){
		for(int j = 0; j < centroids.col; j++){
			if(centroids[i][j] != centroidsAns[i][j]) return false; 
		}
	}
	return true;
}

void copyCentroids(Points centroids, Points &centroidsAns){
	for(int i = 0; i < centroids.fil; i++){
		memcpy(centroidsAns[i],centroids[i],sizeof(Value) * centroids.col);
	}
}

void selectRandom(Points points, Points &centroids){
	for(int i = 0; i < centroids.fil; i++){
		memcpy(centroids[i],points[rand() % points.fil],sizeof(Value) * points.col);
	}
}

Dist distance(Value * a, Value * b, int dim){
	Dist res = 0;
	for(int i = 0; i < dim; i++){
		res += pow(b[i] - a[i],2);
	}
	return sqrt(res);
}

void getNewMeans(Points points, Points &centroids, int * colors){
	int color = -1;
	int counts[centroids.fil];
	for(int i = 0; i < centroids.fil; i++){
		counts[i] = 0;
		for(int j = 0; j < centroids.col; j++){
			centroids[i][j] = 0;
		}
	}
	for(int i = 0; i < points.fil; i++){
		color = colors[i];
		counts[color]++;
		for(int j = 0; j < points.col; j++){
			centroids[color][j] += points[i][j];
		}
	}
	for(int i = 0; i < centroids.fil; i++){
		for(int j = 0; j < centroids.col; j++){
			centroids[i][j] /= counts[i];
		}
	}
}

tuple<int *,Points> k_means(Points points, int k){
	int num = points.fil;
	int dim = points.col;
	Points centroids(k,dim);
	Points centroidsAns(k,dim);
	int * colors = new int[num];
	int k_menor = -1;
	Dist distance_menor = -1;
	Dist distance_temp = 0;
	selectRandom(points, centroids);
	points.print();
	do{	
		copyCentroids(centroids,centroidsAns);
		cout<<endl;
		centroids.print();
		cout<<endl;
		for(int i = 0; i < num; i++){
			k_menor = -1;
			distance_menor = -1;
			for(int j = 0; j < k; j++){
				distance_temp = distance(points[i],centroids[j],dim);
				cout<<"Dist->"<<j<<"->"<<distance_temp<<endl;
				if(distance_menor == -1 or distance_menor > distance_temp){
					k_menor = j;
					distance_menor = distance_temp;
				}
			}
			colors[i] = k_menor;
		}

		for(int i = 0; i < num; i++){
			cout<<colors[i]<<" ";
		}
		cout<<endl;

		getNewMeans(points, centroids, colors);
	}while(!verifyCentroids(centroids,centroidsAns));
	return make_tuple(colors,centroids);
}

/*
void confusionMatrix(Points points, Points centroidsAns){
	int matrix[centroids.fil][centroids.fil];
	for(int i = 0; i < centroids.fil; i++){
		for(int j = 0; j < centroids.fil; j++){
			matrix[i][j] = 0;
		}
	}
	Dist menor = -1;
	Dist temp = 0;
	int k_menor = -1;

	for(int i = 0; i < points.fil; i++){
		menor = -1;
		for(int j = 0; j < centroids.fil; j++){
			temp = distance(points[i],centroids[j],points.col);
			if(temp == -1 or menor > temp){
				menor = temp;
				k_menor = j;
			}
		}

	}

}
*/

Points generateRandom(int num, int dim){
	Points res(num,dim);
	for(int i = 0; i < num; i++){
		for(int j = 0; j < dim; j++){
			res[i][j] = rand() % RANDOM_RANGE;
		}	
	}
	return res;
}

void savePoints(Points points, int * colors, string file){
	ofstream archivo(file.c_str());
	for(int i = 0; i < points.fil; i++){
		for(int j = 0; j < 3; j++){
			archivo<<points[i][j]<<" ";
		}
		archivo<<colors[i]<<endl;
	}
	archivo.close();
}


