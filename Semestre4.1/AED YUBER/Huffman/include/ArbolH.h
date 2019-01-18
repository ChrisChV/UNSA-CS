#ifndef ARBOLH_H
#define ARBOLH_H
#include "list"
#include "FibonacciHeap.h"
#include "fstream"
#include "Monticulo.h"

#define CARACTER_NULL 126
#define END_L 94

using namespace std;

class ArbolH
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(char caracter, float valor);
                Nodo(float valor);
                Nodo * hijos[2];
                Nodo * padre;
                char caracter;
                bool buscarHijo(Nodo *& hijo);
                void _reducirNodo(string &);
                void _generarNodo(string::iterator &,string::iterator endI,map<char, Nodo *> &hojas);
                float valor;
        };
        class NodoDTO{
            public:
                NodoDTO(){nodo = nullptr;};
                NodoDTO(Nodo * nodo){this->nodo = nodo;};
                Nodo * nodo;
                bool operator == (NodoDTO second){
                    if(this->nodo->valor == second.nodo->valor) return true;
                    return false;
                };
                bool operator != (NodoDTO second){
                    if(this->nodo->valor != second.nodo->valor) return true;
                    return false;
                };
                bool operator < (NodoDTO second){
                    if(this->nodo->valor < second.nodo->valor) return true;
                    return false;
                };
                bool operator > (NodoDTO second){
                    if(this->nodo->valor > second.nodo->valor) return true;
                    return false;
                };
        };
        ArbolH();
        ArbolH(char caracter, float valor);
        void uni(vector<NodoDTO> &arboles);
        string codificar(char caracter);
        string  reducirArbol();
        void imprimirAlfabeto();
        char decodificar(ifstream &archvio, char &caracter);
        void print();
        void generarArbol(string cod);
        virtual ~ArbolH();
    protected:
    private:
        Nodo * root;
        map<char, Nodo *> hojas;
        int siz;
        void _uni(Nodo * root2);
};

void ArbolH::Nodo::_generarNodo(string::iterator &iter, string::iterator endI,map<char, Nodo *> &hojas){
    if(iter == endI) return;
    if((*iter) != '0' and (*iter) != '1'){
        this->caracter = *iter;
        iter++;
        hojas[caracter] = this;
        return;
    }
    if((*iter) == '0'){
        hijos[0] = new Nodo();
        hijos[0]->padre = this;
        iter++;
        hijos[0]->_generarNodo(iter,endI,hojas);
    }
    if((*iter) == '1'){
        hijos[1] = new Nodo();
        hijos[1]->padre = this;
        iter++;
        hijos[1]->_generarNodo(iter,endI,hojas);
    }
}

void ArbolH::generarArbol(string cod){
    auto iter = cod.begin();
    root = new Nodo();
    root->_generarNodo(iter,cod.end(),hojas);
}

void ArbolH::imprimirAlfabeto(){
    for(auto iter = hojas.begin(); iter != hojas.end(); ++iter){
        cout<<"ALFB->"<<iter->first<<endl;
    }
}

void ArbolH::Nodo::_reducirNodo(string &res){
    char a = caracter;
    if(caracter != CARACTER_NULL){
        res.insert(res.end(),caracter);
        return;
    }
    res.insert(res.end(),'0');
    hijos[0]->_reducirNodo(res);
    res.insert(res.end(),'1');
    hijos[1]->_reducirNodo(res);
}

string ArbolH::reducirArbol(){
    string res;
    if(!root) return res;
    root->_reducirNodo(res);
    return res;
}

bool ArbolH::Nodo::buscarHijo(Nodo *&hijo){
    if(hijos[0] == hijo) return false;
    return true;
}

char ArbolH::decodificar(ifstream &archivo, char &caracter){
    Nodo * temp = root;
    while(temp){
        if(temp->caracter != CARACTER_NULL){
            if(temp->caracter ==     END_L) return '\n';
            return temp->caracter;
        }
        temp = temp->hijos[caracter == '1'];
        if(!archivo.get(caracter)) return CARACTER_NULL;
    }
    return CARACTER_NULL;
}

string ArbolH::codificar(char caracter){
    if(caracter == '\n') caracter = END_L;
    if(hojas.find(caracter) == hojas.end()) return  "ERROR";
    string res;
    Nodo * temp = hojas[caracter]->padre;
    Nodo * h = hojas[caracter];
    while(temp){
        char r = '0';
        if(temp->buscarHijo(h)) r = '1';
        res.insert(res.begin(),r);
        h = temp;
        temp = temp->padre;
    }
    return res;
}

void ArbolH::print(){
    ofstream archivo("eje.dot");
    list<Nodo *> nodos;
    if(root) nodos.push_back(root);
    archivo<<"digraph{"<<endl;
    for(auto iter = nodos.begin(); iter != nodos.end(); iter++){
        archivo<<(*iter)->valor<<endl;
        if((*iter)->hijos[0]){
            archivo<<(*iter)->valor<<"->"<<(*iter)->hijos[0]->valor<<endl;
            nodos.push_back((*iter)->hijos[0]);
        }
        if((*iter)->hijos[1]){
            archivo<<(*iter)->valor<<"->"<<(*iter)->hijos[1]->valor<<endl;
            nodos.push_back((*iter)->hijos[1]);
        }
    }
    archivo<<"}";
    system("dot -Tpdf eje.dot -o eje.pdf");
}

void ArbolH::_uni(Nodo * root2){
    Nodo * nuevo = new Nodo(this->root->valor + root2->valor);
    this->root->padre = nuevo;
    root2->padre = nuevo;
    nuevo->hijos[0] = this->root;
    nuevo->hijos[1] = root2;
    root = nuevo;
}

void ArbolH::uni(vector<NodoDTO> &arboles){
    if(!root){
        root = getMin(arboles).nodo;
        if(root->caracter == '\n') root->caracter = END_L;
        hojas[root->caracter] = root;
        deleteMin(arboles);
    }
    Nodo * n = getMin(arboles).nodo;
    if(n->caracter == '\n') n->caracter = END_L;
    _uni(n);
    if(n->caracter != CARACTER_NULL) hojas[n->caracter] = n;
    deleteMin(arboles);
    minHeapinsert(arboles,NodoDTO(root));
    if(arboles.size() != 1){
        root = nullptr;
        uni(arboles);
    }
}

ArbolH::Nodo::Nodo(float valor){
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    padre = nullptr;
    this->caracter = CARACTER_NULL;
    this->valor = valor;
}

ArbolH::Nodo::Nodo(char caracter, float valor){
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    padre = nullptr;
    this->caracter = caracter;
    this->valor = valor;
}

ArbolH::Nodo::Nodo(){
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    padre = nullptr;
    caracter = CARACTER_NULL;
    valor = -1;
}

ArbolH::ArbolH(char caracter, float valor){
    root = new Nodo(caracter,valor);
    siz = 1;
}

ArbolH::ArbolH(){
    root = nullptr;
    siz = 0;
}

ArbolH::~ArbolH(){

}

#endif // ARBOLH_H
