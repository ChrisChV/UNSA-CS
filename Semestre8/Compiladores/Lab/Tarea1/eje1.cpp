#include <iostream>
#include <cctype>
#include <clocale>

using namespace std;

int main(){
	try{
		int estado = 1;
		char c;
		cin>>c;
		while(c != '#'){
			switch(estado){
				case 1: 
					if(isalpha(c)) estado = 3;
					else if (isdigit(c)) estado = 2;
					else throw((int) 1);
					break;
				case 2: throw((int) 2);
				case 3:
					if(isalpha(c)) estado = 3;
					else if (isdigit(c)) estado = 3;
					else throw((int) 3);

			}
			cin>>c;
		}
		if(estado != 3) throw((int) 4);
		cout<<"OK..."<<endl;
	}
	catch(int e){
		switch(e){
			case 1: cout<<"Símbolo no reconocido"<<endl; break;
			case 2: cout<<"No se puede comenzar un identificador con un dígito"<<endl; break;
			case 3: cout<<"Símbolo no reconocido"<<endl; break;
			case 4: cout<<"Identificador no reconocido"<<endl; break;
		}
	}
}