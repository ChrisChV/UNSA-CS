#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

bool esLetra(string::iterator &);
bool esNumero(string::iterator &);
bool esLetra(const char &);
bool esNumero(const char &);

bool propioIdentificador(){
    string frase;
    cin>>frase;
    int estado = 0;
    for(int i = 0; i < frase.size(); i++){
        switch(estado){
            case 0:
                if(frase[i] == 99){
                    estado = 1;
                }
                else{
                    estado = 7;
                    i = frase.size() + 1;
                }
                break;
            case 1:
                if(frase[i] == 111){
                    estado = 2;
                }
                else{
                    estado = 7;
                    i = frase.size() + 1;
                }
                break;
            case 2:
                if(frase[i] == 100){
                    estado = 3;
                }
                else{
                    estado = 7;
                    i = frase.size() + 1;
                }
                break;
            case 3:
                if(frase[i] == 101){
                    estado = 4;
                }
                else{
                    estado = 7;
                    i = frase.size() + 1;
                }
                break;
            case 4:
                if(frase[i] == 99){
                    estado = 5;
                }
                else if((frase[i] >= 65 and frase[i] <= 90) or (frase[i] >= 97 and frase[i] <= 122) or (frase[i] >= 48 and frase[i] <= 57)){
                    estado = 4;
                }
                else{
                    estado = 7;
                    i = frase.size() + 1;
                }
                break;
            case 5:
                if(frase[i] == 104){
                    estado = 6;
                }
                else if(frase[i] == 99){
                    estado = 5;
                }
                else if((frase[i] >= 65 and frase[i] <= 90) or (frase[i] >= 97 and frase[i] <= 122) or (frase[i] >= 48 and frase[i] <= 57)){
                    estado = 4;
                }
                else{
                    estado = 7;
                    i = frase.size() + 1;
                }
                break;
            case 6:
                if((frase[i] >= 65 and frase[i] <= 90) or (frase[i] >= 97 and frase[i] <= 122) or (frase[i] >= 48 and frase[i] <= 57)){
                    estado = 4;
                }
                else{
                    estado = 7;
                    i = frase.size() + 1;
                }
                break;
        }
    }
    if(estado == 6){
        cout<<"El identificador esta correcto"<<endl;
        return true;
    }
    cout<<"ERROR"<<endl;
    return false;
}

bool numeroFloat(){
    string frase;
    cin>>frase;
    int estado = 0;
    for(int i = 0; i < frase.size(); i++){
        switch(estado){
            case 0:
                if(frase[i] >= 48 and frase[i] <= 57){
                    estado = 1;
                }
                else{
                    estado = 3;
                    i = frase.size() + 1;
                }
                break;
            case 1:
                if(frase[i] >= 48 and frase[i] <= 57){
                    estado = 1;
                }
                else if(frase[i] == 46){
                    estado = 2;
                }
                else{
                    estado = 3;
                    i = frase.size() + 1;
                }
                break;
            case 2:
                if(frase[i] >= 48 and frase[i] <= 57){
                    estado = 4;
                }
                else{
                    estado = 3;
                    i = frase.size() + 1;
                }
                break;
            case 4:
                if(frase[i] >= 48 and frase[i] <= 57){
                    estado = 4;
                }
                else{
                    estado = 3;
                    i = frase.size() + 1;
                }
                break;
        }
    }
    if(estado == 1 or estado == 4){
        cout<<"El identificador esta correcto"<<endl;
        return true;
    }
    cout<<"ERROR"<<endl;
    return false;
}

void verificarIdentificador(){
    char letra;
    ifstream fe("datos.txt");
    int estado = 0;
    while(!fe.eof()){
        estado = 0;
        while(letra != ';' and !fe.eof()){
            fe>>letra;
            switch(estado){
                case 0:
                    if(letra >= 48 and letra <= 57){
                        estado = 2;
                    }
                    else if((letra >= 97 and letra <= 122) or letra == '_'){
                        estado = 1;
                    }
                    else{
                        estado = 2;
                    }
                    break;
                case 1:
                    if((letra >= 97 and letra <= 122) or (letra >=48 and letra <= 57) or letra == '_'){
                        estado = 1;
                    }
                    else if(letra == ';'){
                        estado = 3;
                    }
                    else{
                        estado = 2;
                    }
                    break;
            }
        }
        if(estado == 3){
            cout<<"Identificador correcto"<<endl;
       }
       else{
            cout<<"ERROR"<<endl;
       }
    fe>>letra;
    }
}

