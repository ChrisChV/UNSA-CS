#ifndef AUTOMATA_H
#define AUTOMATA_H
#include "list"
#include "tuple"
#include "iostream"
#include "fstream"
#include "algorithm"

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
                Conjunto(Name, T);
                virtual ~Conjunto();
                Name name;
                list<T> valores;
                void filtreRepeat();
                bool marcado;
                void uni(Conjunto *);
                bool pertenece(T valor);
                void print();
        };
        Automata(list<Caracter>, list<Conjunto *>,list<tuple<Conjunto *, Caracter, Conjunto *>>);
        void setEstadosFinales(list<Conjunto*>);
        Conjunto * relacion(Conjunto *, Caracter);
        Conjunto * findRealcion(T, Caracter);
        Conjunto * e_Clausura(Conjunto *);
        Conjunto * clausura(Conjunto *, Caracter);
        Conjunto * findConjunto(Name);
        Conjunto * findConjuntoVal(T);
        Automata construccionSubconjuntos();
        bool findListValores(Conjunto *);
        bool pertenece(Conjunto *);
        void print();
        void metadata();
    private:
        list<Caracter> alfabeto;
        list<Conjunto *> elementos;
        list<Conjunto *> estadosFinales;
        list<tuple<Conjunto *, Caracter, Conjunto *>> relaciones;
        string file;
};

template <typename T>
void Automata<T>::Conjunto::print(){
    for(auto iter = valores.begin(); iter != valores.end(); ++iter){
        cout<<*iter<<" ";
    }
    cout<<endl;
}

template <typename T>
bool Automata<T>::findListValores(Conjunto *conjunto){
    for(auto iter = elementos.begin(); iter != elementos.end(); ++iter){
        if((*iter)->valores == conjunto->valores) return true;
    }
    return false;
}

template <typename T>
Automata<T> Automata<T>::construccionSubconjuntos(){
    Conjunto * inicial = e_Clausura(elementos.front());
    inicial->print();
    inicial->marcado = false;
    list<Conjunto *> estados;
    list<Conjunto *> estadosF;
    list<tuple<Conjunto *, Caracter, Conjunto *>> rel;
    estados.push_back(inicial);
    for(auto iter = estados.begin(); iter != estados.end(); ++iter){
        (*iter)->marcado = true;
        for(auto iter2 = alfabeto.begin(); iter2 != alfabeto.end(); ++iter2){
            Conjunto * temporal = e_Clausura(relacion(*iter,*iter2));
            temporal->print();
            rel.push_back(make_tuple(*iter,*iter2,temporal));
            if(!findListValores(temporal)){
                if(temporal->valores.size() > 0){
                    estados.push_back(temporal);
                }
                temporal->marcado = false;
            }
        }
    }
    Automata result(this->alfabeto,estados,rel);
    for(auto iter = estadosFinales.begin(); iter != estadosFinales.end(); ++iter){
        auto temp = result.findConjuntoVal((*iter)->valores.front());
        if(temp){
            estadosF.push_back(temp);
        }
    }
    result.setEstadosFinales(estadosF);
    return result;
}

template <typename T>
typename Automata<T>::Conjunto * Automata<T>::findConjuntoVal(T valor){
    for(auto iter = elementos.begin(); iter != elementos.end(); ++iter){
        for(auto iter2 = (*iter)->valores.begin(); iter2 !=(*iter)->valores.end();++iter2){
            if(*iter2 == valor) return *iter;
        }
    }
    return nullptr;
}

template <typename T>
void Automata<T>::metadata(){
    cout<<"alfabeto:"<<endl;
    for(auto iter = alfabeto.begin(); iter != alfabeto.end(); ++iter){
        cout<<*iter<<endl;
    }
    cout<<"elementos"<<endl;
    for(auto iter = elementos.begin(); iter != elementos.end();++iter){
        cout<<"Conjunto->"<<(*iter)->name<<endl;
        for(auto iter2 = (*iter)->valores.begin(); iter2 !=(*iter)->valores.end();++iter2){
            cout<<(*iter2)<<",";
        }
        cout<<endl;
    }
    cout<<"estados finales"<<endl;
    for(auto iter = estadosFinales.begin(); iter != estadosFinales.end(); ++iter){
        cout<<"Conjunto->"<<(*iter)->name<<endl;
        for(auto iter2 = (*iter)->valores.begin(); iter2 !=(*iter)->valores.end();++iter2){
            cout<<(*iter2)<<",";
        }
        cout<<endl;
    }
    cout<<"relaciones"<<endl;
    for(auto iter = relaciones.begin(); iter != relaciones.end(); ++iter){
        cout<<get<0>(*iter)->name<<" "<<get<1>(*iter)<<" "<<get<2>(*iter)->name<<endl;
    }
}

template <typename T>
void Automata<T>::print(){





    string f = file + ".dot";
    ofstream archivo(f);
    if(archivo.fail()){
        cout<<"EL archivo no se pudo abrir"<<endl;
    }
    archivo.close();
}



