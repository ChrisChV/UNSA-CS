#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H
#include "fstream"
#include "iostream"
#include "algorithm"


#define MENOSINFINITO -100

using namespace std;

template <typename T>
class BinomialHeap
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T);
                T valor;
                Nodo * padre;
                Nodo * hijo;
                Nodo * hermano;
                void destruirme();
                bool find(T valor, Nodo **&);
                void print(ofstream &);
                int _grado();
        };
        class Root{
            public:
                Root();
                Root(Nodo *);
                Nodo * contenido;
                Root * siguiente;
                int grado();
                void destruirme();
                void print(ofstream &);
        };
        BinomialHeap();
        BinomialHeap(string);
        void deleteRoot(Root **&);
        BinomialHeap(string,bool);
        Root *& getInicio();
        void print(string);
        void print(string, Root*&);
        void decreasekey(Nodo *);
        bool findTree(T valor, Root **&);
        bool find(T valor, Nodo **&);
        void deleteTree(T);
        void insert(T valor);
        void uni(Root *&);
        void deleteMin();
        void deleteNodo(T);
        T returnMin();
        virtual ~BinomialHeap();
    protected:
    private:
        Root * inicio;
        bool animation;
        string name;
        void _merge(Root *&);
        void _herencia(Root *, Root *);
        void _createHeap(BinomialHeap &, Nodo*);
        bool searchMin(Root **&);
};
template <typename T>
T BinomialHeap<T>::returnMin(){
    try{
        Root ** result;
        if(!searchMin(result)){
            string e = "La lista esta vacía";
            throw(e);
        }
        return (*result)->contenido->valor;
    }
    catch(string e){
        cout<<e<<endl;
    }
}

template <typename T>
void BinomialHeap<T>::deleteNodo(T valor){
    if(animation)cout<<"ELIMINANDO EL VALOR -> "<<valor<<endl;
    Nodo ** nodo;
    if(!this->find(valor,nodo)){
        if(animation)cout<<"NO SE ENCONTRO EL VALOR -> "<<valor<<endl;
        return;
    }
    (*nodo)->valor = MENOSINFINITO;
    if(animation){
        print("Head" + name + " despues de reemplazar menosinfinito");
        char tt;
        cin>>tt;
    }
    decreasekey(*nodo);
    Root ** menor;
    deleteMin();
}

template <typename T>
void BinomialHeap<T>::deleteMin(){
    if(animation)cout<<"ELIMINANDO EL MENOR"<<endl;
    BinomialHeap secondHeap("seconTemp");
    Root **root;
    if(!searchMin(root))return;
    _createHeap(secondHeap, (*root)->contenido->hijo);
    //*root = (*root)->siguiente;

    Root * a = nullptr;
    Root * b = inicio;
    while(b){
        if(b == (*root))break;
        if(a == nullptr){
            a = inicio;
        }
        else a = a->siguiente;
        b = b->siguiente;
    }
    if(!a){
        inicio = b->siguiente;
    }
    else{
        a->siguiente = b->siguiente;
    }
    uni(secondHeap.inicio);
}

template <typename T>
bool BinomialHeap<T>::searchMin(Root **&root){
    if(!inicio)return false;
    auto menor = inicio;
    auto iter = inicio->siguiente;
    while(iter){
        if(iter->contenido->valor < menor->contenido->valor){
            menor = iter;
        }
        iter = iter->siguiente;
    }
    root = &(menor);
    return true;
}

template <typename T>
void BinomialHeap<T>::_createHeap(BinomialHeap & heap, Nodo * hijoPrimerizo){
    if(!hijoPrimerizo)return;
    auto iter = hijoPrimerizo;
    heap.getInicio() = nullptr;
    heap.getInicio() = new Root(iter);
    iter = iter->hermano;
    while(iter){
        auto temp = heap.getInicio();
        heap.getInicio() = new Root(iter);
        heap.getInicio()->siguiente = temp;
        iter = iter->hermano;
    }
}

template <typename T>
void BinomialHeap<T>::deleteRoot(Root **&root){
    BinomialHeap secondHeap("seconTemp");
    _createHeap(secondHeap, (*root)->contenido->hijo);
    auto temp =  *root;
    *root = (*root)->siguiente;
    print("tercero");
    ///temp->destruirme();
    secondHeap.print("segundo");
    //uni(secondHeap.getInicio());
}

template <typename T>
void BinomialHeap<T>::decreasekey(Nodo * nodo){
    while(nodo->padre){
        if(nodo->valor < nodo->padre->valor){
            swap(nodo->valor, nodo->padre->valor);
            if(animation){
                print("Heap " + name + "despues de un swap");
                char tt;
                cin>>tt;
            }
        }
        nodo = nodo->padre;
    }
}