void verificarBinario(){
    char letra;
    ifstream fe("datos.txt");
    int estado = 0;
    bool flag = true;
    fe.get(letra);
    while(!fe.eof()){
        estado = 0;
        while(letra != '\n' and !fe.eof()){
            switch(estado){
                case 0:
                    if(letra == 48){
                        estado = 1;
                    }
                    else{
                        estado = 4;
                    }
                    break;
                case 1:
                    if(letra == 49){
                        estado = 2;
                    }
                    else if(letra == 48){
                        estado = 1;
                    }
                    else{
                        estado = 4;
                    }
                    break;
                case 2:
                    if(letra == 49){
                        estado = 2;
                    }
                    else if(letra == 48){
                        estado = 1;
                    }
                    else{
                        estado = 4;
                    }
                    break;
            }
            fe.get(letra);
        }
        if(estado == 2){
            cout<<"Identificador correcto"<<endl;
        }
        else{
            cout<<"ERROR"<<endl;
        }
    fe.get(letra);
    }
}

void fun(){
    char letra;
    ifstream fe("datos.txt");
    int contador = 0;
    while(!fe.eof()){
        fe.get(letra);
        cout<<letra;
        contador++;
        cout<<contador;
    }

}

bool identificadorConTabla(){
    string frase;
    cin>>frase;
    int entrada;
    string::iterator letra = frase.begin();
    int estado = 0;
    int tabla[3][3] = {{2,1,-1},{-1,-1,-1},{2,2,10}};
    do{
        if(esLetra(letra) or *letra == '_'){
            entrada = 0;
        }
        else if(esNumero(letra)){
            entrada = 1;
        }
        else if(letra == frase.end()){
            entrada = 2;
        }
        else{
            return false;
        }
        estado = tabla[estado][entrada];
        if(estado == -1)return false;
        letra++;
    }while(estado != 10);
    return true;
}

bool esNumero(const char &caracter){
    if(caracter >= 48 and caracter <= 57)return true;
    return false;
}

bool esLetra(const char &caracter){
    if(caracter >= 48 and caracter <= 57)return true;
    return false;
}

bool esNumero(string::iterator &letra){
    if(*letra >= 48 and *letra <= 57)return true;
    return false;
}

bool esLetra(string::iterator &letra){
    if(*letra >= 97 and *letra <= 122)return true;
    return false;
}

bool numeroReal(){
    string frase;
    cin>>frase;
    int estado = 1;
    for(int i = 0; i < frase.size(); i++){
        switch(estado){
            case 1:
                if(esLetra(frase[i])){
                    estado = 2;
                }
                else{
                    return false;
                }
                break;
            case 2:
                if(esNumero(frase[i])){
                    estado = 2;
                }
                else if(frase[i] == '.'){
                    estado = 3;
                }
                else if(frase[i] == 'E'){
                    estado = 5;
                }
                else{
                    return false;
                }
                break;
            case 3:
                if(esNumero(frase[i])){
                    estado = 4;
                }
                else{
                    return  false;
                }
                break;
            case 4:
                if(esNumero(frase[i])){
                    estado = 4;
                }
                else if(frase[i] == 'E'){
                    estado = 5;
                }
                else{
                    return false;
                }
                break;
            case 5:
                if(esNumero(frase[i])){
                    estado = 7;
                }
                else if(frase[i] == '-' or frase[i] == '+'){
                    estado = 6;
                }
                else{
                    return false;
                }
                break;
            case 6:
                if(esNumero(frase[i])){
                    estado = 7;
                }
                else{
                    return false;
                }
                break;
            case 7:
                if(esNumero(frase[i])){
                    estado = 7;
                }
                else{
                    return false;
                }
                break;
        }
    }
    if(estado == 4 or estado == 7){
        return true;
    }
    return false;
}

bool numeroRealConTabla(){
    string frase;
    cin>>frase;
    int entrada;
    string::iterator letra = frase.begin();
    int estado = 1;
    int tabla[7][6] = {{2,-1,-1,-1,-1,-1},
                       {2, 3, 5,-1,-1,-1},
                       {4,-1,-1,-1,-1,-1},
                       {4,-1, 5,-1,-1,10},
                       {7,-1,-1, 6, 6,-1},
                       {7,-1,-1,-1,-1,-1},
                       {7,-1,-1,-1,-1,10}};
    do{
        if(esNumero(letra))
            entrada = 0;
        else if(letra == frase.end())
            entrada = 5;
        else{
            switch(*letra){
                case '.':entrada = 1;break;
                case 'E':entrada = 2;break;
                case '+':entrada = 3;break;
                case '-':entrada = 4;break;
                default: return false;
            }
        }
        estado = tabla[estado - 1][entrada];
        letra++;
        if(estado == -1)return false;
    }while(estado != 10);
    return true;
}

int main()
{
    while(true){
    if(numeroRealConTabla())cout<<"Identificador Valido"<<endl;
    else cout<<"Identificador No Valido"<<endl;
    }
}
