#include <iostream>
#include <vector>
#include <cctype>
#include <clocale>
#include <list>

//+ -
//* /

using namespace std;

int esOperador(char e){
	if(e == '+' or e == '-') return 1;
	else if(e == '*' or e == '/') return 2;
	return -1;
}

int presedencia(char e){
	return esOperador(e);
}


string infijaToPosfija(string ecuacion){
	list<char> pila;
	string res = "";
	for(char e : ecuacion){
		if(isdigit(e)) res.push_back(e);
		else if(e == 'x') res.push_back(e);
		else if(e == '(') pila.push_front(e);
		else if(e == ')'){
			bool flag = false;
			char temp;
			while(!pila.empty()){
				temp = pila.front();
				pila.pop_front();
				if(temp == '('){
					flag = true;
					break;
				}
				res.push_back(temp);
			}
			if(!flag) throw((int) 2);
		}
		else if(esOperador(e) != -1){
			int presedenciaE = presedencia(e);
			int presedencia2 = -1;
			char temp;
			while(!pila.empty()){
				temp = pila.front();
				presedencia2 = presedencia(temp);
				if(presedencia2 == -1) break;
				else if(presedencia2 >= presedenciaE){
					pila.pop_front();
					res.push_back(temp);
				}
				else break;
			}
			pila.push_front(e);			
		}
	}
	for(char c : pila){
		res.push_back(c);
	}
	return res;
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
		string res = infijaToPosfija(ecuacion);
		cout<<res<<endl;	
	}
	catch(int e){
		cout<<"ERROR"<<endl;
	}
	
}
