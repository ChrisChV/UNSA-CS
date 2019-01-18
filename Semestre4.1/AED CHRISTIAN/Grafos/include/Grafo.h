
#ifndef GRAFO_H
#define GRAFO_H
#include "map"
#include "list"
#include "fstream"
#include "iostream"
#include "FibonacciHeap.h"
#include "stack"
#include "queue"


#define INFINITO 1000;

using namespace std;

////////////PROBAR = STRING

class MyGrafo
{
    public:
        class Relacion{
            public:
                Relacion();
                Relacion(string,string,float);
                string nodos[2];
                string _getDestino(string);
                float peso;
        };
        class Nodo{
            public:
                Nodo();
                Nodo(string);
                list<Relacion *> relaciones;
                bool _existeRelacion(Nodo *,Relacion *&);
                string nombre;
                bool marcado;
        };
        class PesoDTO{
            public:
                PesoDTO(){nodos[0] = nullptr; nodos[1] = nullptr;peso = INFINITO;}
                PesoDTO(float p, Nodo * nodo1, Nodo * nodo2){nodos[0] = nodo1; nodos[1] = nodo2; peso = p;}
                float peso;
                Nodo * nodos[2];
                Nodo * _getDestino(Nodo * nodo1){
                    return nodos[nodo1->nombre == nodos[0]->nombre];
                }
                bool operator<(PesoDTO second){
                    if(peso < second.peso) return true;
                    return false;
                }
                bool operator>(PesoDTO second){
                    if(peso > second.peso) return true;
                    return false;
                }
                bool operator==(PesoDTO second){
                    if(peso == second.peso and nodos[0] == second.nodos[0] and nodos[1] == second.nodos[1])return true;
                    return false;
                }
        };
        MyGrafo();
        void print();
        void warshall();
        void floid();
        bool kruskall(MyGrafo&);
        bool prim(MyGrafo &);
        bool hayCamino(string,string);
        void DSF(void(*funcion)(Nodo *));
        void BSF(void(*funcion)(Nodo *));
        int relSize();
        int size();
        map<string,float> dijkstra(string i);
        list<Relacion *>& operator[](string);
        bool operator()(string,string,float);
        virtual ~MyGrafo();
    protected:
    private:
        bool _existeRelacion(Nodo *, Nodo *);
        bool _existeRelacion(Nodo * nodo1, Nodo * nodo2, Relacion *& rel);
        bool _existeNodo(string,Nodo*&);
        bool _crearRelacion(string,string,float);
        void _desmarcar();
        float _getPeso(Nodo * nodo1, Nodo * nodo2);
        float _getPeso(Nodo * nodo1, Nodo * nodo2,Relacion *&);
        bool _hayCamino(string a, Nodo * b, list<string>&);
        bool _llenarHeap_dijkstra(FibonacciHeap<PesoDTO> &,list<PesoDTO*>&, string);
        void _llenarHeap_kruskall(FibonacciHeap<PesoDTO> &);
        void _llenarHeap_prim(FibonacciHeap<PesoDTO> &,Nodo *);
        map<string,float> _generarLista(list<PesoDTO*>&);
        map<string, Nodo *> grafo;
        list<Relacion *> relaciones;

};

void MyGrafo::BSF(void(*funcion)(Nodo *)){
    _desmarcar();
    queue<Nodo *> fila;
    auto iter = grafo.begin();
    fila.push(iter->second);
    iter->second->marcado = true;
    while(!fila.empty()){
        Nodo * temp = fila.front();
        fila.pop();
        for(auto rel : temp->relaciones){
            Nodo * nodo;
            _existeNodo(rel->_getDestino(temp->nombre),nodo);
            if(!nodo->marcado){
                fila.push(nodo);
                nodo->marcado = true;
            }
        }
        funcion(temp);
    }
}

