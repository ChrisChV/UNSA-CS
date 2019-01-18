#include <iostream>
#include <cmath>
#define VN 48
#define LETRAS 27
#define ESPACIO 27
#define VT 97

using namespace std;


string cesar(const char * f, const int sumatoria){
    string frase = f;
    string resultado;
    for(auto iter = frase.begin(); iter != frase.end(); ++iter){
        if(*iter == 32){
            resultado.insert(resultado.end(), (ESPACIO + sumatoria ) % LETRAS);
        }
        else{
            resultado.insert(resultado.end(), (((*iter - VT) + sumatoria) % LETRAS) + VT);
        }
    }
    return resultado;
}

string cesarDes(const char *f, const int sumatoria){
    string frase = f;
    string resultado;
    for(auto iter = frase.begin(); iter != frase.end(); ++iter){
        if(*iter == 32){
            resultado.insert(resultado.end(), (ESPACIO + sumatoria ) % LETRAS);
        }
        else{
            int num = abs(((*iter - VT) - sumatoria));
            resultado.insert(resultado.end(), ( num % LETRAS) + VT);
        }
    }
    return resultado;
}

int main()
{
    auto a = cesar("hola",100);
    cout<<a;

}
