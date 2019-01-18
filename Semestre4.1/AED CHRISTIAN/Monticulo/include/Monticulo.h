#ifndef MONTICULO_H
#define MONTICULO_H
#include "vector"
#include "algorithm"
#include "fstream"

using namespace std;

int _izquierda(int);
int _derecha(int);
int _padre(int);
template <typename T>
void minheapFy(vector<T> &, int);
template <typename T>
void maxheapFy(vector <T> &, int);
template <typename T>
void maxheapFy(vector <T> &, int,int);
template <typename T>
bool esHoja(vector<T> &, int);
template <typename T>
void makeMinHeap(vector<T> &);
template <typename T>
void makeMaxHeap(vector<T> &);
template <typename T>
void heapSort(vector<T> &);
template <typename T>
void print(vector<T> &);
template <typename T>
void minHeapDecreaseKey(vector<T> &, int);
template <typename T>
void maxHeapDeceaseKey(vector<T> &, int);
template <typename T>
T getMin(vector<T> &);
template <typename T>
void deleteMin(vector<T> &);
template <typename T>
void insert(vector<T> &vec, T);

template <typename T>
void minHeapinsert(vector<T> &vec, T valor){
    vec.pop_back(valor);
    minHeapDecreaseKey(vec, vec.size() - 1);
}

template <typename T>
void deleteMin(vector<T> &vec){
    swap(vec[0], vec[vec.size - 1]);
    vec.pop_back();
    minheapFy(vec,0);
}

template <typename T>
T getMin(vector<T> &vec){
    if(vec.empty())return;
    return vec.front();
}

template <typename T>
void maxHeapDeceaseKey(vector<T> &vec, int index){
    if(index == 0)return;
    if(vec[index] < vec[_padre(index)])swap(vec[index],vec[_padre(index)]);
    maxHeapDeceaseKey(vec,_padre(index));
}

template<typename T>
void minHeapDecreaseKey(vector<T> &vec, int index){
    if(index == 0)return;
    if(vec[index] > vec[_padre(index)])swap(vec[index],vec[_padre(index)]);
    minHeapDecreaseKey(vec,_padre(index));
}

template <typename T>
void print(vector<T> &vec){
    ofstream monti("monti.dot");
    if(monti.fail()){
        cout<<"El archvio no se pudo abrir"<<endl;
        return;
    }
    monti<<"digraph{"<<endl;
    for(int i = 0; i < vec.size(); i++){
        if(_izquierda(i) < vec.size())monti<<vec[i]<<"->"<<vec[_izquierda(i)]<<endl;
        if(_derecha(i) < vec.size())monti<<vec[i]<<"->"<<vec[_derecha(i)]<<endl;
    }
    monti<<"}";
    monti.close();
}

template <typename T>
void heapSort(vector<T> &vec){
    makeMaxHeap(vec);
    auto b = vec.size();
    auto s = vec.size() - 1;
    for(int i = 0; i < b - 1; i++){
        swap(vec[0], vec[s]);
        s--;
        maxheapFy(vec,0,s);
    }

}

template <typename T>
void makeMaxHeap(vector<T> &vec){
    for(int i = vec.size() / 2; i >= 0; i--){
        maxheapFy(vec, i);
    }
}

template <typename T>
void makeMinHeap(vector<T> &vec){
    for(int i = vec.size() / 2; i >= 0; i--){
        minheapFy(vec,i);
    }
}


template <typename T>
bool esHoja(vector <T> &vec, int index){
    auto b = vec.size();
    if(_derecha(index) > b and _izquierda(index) > b)return true;
    return false;
}

int _derecha(int index){
    return 2 * index + 2;
}

int _izquierda(int index){
    return 2 * index + 1;
}

int _padre(int index){
    return (index - 1)/2;
}
template <typename T>
void minheapFy(vector<T> &vec, int index){
    if(esHoja(vec,index))return;
    int menor = index;
    if(vec[_izquierda(index)] < vec[menor])menor = _izquierda(index);
    if(vec[_derecha(index)] < vec[menor] and _derecha(index) < vec.size())menor = _derecha(index);
    if(menor == index)return;
    swap(vec[index], vec[menor]);
    minheapFy(vec, menor);

}

template <typename T>
void maxheapFy(vector<T> &vec, int index, int tam){
    if(_izquierda(index) > tam)return;
    if(esHoja(vec,index))return;
    int mayor = index;
    if(vec[_izquierda(index)] > vec[mayor])mayor = _izquierda(index);
    if(vec[_derecha(index)] > vec[mayor] and _derecha(index) < tam)mayor = _derecha(index);
    if(mayor == index)return;
    swap(vec[index], vec[mayor]);
    maxheapFy(vec, mayor,tam);
}

template <typename T>
void maxheapFy(vector<T> &vec, int index){
    if(esHoja(vec,index))return;
    int mayor = index;
    if(vec[_izquierda(index)] > vec[mayor])mayor = _izquierda(index);
    if(vec[_derecha(index)] > vec[mayor])mayor = _derecha(index);
    if(mayor == index)return;
    swap(vec[index], vec[mayor]);
    maxheapFy(vec, mayor);
}



#endif // MONTICULO_H
