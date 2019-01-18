#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <fstream>
#include <list>
#include <map>
#include "../Error/error.h"

using namespace std;

#define VACIO 126

typedef int IdEstado;

class Estado;

typedef tuple<Estado *,char, Estado *> Transicion;

class Estado{
    public:
        Estado(IdEstado id){
            this->id = id;
        }
        Estado(IdEstado id, vector<Estado *> subEstados){
            this->id = id;
            vector<IdEstado> subConjunto;
            cadenaSubConjunto = "[";
            for(Estado * estado : subEstados){
                this->subEstados.push_back(estado);
                subConjunto.push_back(estado->id);
            }
            sort(subConjunto.begin(), subConjunto.end());
            for(IdEstado id : subConjunto){
                cadenaSubConjunto = cadenaSubConjunto + to_string(id) + " ";
            }
            cadenaSubConjunto.pop_back();            
            cadenaSubConjunto.push_back(']');
        }
        IdEstado id;
        vector<Estado *> subEstados;
        vector<Transicion> transiciones;
        string cadenaSubConjunto;
}; 


class Automata{
public:
    Automata(){};
    Automata(string file);
    Automata(char c, IdEstado &estadoActual);

    void printAutomata(ostream &file, bool flag);
    Estado * findEstado(IdEstado id);
    vector<Estado *> deleteRepeat(vector<Estado *> estadosV);
    vector<Estado *> e_clausura(Estado * estado);
    vector<Estado *> e_clausura(vector<Estado *> estadosV);
    vector<Estado *> findTransiciones(vector<Estado *> estadoV, char caracter);
    Estado * findSubConjunto(vector<Estado *> subConjunto);
    bool esAceptacion(IdEstado id);

    void deleteAutomata(){
        for(Estado * estado : estados){
            delete estado;
        }
    }

    string expresionRegular;
    vector<Estado *> estados;
    Estado * inicial;
    vector<Estado *> aceptacion;
    vector<char> entradas;
    vector<Transicion> transiciones;

};

Automata::Automata(string fileName){
    ifstream file(fileName.c_str());
    string line = "";
    int estado = 0;
    while(file>>line){
        if(estado == 0){
            file>>line;
            file>>line;
            expresionRegular = line;
            file>>line;
            if(line != "Estados") throw(Error(READ_AUTOMATA_LEX_ESTADOS,line));
            estado = 1;
        }
        else if(estado == 1){
            int numEstados = stoi(line);
            for(int i = 0; i < numEstados; i++){
                file>>line;
                estados.push_back(new Estado(stoi(line)));                
            }
            file>>line;
            if(line != "Inicial") throw(Error(READ_AUTOMATA_LEX_INICIAL,line));
            estado = 2;
        }
        else if(estado == 2){
            inicial = findEstado(stoi(line));
            if(inicial == nullptr)  throw(Error(READ_AUTOMATA_ESTADO_INICIAL,line));
            file>>line;
            if(line != "Aceptacion") throw(Error(READ_AUTOMATA_LEX_ACEPTACION,line));
            estado = 3;
        }
        else if(estado == 3){
            int numEstados = stoi(line);
            for(int i = 0; i < numEstados; i++){
                file>>line;
                auto temp = findEstado(stoi(line));
                if(temp == nullptr) throw(Error(READ_AUTOMATA_ESTADO_ACEPTACION,line));
                aceptacion.push_back(temp);
            }
            file>>line;
            if(line != "Entradas") throw(Error(READ_AUTOMATA_LEX_ENTRADAS,line));
            estado = 4;
        }
        else if(estado == 4){
            int numEntradas = stoi(line);
            for(int i = 0; i < numEntradas; i++){
                file>>line;
                entradas.push_back(line.front());
            }
            file>>line;
            if(line != "Transiciones") throw(Error(READ_AUTOMATA_LEX_TRANSICIONES,line));
            estado = 5;
        }
        else if(estado == 5){
            int numTransiciones = stoi(line);
            for(int i = 0; i < numTransiciones; i++){
                file>>line;
                int id1 = stoi(line);
                auto estado1 = findEstado(id1);
                if(estado1 == nullptr) throw(Error(READ_AUTOMATA_TRANSICION_ESTADO,line));
                file>>line;
                char entrada = line.front();
                if(entrada != VACIO){
                    auto temp = find(entradas.begin(), entradas.end(),entrada);
                    if(temp == entradas.end()) throw(Error(READ_AUTOMATA_TRANSICION_ENTRADA,line));    
                }
                file>>line;
                int id2 = stoi(line);
                auto estado2 = findEstado(id2);
                if(estado2 == nullptr) throw(Error(READ_AUTOMATA_TRANSICION_ESTADO,line));
                transiciones.push_back(make_tuple(estado1,entrada,estado2));
                estado1->transiciones.push_back(make_tuple(nullptr,entrada,estado2));
            }
            estado = 6;
        }
    }
    if(estado != 6) throw(Error(READ_AUTOMATA_END,line));
}

