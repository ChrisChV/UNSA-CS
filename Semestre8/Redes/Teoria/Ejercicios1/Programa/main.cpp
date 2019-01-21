#include <iostream>

using namespace std;

void protocolo7(string mensaje){
	cout<<"Protocolo 7: "<<mensaje<<endl;
	cout<<"Mensaje enviado"<<endl;
}

void protocolo6(string mensaje){
	cout<<"Protocolo 6: "<<mensaje<<endl;
	protocolo7(mensaje);
}

void protocolo5(string mensaje){
	cout<<"Protocolo 5: "<<mensaje<<endl;
	protocolo6(mensaje);
}

void protocolo4(string mensaje){
	cout<<"Protocolo 4: "<<mensaje<<endl;
	protocolo5(mensaje);
}

void protocolo3(string mensaje){
	cout<<"Protocolo 3: "<<mensaje<<endl;
	protocolo4(mensaje);
}

void protocolo2(string mensaje){
	cout<<"Protocolo 2: "<<mensaje<<endl;
	protocolo3(mensaje);
}

void protocolo1(string mensaje){
	cout<<"Protocolo 1: "<<mensaje<<endl;
	protocolo2(mensaje);
}

int main(int argc, char const ** argv){
	if(argc != 2){
		cout<<"Faltan argumentos <mensaje>"<<endl;
		return 0;
	}
	string mensaje(argv[1]);
	protocolo1(mensaje);
	return 0;
}