template <typename T>
bool BinomialHeap<T>::Nodo::find(T valor, Nodo **& nodo){
    if(this->valor == valor)return true;
    if(this->hermano){
        nodo = &(this->hermano);
        if(this->hermano->find(valor, nodo))return true;
    }
    if(this->hijo){
        nodo = &(this->hijo);
        if(this->hijo->find(valor, nodo))return true;
    }
    return false;
}

template <typename T>
bool BinomialHeap<T>::find(T valor, Nodo **& nodo){
    Root * iter = inicio;
    nodo = &(inicio->contenido);
    while(iter){
        if(iter->contenido->find(valor, nodo)) return true;
        iter = iter->siguiente;
        nodo = &(iter->contenido);
    }
    return false;
}

template <typename T>
bool BinomialHeap<T>::findTree(T valor, Root **&iter){
    iter = &inicio;
    while(*iter){
        if((*iter)->contenido->valor == valor) return true;
        iter = &((*iter)->siguiente);
    }
    return false;
}

template <typename T>
void BinomialHeap<T>::deleteTree(T valor){
    Root ** root;
    if(!findTree(valor, root))return;
    Root * temp =  *root;
    *root = (*root)->siguiente;
    temp->destruirme();
}

template <typename T>
void BinomialHeap<T>::insert(T valor){
    if(animation)cout<<"INSERTANDO EL VALOR -> "<<valor<<endl;
    Nodo ** nodo;
    if(this->find(valor,nodo))return;
    Root *nuevo = new Root(new Nodo(valor));
    uni(nuevo);
}

template <typename T>
void BinomialHeap<T>::Nodo::print(ofstream &archivo){
    archivo<<this->padre->valor<<"->"<<this->valor<<endl;
    if(this->hermano){
        this->hermano->print(archivo);
    }
    else return;
    if(this->hijo){
        this->hijo->print(archivo);
    }
}

template <typename T>
void BinomialHeap<T>::Root::print(ofstream &archivo){
    if(this->contenido){
        if(this->contenido->hijo)
            this->contenido->hijo->print(archivo);
        else{
            archivo<<this->contenido->valor<<endl;
        }
    }
}

template <typename T>
void BinomialHeap<T>::print(string mensaje, Root *&secondInicio){
    cout<<mensaje<<endl;
    string nombreArchivo = name + ".dot";
    ofstream archivo(nombreArchivo);
    if(archivo.fail()){
        cout<<"ERROR->El archivo n ose pudo abrir";
        return;
    }
    archivo<<"digraph{"<<endl;
    Root * temp = inicio;
    while(temp){
        temp->print(archivo);
        temp = temp->siguiente;
    }
    temp = secondInicio;
    while(temp){
        temp->print(archivo);
        temp = temp->siguiente;
    }
    archivo<<"}";
    archivo.close();
    string comando = "dot -Tpdf " + nombreArchivo + " -o " + name + ".pdf";
    const char *c;
    c = comando.c_str();
    system(c);
}

template <typename T>
void BinomialHeap<T>::print(string mensaje){
    cout<<mensaje<<endl;
    string nombreArchivo = name + ".dot";
    ofstream archivo(nombreArchivo);
    if(archivo.fail()){
        cout<<"ERROR->El archivo no se pudo abrir"<<endl;
        return;
    }
    archivo<<"digraph{"<<endl;
    Root * temp = inicio;
    while(temp){
        temp->print(archivo);
        temp = temp->siguiente;
    }
    archivo<<"}";
    archivo.close();
    string comando = "dot -Tpdf " + nombreArchivo + " -o " + name + ".pdf";
    const char *c;
    c = comando.c_str();
    system(c);
}

template <typename T>
void BinomialHeap<T>::_merge(Root *&secondInicio){
    Root ** firstIter = &(this->inicio);
    Root ** secondIter = &(secondInicio);
    while(*secondIter){
        auto temp1 = inicio;
        if(*firstIter == nullptr){
            if(*firstIter == inicio){
                this->inicio = *secondIter;
            }
            else{
                *firstIter = *secondIter;
            }
            break;
        }
        else if((*firstIter)->grado() > (*secondIter)->grado()){
            auto temp = (*firstIter);
            auto temp2 = (*secondIter)->siguiente;
            *firstIter = *secondIter;
            (*firstIter)->siguiente = temp;
            firstIter = &((*firstIter)->siguiente);
            secondIter = &(temp2);
        }
        else if((*firstIter)->siguiente == nullptr){
            (*firstIter)->siguiente = *secondIter;
            break;
        }
        else if((*firstIter)->grado() == (*secondIter)->grado()){
            Root * temp = (*firstIter)->siguiente;
            Root * temp2 = (*secondIter)->siguiente;
            (*firstIter)->siguiente = *secondIter;
            (*firstIter)->siguiente->siguiente = temp;
            firstIter = &((*firstIter)->siguiente->siguiente);
            secondIter = &(temp2);
        }
    }
}
///Intentar poner el delete
template <typename T>
void BinomialHeap<T>::_herencia(Root *menor, Root *mayor){
    mayor->contenido->hermano = menor->contenido->hijo;
    menor->contenido->hijo = mayor->contenido;
    mayor->contenido->padre = menor->contenido;
}

