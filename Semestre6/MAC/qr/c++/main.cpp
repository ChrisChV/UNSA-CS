#include <iostream>
	#include <tuple>
#include "OperacionesMatriz.h"

using namespace std;

Num MulList(Lista A, Lista B){
	Num res = 0;
	for(int i = 0; i < A.size(); i++){
		res += A[i] * B[i];
	}
	return res;
}

vector<Lista> getColumns(Matriz & A){
	vector<Lista> res;
	for(int i = 0; i < A.size(); i++){
		Lista temp;
		for(int j = 0; j < A.size(); j++){
			temp.push_back(A[j][i]);
		}
		res.push_back(temp);
	}
	return res;
}

Matriz generarMatriz(vector<Lista> E, int tam){
	Matriz Q = zeros(tam);
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			Q[j][i] = E[i][j];
		}
	}
	return Q;
}

tuple<Matriz,Matriz> QRdescomposition(Matriz & A){
	vector<Lista> Es;
	Matriz Q = zeros(A.size());
	Matriz R = zeros(A.size());
	vector<Lista> As = getColumns(A);
	for(int i = 0; i < A.size(); i ++){
		Lista u = As[i];
		for(int j = 0; j < i; j++){
			Num temp = MulList(As[i],Es[j]);
			R[j][i] = temp;
			Lista lTemp = Es[j] * temp;
			u = u - lTemp;
		}
		Es.push_back(u * (1.0/norm(u)));
		R[i][i] = MulList(As[i],Es[i]);
	}
	Q = generarMatriz(Es,A.size());
	return make_tuple(Q,R);
}

int main(int argc, char * argv[]){
	if(argc != 2){
		cout<<"Faltan argumentos <Numero de iteraciones"<<endl;
		return 0;
	}
	string temp(argv[1]);
	int n = stoi(temp);
	Matriz A = {{18,40},{-12,26}};
	cout<<"Matriz Original:"<<endl;
	mostrarMatriz(A);
	for(int i = 0; i < n; i++){
		auto Q_R = QRdescomposition(A);		
		Matriz Q = get<0>(Q_R);
		Matriz R = get<1>(Q_R);
		A = R * Q;
		cout<<"Iteracion "<<i+1<<":"<<endl;
		mostrarMatriz(A);
	}
}