Automata::Automata(char c, IdEstado &estadoActual){
    expresionRegular.push_back(c);
    estados.push_back(new Estado(estadoActual));
    estadoActual++;
    estados.push_back(new Estado(estadoActual));
    estadoActual++;
    inicial = estados.front();
    aceptacion.push_back(estados.back());
    entradas.push_back(c);
    transiciones.push_back(make_tuple(inicial,c,aceptacion.front()));
    inicial->transiciones.push_back(make_tuple(nullptr,c,aceptacion.front()));
}

void Automata::printAutomata(ostream & file, bool flag){
    file<<"Automata de "<<expresionRegular<<endl;
    file<<"Estados"<<endl;
    file<<estados.size()<<endl;
    for(Estado * estado : estados){
        if(flag) file<<estado->id<<" "<<estado->cadenaSubConjunto<<endl;
        else file<<estado->id<<" ";
    }
    if(!flag) file<<endl;
    file<<"Inicial"<<endl;
    file<<inicial->id<<endl;
    file<<"Aceptacion"<<endl;
    file<<aceptacion.size()<<endl;
    for(Estado * estado : aceptacion){
        file<<estado->id<<" ";
    }
    file<<endl;
    file<<"Entradas"<<endl;
    file<<entradas.size()<<endl;
    for(char c : entradas){
        file<<c<<' ';
    }
    file<<endl;
    file<<"Transiciones"<<endl;
    file<<transiciones.size()<<endl;
    Estado * estado1 = nullptr;
    Estado * estado2 = nullptr;
    char c;
    for(Transicion tran : transiciones){
        tie(estado1,c,estado2) = tran;
        file<<estado1->id<<" "<<c<<" "<<estado2->id<<endl;
    }
}

Estado * Automata::findEstado(IdEstado id){
    for(Estado * res : estados){
        if(res->id == id) return res;
    }
    return nullptr;
}

vector<Estado *> Automata::deleteRepeat(vector<Estado *> estadosV){
    vector<Estado *> res;
    vector<IdEstado> temp;
    for(Estado * estado : estadosV){
        temp.push_back(estado->id);
    }
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(),temp.end()),temp.end());
    for(IdEstado id : temp){
        res.push_back(findEstado(id));
    }
    return res;
}

vector<Estado *> Automata::e_clausura(Estado * estado){
    map<IdEstado,bool> flags;
    list<Estado *> pila;
    vector<Estado *> res;
    for(Estado * estado : estados){
        flags[estado->id] = false;
    }
    Estado * actual = nullptr;
    Estado * estado1 = nullptr;
    Estado * estado2 = nullptr;
    char entrada;
    res.push_back(estado);
    pila.push_front(estado);
    while(!pila.empty()){
        actual = pila.front();
        pila.pop_front();
        flags[actual->id] = true;
        for(Transicion transicion : actual->transiciones){
            tie(estado1,entrada,estado2) = transicion;
            if(entrada == VACIO and !flags[estado2->id]){
                pila.push_front(estado2);
                res.push_back(estado2);
            }
        }
    }
    return res;
}

vector<Estado *> Automata::e_clausura(vector<Estado *> estadosV){
    vector<Estado *> res;
    for(Estado * estado : estadosV){
        vector<Estado *> temp = e_clausura(estado);
        res.insert(res.begin(),temp.begin(),temp.end());
    }
    res = deleteRepeat(res);
    return res;
}

vector<Estado *> Automata::findTransiciones(vector<Estado *> estadosV, char caracter){
    vector<Estado *> res;
    Estado * estado1 = nullptr;
    Estado * estado2 = nullptr;
    char entrada;
    for(Estado * estado : estadosV){
        for(Transicion transicion : estado->transiciones){
            tie(estado1, entrada, estado2) = transicion;
            if(entrada == caracter) res.push_back(estado2);
        }
    }
    res = deleteRepeat(res);
    return res;
}

Estado * Automata::findSubConjunto(vector<Estado *> subConjunto){
    if(subConjunto.empty()) return nullptr;
    vector<IdEstado> subConjuntoId;
    string comp = "[";
    for(Estado * estado : subConjunto){
        subConjuntoId.push_back(estado->id);
    }
    sort(subConjuntoId.begin(),subConjuntoId.end());
    for(IdEstado id : subConjuntoId){
        comp = comp + to_string(id) + " ";
    }
    comp.pop_back();
    comp.push_back(']');
    for(Estado * estado : estados){
        if(!estado->cadenaSubConjunto.empty()){
            if(estado->cadenaSubConjunto == comp) return estado;    
        }
    }
    return nullptr;
}

bool Automata::esAceptacion(IdEstado id){
    for(Estado * estado : aceptacion){
        if(estado->id == id) return true;
    }
    return false;
}


#endif