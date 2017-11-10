#include <iostream>
#include <vector>
#include <cctype>
#include <clocale>

using namespace std;

enum Tipos {LETRA, DIGITO, FDC};

vector<vector<int>> tabla = {{2,1,-1},{-1,-1,-1},{2,2,-2}};

int main(){
	try{
		int estado = 0;
		int entrada = -1;
		char c;
		cin>>c;
		while(true){
			if(c == '#') entrada = FDC;
			else if(isalpha(c)) entrada = LETRA;
			else if(isdigit(c)) entrada = DIGITO;
			else throw((int) 2);
			estado = tabla[estado][entrada];
			if(estado == -1) throw((int) 1);
			if(estado == -2){
				cout<<"OK..."<<endl;
				break;
			}
			cin>>c;
		}
	}
	catch(int e){
		if(e == 1) cout<<"Error en la cadena"<<endl;
		else cout<<"Símbolo no reconocido"<<endl;
	}

}