void MyGrafo::DSF(void(*funcion)(Nodo *)){
    _desmarcar();
    stack<Nodo *> cola;
    auto iter = grafo.begin();
    cola.push(iter->second);
    iter->second->marcado = true;
    while(!cola.empty()){
        Nodo * temp = cola.top();
        cola.pop();
        auto iter = temp->relaciones.end();
        iter--;
        for(iter; iter != temp->relaciones.begin(); --iter){
            Nodo * nodo;
            _existeNodo((*iter)->_getDestino(temp->nombre),nodo);
            if(!nodo->marcado) {
                cola.push(nodo);
                nodo->marcado = true;
            }
        }
        Nodo * nodo;
        _existeNodo((*iter)->_getDestino(temp->nombre),nodo);
        if(!nodo->marcado) {
            cola.push(nodo);
            nodo->marcado = true;
        }
        funcion(temp);
    }
}

void MyGrafo::_llenarHeap_prim(FibonacciHeap<PesoDTO> &heap, Nodo * nodo){
    for(auto rel : nodo->relaciones){
        Nodo * nodo1;
        Nodo * nodo2;
        _existeNodo(rel->nodos[0],nodo1);
        _existeNodo(rel->nodos[1],nodo2);
        if(!nodo1->marcado and !nodo2->marcado)
            heap.insert(PesoDTO(rel->peso,nodo1,nodo2));
    }
}

bool MyGrafo::prim(MyGrafo& resultado){
    FibonacciHeap<PesoDTO> heap;
    _desmarcar();
    auto iter = grafo.begin();
    _llenarHeap_prim(heap,iter->second);
    iter->second->marcado = true;
    Nodo * actual = iter->second;
    while(resultado.relSize() != grafo.size() - 1){
        if(heap.empty())return false;
        PesoDTO menor = heap.popMin();
        if(!menor.nodos[0]->marcado or !menor.nodos[1]->marcado){
            resultado[menor.nodos[0]->nombre];
            resultado[menor.nodos[1]->nombre];
            resultado(menor.nodos[0]->nombre,menor.nodos[1]->nombre,menor.peso);
            actual = menor._getDestino(actual);
            cout<<"ACTUAL->"<<actual->nombre<<endl;
            _llenarHeap_prim(heap,actual);
            actual->marcado = true;
        }
    }
    return true;
}

int MyGrafo::relSize(){
    return relaciones.size();
}

bool MyGrafo::_hayCamino(string a, Nodo * b, list<string> &visitados){
    if(find(visitados.begin(),visitados.end(),a) != visitados.end())return false;
    visitados.push_back(a);
    Nodo * nodo1;
    _existeNodo(a,nodo1);
    if(_existeRelacion(nodo1,b))return true;
    for(auto rel : nodo1->relaciones){
        if(_hayCamino(rel->_getDestino(a),b,visitados))return true;
    }
    return false;
}

bool MyGrafo::hayCamino(string a,string b){
    list<string> visitados;
    Nodo * nodo1;
    Nodo * nodo2;
    if(!_existeNodo(a,nodo1) or !_existeNodo(b,nodo2))return false;
    return _hayCamino(a,nodo2,visitados);
}

void MyGrafo::_llenarHeap_kruskall(FibonacciHeap<PesoDTO>& heap){
    for(auto rel : relaciones){
        Nodo * nodo1;
        Nodo * nodo2;
        _existeNodo(rel->nodos[0],nodo1);
        _existeNodo(rel->nodos[1],nodo2);
        nodo1->marcado = false;
        nodo2->marcado = false;
        cout<<"PPPPPPP->"<<rel->peso<<endl;
        PesoDTO nuevo(rel->peso,nodo1,nodo2);
        heap.insert(nuevo);
        cout<<"MIN->"<<heap.getMin().peso<<endl;
    }
}

