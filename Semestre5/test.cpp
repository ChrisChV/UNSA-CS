#include <iostream>
#include <cstdlib>
#include <time.h>
#include <fstream>

using namespace std;

int numeroRandom(int a, int b){
    int temp = b - a +1;
    return rand() % temp + a;
}

string ingresarTexto(){
    string palabra;
    string frase;
    cin>>palabra;
    while(palabra != "-end"){
        for(string::iterator iter = palabra.begin(); iter != palabra.end(); iter++){
            frase.insert(frase.end(), *iter);
        }
        cin>>palabra;
        if(palabra != "-end"){
            frase.insert(frase.end(), ' ');
        }
    }
    return frase;
}
//ab0ed3ka0ne2rc2

void codificador_loop(){
    cout<<"====================================="<<endl;
    cout<<"=     FAVOR DE NO INGRESAR LETRAS   ="<<endl;
    cout<<"=        MAYUSCULAS NI NUMEROS      ="<<endl;
    cout<<"====================================="<<endl;
    cout<<"->INGRESAR TEXTO"<<endl;
    string resultado;
    string frase = ingresarTexto();

    for(int i = 97;  i < 124; i++){
        char looping = i;
        for(int j = 0; j < frase.length(); j++){
            if(frase[j] == i){
                resultado.insert(resultado.end(), frase[j]);
                resultado.insert(resultado.end(), j + 97);
                resultado.insert(resultado.end(), numeroRandom(48, 57));
            }
            else if(i == 123 and frase[j] == 32){
                resultado.insert(resultado.end(), 32);
                resultado.insert(resultado.end(), j + 97);
                resultado.insert(resultado.end(), numeroRandom(48, 57));
            }
        }
    }
    cout<<resultado<<endl<<endl;
}

int main(){
    srand(time(NULL));
    codificador_loop();
}