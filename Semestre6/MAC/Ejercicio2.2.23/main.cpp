#include <iostream>
#include "OperacionesMatriz.h"

using namespace std;

int main(){
	mostrarMatriz(Hilbert(5));
	Num n = 0.0000001;
	int tipo;
	Num x;
	cout<<"Que problema quiere resolver: b (1) - c (2) ->";
	cin>>tipo;
	cout<<"Con que x quieres trabajar ->";
	cin>>x;
	Matriz A = {{375,374},{752,750}};
	Lista X = createLista(x,A.size());
	Lista alfa = createLista(n,A.size());
	Lista b = A * X;
	cout.precision(20);
	if(tipo == 1){
		cout<<"----X----"<<endl;
		mostrarLista(alfa);
		cout<<"----aX----"<<endl;
		X = X - alfa;
		mostrarLista(X);
		cout<<"----error----"<<endl;
		cout<<normInf(X) / normInf(alfa)<<endl;

		cout<<"----B----"<<endl;
		b = b - alfa;
		mostrarLista(b);
		cout<<"----aB----"<<endl;
		mostrarLista(alfa);
		cout<<"----error----"<<endl;
		cout<<normInf(alfa) / normInf(b)<<endl;
	}
	else if(tipo == 2){
		cout<<"----X----"<<endl;
		X = X - alfa;
		mostrarLista(X);
		cout<<"----aX----"<<endl;
		mostrarLista(alfa);
		cout<<"----error----"<<endl;
		cout<<normInf(alfa) / normInf(X)<<endl;

		cout<<"----B----"<<endl;
		mostrarLista(alfa);
		cout<<"----aB----"<<endl;
		b = b - alfa;
		mostrarLista(b);
		cout<<"----error----"<<endl;
		cout<<normInf(b) / normInf(alfa)<<endl;	
	}
}