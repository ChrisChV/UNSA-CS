#include <iostream>
#include <cmath>
#include "SATree.h"


using namespace std;

Dis dd(vector<float> a, vector<float> b){
	Dis sum = 0;
	for(int i = 0; i < a.size(); i++) sum += pow(b[i] - a[i],2);
	return sqrt(sum);
}

int main(){
	vector<vector<float>>  points = { {0,0},{1,1},{-2,1},{0,-1},{-3,2},{-4,2},{1,-2},{4,4} };
	SATree<vector<float>>::FunDis distance = dd;
	SATree<vector<float>> tree(dd, points);
	tree.print("hola");
}