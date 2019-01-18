#include <iostream>
#include <vector>
#include <cctype>
#include <clocale>
#include <list>



using namespace std;

enum Op {SUMA,RESTA,MULTIPLICACION,DIVISION};

int esOperador(char e){
	if(e == '+') return SUMA;
	else if(e == '-') return RESTA;
	else if(e == '*') return MULTIPLICACION;
	else if(e == '/') return DIVISION;
	else return -1;
}

int operar(string ecuacion){
	list<int> pila;
	string temp = "";
	int op = -1;
	for(char e : ecuacion){
		if(isdigit(e)){
			temp.push_back(e);
			pila.push_front(stoi(temp));
			temp.clear();
		}
		else if((op = esOperador(e)) != -1){
			int oper1 = pila.front();
			pila.pop_front();
			int oper2 = pila.front();
			pila.pop_front();
			switch(op){
				case SUMA:
					pila.push_front(oper1 + oper2);
					break;
				case RESTA:
					pila.push_front(oper2 - oper1);
					break;
				case MULTIPLICACION:
					pila.push_front(oper1 * oper2);
					break;
				case DIVISION:
					if(oper1 == 0) throw((int) 0);
					pila.push_front(oper2 / oper1);
					break;
			}
		}
	}
	return pila.front();
}

string leerArchivo(){
	string ecuacion = "";
	string temp = "";
	while(cin>>temp){
		ecuacion = ecuacion + temp;
	}
	return ecuacion;
}

int main(int argc, char const *argv[])
{
	try{
		string ecuacion = leerArchivo();
		int oper = operar(ecuacion);
		cout<<oper<<endl;
		return 0;	
	}
	catch(int e){
		if(e == 0) cout<<"Division entre cero"<<endl;
	}
	
}
