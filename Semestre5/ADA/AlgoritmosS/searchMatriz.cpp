#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

bool searchBinary(vector<vector<int>> &M, int col, int y0, int yf, int k){
	if(y0 == yf) return false;
	int media = (y0 + yf) / 2;
	if(k == M[media][col]) return true;
	if(k < M[media][col]) return searchBinary(M,col,y0,media,k);
	return searchBinary(M,col,media+1,yf,k);
}

bool searchM(vector<vector<int>> &M, int x0, int xf, int yf, int k){
	if(x0 == xf){
		cout<<"x0->"<<x0<<endl;
		return searchBinary(M,x0,0,yf,k);

	}
	int Mx = (x0 + xf) / 2;
	if(M[0][x0] <= k and k <= M[yf][Mx]){
		cout<<"A"<<endl;
		return searchM(M,x0,Mx,yf,k);	
	} 
	if(M[0][Mx+1] <= k and k <= M[yf][xf]){
		cout<<"B"<<endl;
	 return searchM(M,Mx+1,xf,yf,k);
	}
	/*
	if(M[x0][My+1] <= k and k <= M[Mx][yf]){
		cout<<"C"<<endl;
		return searchM(M,x0,My+1,Mx,yf,k);	
	} 
	if(M[Mx+1][My+1] <= k and k <= M[xf][yf]){
		cout<<"D"<<endl;
	 return searchM(M,Mx+1,My+1,xf,yf,k);
	}
	*/
	return false;
}

vector<vector<int>> generarMatriz(int n){
	vector<vector<int>> res;
	for(int i = 0; i < n; i++){
		res.push_back(vector<int>(n));
	}
	int actual = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			actual = actual + rand()%6;
			res[j][i] = actual;
		}
	}
	return res;
}

void imprimirMatriz(vector<vector<int>> &vec){
	int n = vec.size();
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cout<<vec[i][j]<<" ";
		}
		cout<<endl;
	}
}


int main(){
	srand(time(NULL));
	int n;
	int k;
	cout<<"Ingrese su n->";
	cin>>n;
	vector<vector<int>> M = generarMatriz(n);
	cout<<"Ingrese su k->";
	cin>>k;
	imprimirMatriz(M);
	cout<<searchM(M,0,n-1,n-1,k)<<endl;
}