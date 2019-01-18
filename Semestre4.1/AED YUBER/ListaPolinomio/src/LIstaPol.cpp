#include "LIstaPol.h"

LIstaPol::LIstaPol()
{
    cabeza = nullptr;
    cola = nullptr;
}

NodoPol* LIstaPol::_find(int exponente)
{
    NodoPol * temporal = cabeza;
    while(temporal != nullptr){
        if(temporal->exponente == exponente){
            return temporal;
        }
    }
    return nullptr;
}

NodoPol* LIstaPol::_findMAyor(int exponente)
{
    NodoPol * temporal = cabeza;
    while(temporal != nullptr){
        if(temporal->exponente >)
    }
}


void LIstaPol::ingresar(int valor, int exponente)
{
    auto temporal = _find(exponente);
    if(temporal != nullptr){
        temporal->valor += valor;
    }
    else{
        NodoPol * nuevo = new NodoPol(valor, exponente);
        auto temp = _find(exponente + 1);
        if(temp != nullptr){

        }
    }
}

void LIstaPol::print()
{

}

LIstaPol::~LIstaPol()
{
    //dtor
}