template <typename T>
typename Automata<T>::Conjunto * Automata<T>::findConjunto(Name name){
    for(auto iter = elementos.begin(); iter != elementos.end(); ++iter){
        if((*iter)->name == name) return (*iter);
    }
    return nullptr;
}

template <typename T>
typename Automata<T>::Conjunto * Automata<T>::clausura(Conjunto * conjunto, Caracter etiqueta){
    Conjunto * resultado = new Conjunto();
    for(auto iter2 = conjunto->valores.begin(); iter2 != conjunto->valores.end(); ++iter2){
        Conjunto * temp = findRealcion(*iter2, etiqueta);
        for(auto iter = temp->valores.begin(); iter != temp->valores.end(); ++iter){
            Conjunto * temptemp = findRealcion(*iter, etiqueta);
            if(temptemp->valores.size() > 0){
                temp->uni(temptemp);
            }
        }
        if(temp->valores.size() > 0){
            resultado->uni(temp);
        }
    }

    resultado->filtreRepeat();
    resultado->valores.sort();
    return resultado;
}

template <typename T>
typename Automata<T>::Conjunto * Automata<T>::e_Clausura(Conjunto * conjunto){
    return clausura(conjunto,'e');
}

template <typename T>
typename Automata<T>::Conjunto* Automata<T>::findRealcion(T valor, Caracter etiqueta){
    Conjunto * resultado = new Conjunto();
    for(auto iter = relaciones.begin(); iter != relaciones.end(); ++iter){
        if(get<1>(*iter) == etiqueta and get<0>(*iter)->pertenece(valor)){
            resultado->uni(get<2>(*iter));
        }
    }
    return resultado;
}

template <typename T>
typename Automata<T>::Conjunto * Automata<T>::relacion(Conjunto * conjunto, Caracter etiqueta){
    Conjunto * resultado = new Conjunto();
    for(auto iter = conjunto->valores.begin(); iter != conjunto->valores.end(); ++iter){
        Conjunto * temp = findRealcion(*iter, etiqueta);
        if(temp->valores.size() > 0){
            resultado->uni(temp);
        }
    }
    return resultado;
}

template <typename T>
void Automata<T>::Conjunto::uni(Conjunto * conjunto){
    this->valores.insert(this->valores.end(),conjunto->valores.begin(),conjunto->valores.end());
}

template <typename T>
void Automata<T>::Conjunto::filtreRepeat(){
    list<T> temporal;
    for(auto iter = valores.begin(); iter != valores.end(); ++iter){
        bool flag = true;
        auto iter2 = temporal.begin();
        for(iter2; iter2 != temporal.end(); ++iter2){
            if(*iter2 == *iter){
                flag = false;
                break;
            }
        }
        if(flag){
            temporal.push_back(*iter2);
        }
    }
    this->valores = temporal;
}

template <typename T>
Automata<T>::Automata(){

}

template <typename T>
Automata<T>::Automata(const char * file){
    this->file = file;
    string f = this->file + ".txt";
    ifstream archivo(f);
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
            elementos.push_back(new Conjunto(caracter,caracter));
            archivo.get(caracter);
        }
    }
    archivo.get(caracter);
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
            etiqueta = '$';
            first = false;
            second = false;
            archivo.get(caracter);
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
        else if(first and second){
            secondC = findConjunto(caracter);
            first = false;
            archivo.get(caracter);
        }
        else{
            archivo.get(caracter);
        }
    }
    archivo.close();
    for(auto iter = elementos.begin(); iter != elementos.end(); ++iter){
        (*iter)->valores.sort();
    }
}

template <typename T>
void Automata<T>::setEstadosFinales(list<Conjunto *> ef){
    this->estadosFinales = ef;
}

template <typename T>
Automata<T>::Automata(list<Caracter> alfa, list<Conjunto *> estados, list<tuple<Conjunto *, Caracter, Conjunto *>> relaciones){
    this->alfabeto = alfa;
    this->elementos = estados;
    this->relaciones = relaciones;
    file = "resultado";
}

template <typename T>
bool Automata<T>::Conjunto::pertenece(T valor){
    for(auto iter = valores.begin(); iter != valores.end(); ++iter){
        if(valor == *iter) return true;
    }
    return false;
}

template <typename T>
Automata<T>::~Automata(){

}

template <typename T>
Automata<T>::Conjunto::Conjunto(Name name, T valor){
    this->name = name;
    valores.push_back(valor);
    marcado = true;
}

template <typename T>
Automata<T>::Conjunto::Conjunto(Name name){
    this->name = name;
    marcado = true;
}

template <typename T>
Automata<T>::Conjunto::Conjunto(){
    marcado = true;
}


template <typename T>
Automata<T>::Conjunto::~Conjunto(){

}

#endif // AUTOMATA_H
