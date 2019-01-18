#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H
#include "vector"
#include "list"
#include "fstream"
#include "iostream"

using namespace std;

template <typename T>
class SegmentTree
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T, int);
                Nodo(T, int, int);
                T valor;
                int init;
                int fin;
                bool state;
                Nodo *hijos[2];
                void destruirme();
                string label;
        };
        SegmentTree();
        SegmentTree(int (*)(vector<int>::iterator,vector<int>::iterator));
        T find(int,int);
        void print();
        void generate(vector<T>&);
        virtual ~SegmentTree();
    protected:
    private:
        Nodo * root;
        int (*funcion)(vector<int>::iterator,vector<int>::iterator);
        void _generate(vector<T>&, Nodo *&,int,int);
        T _find(Nodo *,int, int);
};

template <typename T>
T SegmentTree<T>::find(int init, int fin){
    return _find(root, init, fin);
}

template <typename T>
T SegmentTree<T>::_find(Nodo * nodo, int init, int fin){
    if(init <= nodo->init and fin >= nodo->fin){
        return nodo->valor;
    }
    else if((init >= nodo->init and init >= nodo->fin) or (fin <= nodo->init and fin <= nodo->fin)){
        return 0;
    }
    else{
        return _find(nodo->hijos[0],init,fin) + _find(nodo->hijos[1],init,fin);
    }
}

template <typename T>
void SegmentTree<T>::print(){
    ofstream archivo("eje.dot");
    if(archivo.fail()){
        cout<<"El archivo no se pudo abrir"<<endl;
        return;
    }
    archivo<<"digraph{"<<endl;
    list<Nodo *> result;
    if(root) result.push_back(root);
    for(auto iter = result.begin(); iter != result.end(); ++iter){
        string color;
        if((*iter)->state) color = "green";
        else color = "black";
        archivo<<(*iter)->label<<" [label=\""<<(*iter)->valor<<endl<<"SEGMENT=["<<(*iter)->init<<","<<(*iter)->fin<<"]\" color = \""<<color<<"\"];"<<endl;
        if((*iter)->hijos[0]){
            result.push_back((*iter)->hijos[0]);
            archivo<<(*iter)->label<<"->"<<(*iter)->hijos[0]->label<<endl;
        }
        if((*iter)->hijos[1]){
            result.push_back((*iter)->hijos[1]);
            archivo<<(*iter)->label<<"->"<<(*iter)->hijos[1]->label<<endl;
        }
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf eje.dot -o eje.pdf");
}

template <typename T>
void SegmentTree<T>::generate(vector<T> &arr){
    if(arr.empty())return;
    _generate(arr,root,0,arr.size()-1);
}

template <typename T>
void SegmentTree<T>::_generate(vector<T> &arr,Nodo *& nuevo,int init, int fin){
    if(init == fin){
        nuevo = new Nodo(arr[init], init);
        return;
    }
    nuevo = new Nodo(funcion(arr.begin() + init, arr.begin() + fin), init, fin);
    int temp = (fin - init) / 2;
    _generate(arr, nuevo->hijos[0],init,  init + temp);
    _generate(arr, nuevo->hijos[1],init + temp + 1, fin);
}

template <typename T>
void SegmentTree<T>::Nodo::destruirme(){
    if(hijos[0]) hijos[0]->destruirme();
    if(hijos[1]) hijos[1]->destruirme();
    delete this;
}

template <typename T>
SegmentTree<T>::Nodo::Nodo(T valor, int init, int fin){
    this->valor = valor;
    this->init = init;
    this->fin = fin;
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    state = false;
    label = to_string(valor) + to_string(init) + to_string(fin);
}

template <typename T>
SegmentTree<T>::Nodo::Nodo(T valor, int segment){
    this->valor = valor;
    init = segment;
    fin = segment;
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    state = true;
    label = to_string(valor) + to_string(init) + to_string(fin);
}

template <typename T>
SegmentTree<T>::Nodo::Nodo(){
    init = 0;
    fin = 0;
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    state = false;
}

template <typename T>
SegmentTree<T>::SegmentTree(int (*funcion)(vector<int>::iterator,vector<int>::iterator)){
    this->funcion = funcion;
    root = nullptr;
}

template <typename T>
SegmentTree<T>::SegmentTree(){
    root = nullptr;
}

template <typename T>
SegmentTree<T>::~SegmentTree(){
    if(root) root->destruirme();
}

#endif // SEGMENTTREE_H