bool MyGrafo::kruskall(MyGrafo &resultado){
    FibonacciHeap<PesoDTO> heap;
    _llenarHeap_kruskall(heap);
    int nodosSinVisitar = grafo.size();
    bool first = true;
    while(resultado.relSize() != grafo.size() - 1){
        if(heap.empty())return false;
        PesoDTO menor = heap.popMin();
        cout<<"PESO->"<<menor.peso<<endl;
        if( menor.nodos[0] != menor.nodos[1] and ((!menor.nodos[0]->marcado or !menor.nodos[1]->marcado) or !resultado.hayCamino(menor.nodos[0]->nombre,menor.nodos[1]->nombre))){
            cout<<"WWWWW->"<<menor.peso<<endl;
            menor.nodos[0]->marcado = true;
            menor.nodos[1]->marcado = true;
            resultado[menor.nodos[0]->nombre];
            resultado[menor.nodos[1]->nombre];
            resultado(menor.nodos[0]->nombre,menor.nodos[1]->nombre,menor.peso);
        }
    }
    return true;
}

bool MyGrafo::_llenarHeap_dijkstra(FibonacciHeap<PesoDTO> &heap,list<PesoDTO*> &distancias, string i){
    Nodo * nodo;
    if(!_existeNodo(i,nodo))return false;
    for(auto iter = grafo.begin(); iter!= grafo.end(); ++iter){
        iter->second->marcado = false;
        if(iter->first != i){
            heap.insert(PesoDTO(_getPeso(nodo,iter->second),nodo,iter->second));
            distancias.push_back(&(heap.back()));
        }
        else{
            distancias.push_back(new PesoDTO(_getPeso(nodo,iter->second),nodo,iter->second));
        }
    }
    return true;
}

void MyGrafo::_desmarcar(){
    for(auto iter = grafo.begin(); iter != grafo.end(); ++iter){
        iter->second->marcado = false;
    }
}

map<string,float> MyGrafo::_generarLista(list<PesoDTO*> &pesos){
    map<string,float> resultado;
    for(auto iter = pesos.begin(); iter != pesos.end(); ++iter){
        resultado[(*iter)->nodos[1]->nombre] = (*iter)->peso;
    }
    return resultado;
}

map<string,float> MyGrafo::dijkstra(string i){
    FibonacciHeap<PesoDTO> heap;
    list<PesoDTO*> distancias;
    if(!_llenarHeap_dijkstra(heap,distancias,i)) return map<string,float>();
    grafo[i]->marcado = true;
    int contadorDeCeros = grafo.size() - 1;
    while(contadorDeCeros > 0){
        PesoDTO pdto =  heap.popMin();
        int p = pdto.peso;
        PesoDTO pdto2 = PesoDTO();
        pdto.nodos[1]->marcado = true;
        contadorDeCeros--;
        for(auto iter = distancias.begin(); iter != distancias.end(); ++iter){
            if(!(*iter)->nodos[1]->marcado){
                cout<<"C"<<endl;
                (*iter)->peso = min((*iter)->peso, p + _getPeso(pdto.nodos[1],(*iter)->nodos[1]));
            }
        }
    }
    return _generarLista(distancias);
}

float MyGrafo::_getPeso(Nodo * nodo1, Nodo * nodo2,Relacion *&r){
    if(!nodo1 or !nodo2)return INFINITO;
    if(_existeRelacion(nodo1,nodo2,r)){
        return r->peso;
    }
    if(_existeRelacion(nodo2,nodo1,r)){
        return r->peso;
    }
    return INFINITO;
}

float MyGrafo::_getPeso(Nodo * nodo1, Nodo * nodo2){
    Relacion * r;
    return _getPeso(nodo1,nodo2,r);
}



void MyGrafo::floid(){
    for(auto i = grafo.begin(); i != grafo.end(); ++i){
        for(auto j = grafo.begin(); j != grafo.end(); ++j){
            for(auto k = grafo.begin(); k != grafo.end(); ++k){
                float a = _getPeso(i->second,k->second);
                float b = _getPeso(k->second,j->second);
                if(_getPeso(i->second,j->second) >  + a + b){
                    _crearRelacion(i->first,j->first, a + b);
                }
            }
        }
    }
}

bool MyGrafo::_existeRelacion(Nodo * nodo1, Nodo * nodo2){
    Relacion *rel;
    if(nodo1->_existeRelacion(nodo2,rel))return true;
    if(nodo2->_existeRelacion(nodo1,rel))return true;
    rel = nullptr;
    return false;
}

