#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

typedef long double Num;
typedef vector<vector<Num>> Matriz;
typedef vector<Num> Lista;
typedef long double Presicion;

void escribirLista(Lista x, ofstream &archivo){
	for(auto iter = x.begin(); iter != x.end(); ++iter){
			archivo<<to_string(*iter);
			if(iter != x.end()-1) archivo<<",";
			else archivo<<endl;
	}
}

Presicion ErrorAbsoluto(Num X_actual, Num X_anterior){
	return abs(X_actual - X_anterior);
}

bool _ErrorAbsoluto(Lista X_actual, Lista X_anterior, Presicion presicion){
	int n = X_actual.size();
	for(int i = 0; i < n; i++){
		if(ErrorAbsoluto(X_actual[i],X_anterior[i]) > presicion) return true;
	}
	return false;
}

Num sumatoria(Matriz &A, Lista &B, Lista &X, int r){
	Num sum = 0;
	int n = X.size();
	for(int i = 0; i < n; i++){
		if(i != r){
			sum += A[r][i] * X[i];
		}
	}
	return sum;
}

Lista generarSiguienteAproxJacoby(Matriz &A, Lista &B, Lista &X){
	int n = X.size();
	Lista res(n);
	for(int i = 0; i < n; i++){
		res[i] = (B[i] - sumatoria(A,B,X,i)) / A[i][i];
	}
	return res;
}

void generarSiguienteAproxGaus(Matriz &A, Lista &B, Lista &X){
	int n = X.size();
	for(int i = 0; i < n; i++){
		X[i] = (B[i] - sumatoria(A,B,X,i)) / A[i][i];
	}
}

void Gaus(Matriz &A, Lista &B, Lista &X, int n, Presicion presicion, string fi){
	string file = fi + ".csv";
	ofstream archivo(file);
	int i = 0;
	archivo<<"\"X\",";
	for(int j = 0; j < X.size(); j++){
		archivo<<"\"X"<<to_string(j)<<"\"";
		if(j != X.size()-1) archivo<<",";
		else archivo<<endl;
	}
	Lista X_anterior = X;
	do{
		i++;
		X_anterior = X;
		generarSiguienteAproxGaus(A,B,X);
		archivo<<"\"i"<<to_string(i)<<"\""<<endl;
		archivo<<"\"X\",";
		escribirLista(X,archivo);
	}while(_ErrorAbsoluto(X,X_anterior,presicion) and i < n);
	archivo<<"\"Resultado actual\",";
	escribirLista(X,archivo);
	archivo.close();
}

void Jacoby(Matriz &A, Lista &B, Lista &X, int n, Presicion presicion, string fi){
	string file = fi + ".csv";
	ofstream archivo(file);
	int i = 0;
	archivo<<"\"X\",";
	for(int j = 0; j < X.size(); j++){
		archivo<<"\"X"<<to_string(j)<<"\"";
		if(j != X.size()-1) archivo<<",";
		else archivo<<endl;
	}
	Lista X_anterior;
	do{
		i++;
		X_anterior = X;
		X = generarSiguienteAproxJacoby(A,B,X);
		archivo<<"\"i"<<to_string(i)<<"\""<<endl;
		archivo<<"\"X\",";
		escribirLista(X,archivo);
	}while(_ErrorAbsoluto(X,X_anterior,presicion) and i < n);
	archivo<<"\"Resultado actual\",";
	escribirLista(X,archivo);
	archivo.close();
}

int main(){
	Matriz A = {{9,2,-5},{1,-4,2},{2,-1,9}};
	Lista B = {14,9,12};
	Lista X = {0,0,0};
	string file;
	int t;
	int n;
	Presicion presicion;
	cout<<"Jacoby (1) or Gaus(2)->";
	cin>>t;
	cout<<"Ingrese la presicion->";
	cin>>presicion;
	cout<<"Ingrese n->";
	cin>>n;
	cout<<"Ingrese el nombre del archivo->";
	cin>>file;
	if(t == 1) Jacoby(A,B,X,n,presicion,file);
	else if(t == 2) Gaus(A,B,X,n,presicion,file);
}