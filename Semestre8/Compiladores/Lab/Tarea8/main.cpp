#include <iostream>
#include <vector>

using namespace  std;

class Cadena{
public:
	Cadena(int size, int posIni){
		cadena = "";
		actual = 0;
		for(int i = 0; i < size; i++){
			cadena.push_back('-');
		}
		this->posIni = posIni;
		aceptada = false;
	}
	void pushCharacter(char c){
		cadena[actual] = c;
		actual++;
	}
	bool verifyCharacter(string patron, char c){
		if(cadena.back() != '-'){
			aceptada = true;
			return true;
		} 
		if(patron[actual] == c){
			pushCharacter(c);
			return true;
		} 
		return false;
	}
	bool verifyCadena(){
		for(char c : cadena){
			if(c == '-') return false;
		}
		return true;
	}
	int actual;
	int posIni;
	bool aceptada;
	string cadena;
};

vector<Cadena> getPatron(string patron, string cadena){
	vector<Cadena> res;
	for(int i = 0; i < cadena.size(); i++){
		char c = cadena[i];
		for(int j = 0; j < res.size(); j++){
			if(res[j].verifyCharacter(patron, c) == false){
				res.erase(res.begin() + j);
				j--;
			}
		}
		if(c == patron[0]){
			res.push_back(Cadena(patron.size(), i));
			res.back().pushCharacter(c);
		}
	}
	for(int i = 0; i < res.size(); i++){
		if(res[i].verifyCadena() == false){
			res.erase(res.begin() + i);
			i--;
		}
	}
	return res;
}

int main(){
	string patron;
	string cadena;
	cout<<"Escriba la cadena:";
	cin>>cadena;
	cout<<"Escriba el patron:";
	cin>>patron;
	vector<Cadena> res = getPatron(patron, cadena);
	cout<<"Se encontraron "<<res.size()<<" coincidencias"<<endl;
	for(Cadena cad : res){
		cout<<"Posicion->"<<cad.posIni<<endl;
	}
}
