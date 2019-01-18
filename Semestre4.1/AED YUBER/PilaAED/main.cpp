#include <iostream>
#include <Pila.h>
#include <math.h>

using namespace std;

float convertirNumero(string numero){
    double resultado = 0;
    auto iter = numero.end();
    iter--;
    double contador = 0;
    for(iter; iter!= numero.begin(); iter--){
        if((*iter) == '.'){
            resultado /= pow(10,contador);
            contador = -1;
        }
        else{
            resultado += pow(10,contador) * ((*iter) - 48);
        }
        contador++;
    }
    resultado += pow(10,contador) * ((*iter) - 48);
    return resultado;
}
int main()
{
    while(true){
    Pila<float> memoria;
    string expresion;
    string temp;
    cin>>expresion;
    for(int i = 0; i < expresion.size(); i++){
        if(expresion[i] == '|'){
            memoria.agregar(convertirNumero(temp));
            temp.clear();
        }
        else if((expresion[i] >= 48 and expresion[i] <= 57)
                 or expresion[i] == '.'){
            temp.insert(temp.end(),expresion[i]);
        }
        else{
            switch(expresion[i]){
                case '+':
                    memoria.agregar(memoria.desapilar()
                                     + convertirNumero(temp));
                    temp.clear();
                    break;
                case '-':
                    memoria.agregar(memoria.desapilar()
                                     - convertirNumero(temp));
                    temp.clear();
                    break;
                case '*':
                    memoria.agregar(memoria.desapilar()
                                     * convertirNumero(temp));
                    temp.clear();
                    break;
                case '/':
                    memoria.agregar(memoria.desapilar()
                                     / convertirNumero(temp));
                    temp.clear();
                    break;
                default:
                    cout<<"El caracter "<<expresion[i]
                        <<" no se puede reconocer";
                    return 0;
            }
        }
    }
    cout<<memoria.desapilar()<<endl;
    }
}