template <typename T>
void BinomialHeap<T>::uni(Root *&secondInicio){
    if(!secondInicio){
        if(animation){
            print("Heap " + name + " despues de un cambio");
            char tt;
            cin>>tt;
        }
        return;
    }
    if(animation){
        print("Heap " + name + " antes del merge",secondInicio);
        char tt;
        cin>>tt;
    }
    _merge(secondInicio);


    secondInicio = nullptr;

    //secondInicio->destruirme();
    Root * iter = inicio;
    Root * nextIter = inicio->siguiente;
    Root * prevIter = nullptr;

    auto temp = inicio;
    while(nextIter){
        if(iter->grado() != nextIter->grado()){
            if(!prevIter)prevIter = inicio;
            nextIter = nextIter->siguiente;
            iter = iter->siguiente;
        }
        else if(nextIter->siguiente and nextIter->siguiente->grado() == nextIter->grado()){
                if(!prevIter)prevIter = inicio;
                nextIter = nextIter->siguiente;
                iter = iter->siguiente;
            }
        ///ver si se puede hacer el delete
        else if(iter->contenido->valor < nextIter->contenido->valor){
            _herencia(iter, nextIter);
            if(prevIter) prevIter->siguiente = iter;
            iter->siguiente = nextIter->siguiente;
            nextIter = nextIter->siguiente;
        }
        else{
            _herencia(nextIter, iter);
            if(iter == inicio){
                inicio = nextIter;
            }
            if(prevIter) prevIter->siguiente = nextIter;
            iter = nextIter;
            nextIter = nextIter->siguiente;
        }
        if(animation){
            print("Heap " + name + " despues de una comparacion");
            char tt;
            cin>>tt;
        }
    }
}

template <typename T>
int BinomialHeap<T>::Nodo::_grado(){
    if(this->hijo)return this->hijo->_grado() + 1;
    return 0;
}

template <typename T>
int BinomialHeap<T>::Root::grado(){
    if(this->contenido){
        if(this->contenido->hijo)
        return this->contenido->hijo->_grado() + 1;
    }
    return 0;
}

template <typename T>
void BinomialHeap<T>::Root::destruirme(){
    if(this->contenido){
        if(this->contenido->hijo)
            this->contenido->hijo->destruirme();
    }
    if(this->siguiente)this->siguiente->destruirme();
    delete this;
}

template <typename T>
void BinomialHeap<T>::Nodo::destruirme(){
    if(this->hermano)this->hermano->destruirme();
    if(this->hijo)this->hijo->destruirme();
    delete this;
}

template <typename T>
typename BinomialHeap<T>::Root*& BinomialHeap<T>::getInicio(){
    return this->inicio;
}

template <typename T>
BinomialHeap<T>::Root::Root(Nodo *contenido){
    this->contenido = contenido;
    siguiente = nullptr;
}

template <typename T>
BinomialHeap<T>::Root::Root(){
    contenido = nullptr;
    siguiente = nullptr;
}

template <typename T>
BinomialHeap<T>::Nodo::Nodo(T valor){
    this->valor = valor;
    padre = nullptr;
    hijo = nullptr;
    hermano = nullptr;
}

template <typename T>
BinomialHeap<T>::Nodo::Nodo(){
    padre = nullptr;
    hijo = nullptr;
    hermano = nullptr;
}

template <typename T>
BinomialHeap<T>::BinomialHeap(string name, bool animation){
    this->name = name;
    this->animation = animation;
    inicio = nullptr;
}

template <typename T>
BinomialHeap<T>::BinomialHeap(string name){
    this->name = name;
    animation = false;
    inicio = nullptr;
}

template <typename T>
BinomialHeap<T>::BinomialHeap(){
    name = "BinomialHeapDefault";
    inicio = nullptr;
    animation = false;
}

template <typename T>
BinomialHeap<T>::~BinomialHeap(){
    if(inicio)
        if(inicio->contenido)
        inicio->destruirme();
}
#endif // BINOMIALHEAP_H