bool MyGrafo::_existeRelacion(Nodo * nodo1, Nodo * nodo2, Relacion *& rel){
    if(nodo1->_existeRelacion(nodo2,rel))return true;
    if(nodo2->_existeRelacion(nodo1,rel))return true;
    rel = nullptr;
    return false;
}

void MyGrafo::warshall(){
    for(auto i = grafo.begin(); i != grafo.end(); ++i){
        for(auto j = grafo.begin(); j != grafo.end(); ++j){
            for(auto k = grafo.begin(); k != grafo.end(); ++k){
                if(_existeRelacion(i->second,j->second) or (_existeRelacion(i->second,k->second) and _existeRelacion(k->second,j->second))){
                    _crearRelacion(i->first,j->first,1);
                }
            }
        }
    }
}

void MyGrafo::print(){
    ofstream archivo("grafo.dot");
    if(archivo.fail()){
        cout<<"El archivo no se pudo abrir"<<endl;
        return;
    }
    archivo<<"graph{"<<endl;
    for(auto rel : relaciones){
        Nodo * nodo1;
        Nodo * nodo2;
        _existeNodo(rel->nodos[0],nodo1) and _existeNodo(rel->nodos[1],nodo2);
        archivo<<nodo1->nombre<<" -- "<<nodo2->nombre<<"[label =\""<<rel->peso<<"\"];";
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf grafo.dot -o grafo.pdf");
}

bool MyGrafo::_crearRelacion(string nodo1, string nodo2, float peso){
    Nodo * n1;
    Nodo * n2;
    if(!_existeNodo(nodo1,n1) or !_existeNodo(nodo2,n2)){
        return false;
    }
    else{
        Relacion * r;
        Relacion * t;
        if(!n1->_existeRelacion(n2,r) and !n2->_existeRelacion(n2,t)){
            Relacion * rel = new Relacion(n1->nombre,n2->nombre,peso);
            relaciones.push_back(rel);
            n1->relaciones.push_back(rel);
            n2->relaciones.push_back(rel);
            return true;
        }
        if(r){
            r->peso = peso;
            return true;
        }
        if(t){
            t->peso = peso;
            return true;
        }
    }
}

bool MyGrafo::operator()(string nodo1, string nodo2, float peso){
    return _crearRelacion(nodo1, nodo2, peso);
}

string MyGrafo::Relacion::_getDestino(string name){
    ///////////PROBAR ACA
    return nodos[nodos[0] == name];
}

list<MyGrafo::Relacion *>& MyGrafo::operator[](string name){
    Nodo * nodo;
    if(!_existeNodo(name,nodo)){
        nodo = new Nodo(name);
        grafo[name] = nodo;
    }
    return grafo[name]->relaciones;
}

bool MyGrafo::Nodo::_existeRelacion(Nodo * nodo, Relacion *&rel){
    for(auto iter_rel : relaciones){
        rel = iter_rel;
        if(rel->_getDestino(nombre) == nodo->nombre)return true;
    }
    rel = nullptr;
    return false;
}

bool MyGrafo::_existeNodo(string name, Nodo *&nodo){
   auto iter = grafo.find(name);
   if(iter != grafo.end()){
       nodo = iter->second;
       return true;
   }
   return false;
}

MyGrafo::Nodo::Nodo(){
    nombre = nullptr;
    marcado = true;
}

MyGrafo::Nodo::Nodo(string name){
    nombre = name;
    marcado = false;
}

MyGrafo::Relacion::Relacion(string nodo1, string nodo2, float peso){
    nodos[0] = nodo1;
    nodos[1] = nodo2;
    this->peso = peso;
}

MyGrafo::Relacion::Relacion(){
    nodos[0] = nullptr;
    nodos[1] = nullptr;
    peso = INFINITO;
}


MyGrafo::MyGrafo(){

}

MyGrafo::~MyGrafo(){
    for(auto iter =grafo.begin(); iter != grafo.end(); ++iter){
        delete(iter->second);
    }
    for(auto rel : relaciones){
        delete(rel);
    }
}

#endif // GRAFO_H
