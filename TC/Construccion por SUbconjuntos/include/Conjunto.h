#ifndef CONJUNTO_H
#define CONJUNTO_H
#include "list"
#include "tuple"
#include "iostream"
#include "fstream"

using namespace std;

typedef char Caracter;
typedef char Name;

template <typename T>
class Automata{
    public:
        Automata();
        Automata(const char *);
        virtual ~Automata();
        class Conjunto
        {
            public:
                Conjunto();
                Conjunto(Name);
                virtual ~Conjunto();
                Name name;
                list<T> valores;
                bool marcado;
                void uni(Conjunto *);
        };
        Conjunto * relacion(Conjunto *, Caracter);
        Conjunto * findRealcion(T, Caracter);
        Conjunto * e_Clausura(Conjunto *);
        Conjunto * camino(T valor, Caracter);
        Conjunto * findConjunto(Name);
    private:
        list<Caracter> alfabeto;
        list<Conjunto *> elementos;
        list<Conjunto *> estadosFinales;
        list<tuple<Conjunto *, Caracter, Conjunto *>> relaciones;
};

Automata::Conjunto * Automata::findConjunto(Name name){
    for(auto iter = elementos.begin(); iter != elementos.end(); ++iter){
        if((*iter)->name == name) return (*iter);
    }
    return nullptr;
}

Automata::Conjunto * Automata::camino(T valor, Caracter etiqueta){
    Conjunto * resultado = new Conjunto();
    Conjunto * temp = findRealcion(valor, etiqueta);
    for(auto iter = temp.valores.begin(); iter != temp.valores.end(); ++iter){
        Conjunto * temptemp = findRealcion(valor, etiqueta);
        if(temptemp.valores.size() > 0){
            temp.uni(temptemp);
        }
    }
    if(temp.valores.size() > 0){
        resultado.uni(temp);
    }
    return resultado;
}

Automata::Conjunto * Automata::e_Clausura(Conjunto * conjunto){

}

Automata::Conjunto* Automata::findRealcion(T valor, Caracter etiqueta){
    Conjunto * resultado = new Conjunto();
    for(auto iter = relaciones.begin(); iter != relaciones.end(); ++iter){

    }
    return resultado;
}

Automata::Conjunto * Automata::relacion(Conjunto * conjunto, Caracter etiqueta){
    Conjunto * resultado = new Conjunto();
    for(auto iter = conjunto.valores.begin(); iter != conjunto.valores.end(); ++iter){
        Conjunto * temp = findRealcion(*iter, etiqueta);
        if(temp.valores.size() > 0){
            resultado.uni(temp);
        }
    }
    return resultado;
}

void Automata::Conjunto::uni(Conjunto * conjunto){
    this->valores.insert(this->valores.end(),conjunto.valores.begin(),conjunto.valores.end());
}

Automata::Automata(){

}

Automata::Automata(const char * file){
    ifstream archivo(file);
    if(archivo.fail()){
        cout<<"El archivo no se pudo abrir correctamente";
        return;
    }
    char caracter;
    archivo.get(caracter);
    while(caracter != ';'){
        if(caracter == ',' or caracter == 32){
            archivo.get(caracter);
        }
        else{
            elementos.push_back(new Caracter(caracter));
            archivo.get(caracter);
        }
    }
    archivo.get(caracter);
    while(caracter != ';'){
        if(caracter == ',' or caracter == 32){
            archivo.get(caracter);
        }
        else{
            alfabeto.push_back(caracter);
            archivo.get(caracter);
        }
    }
    archivo.get(caracter);
    while(caracter != ';'){
        if(caracter == ',' or caracter == 32){
            archivo.get(caracter);
        }
        else{
            Conjunto * temp = findConjunto(caracter);
            if(temp) estadosFinales.push_back(temp);
            archivo.get(caracter);
        }
    }
    archivo.get(caracter);
    bool first = false;
    bool second = false;
    Conjunto * firstC = nullptr;
    Conjunto * secondC = nullptr;
    Caracter etiqueta = '$';
    while(caracter != '.'){
        if(caracter == 32 or caracter == '\n'){
            archivo.get(caracter);
        }
        else if(caracter == ';'){
            relaciones.push_back(make_tuple(firstC,etiqueta,secondC));
            firstC = nullptr;
            secondC = nullptr;
            etiqueta = '$'
            first = false
            second = false;
            archivo.get(caracter);
        }
        else if(!first and !second){
            firstC = findConjunto(caracter);
            first = true;
            archivo.get(caracter);
        }
        else if(first and !second){
            etiqueta = caracter;
            second = true;
            archivo.get(caracter);
        }
        else (first and second){
            secondC = findConjunto(caracter);
            first = false;
            archivo.get(caracter);
        }
        else{
            archivo.get(caracter);
        }
    archivo.close();
    }
}

Automata::~Automata(){

}

Automata::Conjunto::Conjunto(Name name){
    this->name = name;
    marcado = true;
}

Automata::Conjunto::Conjunto(){
    name = nullptr;
    marcado = true;
}

Automata::Conjunto::~Conjunto(){

}


#endif // CONJUNTO_H
