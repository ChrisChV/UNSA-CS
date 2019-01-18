#include <iostream>
#include "Monticulo.h"
#include <fstream>
#include "math.h"
using namespace std;

float convertirNumero(string numero){
    double resultado = 0;
    bool flag = true;
    auto iter = numero.end();
    iter--;
    double contador = 0;
    for(iter; iter!= numero.begin(); iter--){
        if(!((*iter) >= 48 and (*iter) <= 57) and  (*iter)  != '.'){
            return 0;
        }
        if((*iter) == '.'){
            if(flag){
                resultado /= pow(10,contador);
                contador = -1;
                flag = false;
            }
            else{
                return 0;
            }
        }
        else{
            resultado += pow(10,contador) * ((*iter) - 48);
        }
        contador++;
    }
    resultado += pow(10,contador) * ((*iter) - 48);
    return resultado;
}

bool esNumero(string::iterator &letra){
    if(*letra >= 48 and *letra <= 57)return true;
    return false;
}

float verificarLinea(string linea){
    int estado = 0;
    string resultado;
    for(auto iter = linea.begin(); iter != linea.end(); ++iter){
        if(estado == 5)break;
        switch(estado){
            case 0:
                if(*iter == 116)estado = 1;
                else return -1;
                break;
            case 1:
                if(esNumero(iter))estado = 2;
                else return -1;
                break;
            case 2:
                if(esNumero(iter))estado = 2;
                else if(*iter == 32)estado = 3;
                else return -1;
                break;
            case 3:
                if(esNumero(iter)){
                    estado = 4;
                    resultado.insert(resultado.end(),*iter);
                }
                else return -1;
                break;
            case 4:
                if(esNumero(iter)){
                    estado = 4;
                    resultado.insert(resultado.end(),*iter);
                }
                else if(*iter == 32) estado = 5;
                else return -1;
                break;
            default:
                return -1;
        }
    }
    return convertirNumero(resultado);
}

int main()
{
    auto a = convertirNumero("10.5.1");
    cout<<a<<endl;
    /*
    Monticulo impresion;
    ifstream archivo("hojas.txt");
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        return 0;
    }
    char linea[128];
    int numberLine = 0;
    while(archivo.getline(linea,128)){
        numberLine++;
        string tempLinea(linea);
        auto number = verificarLinea(tempLinea);
        if(number == -1){
            cout<<"Error en la linea "<<numberLine<<endl;
        }
        else{
            impresion.ingresar(number);
        }
    }
    archivo.close();
    auto tam = impresion.size();
    for(int i = 0; i < tam; i++){
        cout<<"Impresion numero "<<i+1<<" :"<<impresion.del()<<" hojas"<<endl;
    }*/
}


