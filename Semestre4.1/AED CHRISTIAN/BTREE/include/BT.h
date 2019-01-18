#ifndef BT_H
#define BT_H
#include "list"
#include "vector"
#include "fstream"
#include "queue"
#include "algorithm"
#include "fstream"
#include <string>

using namespace std;

enum Hermano{IZQUIERDO,DERECHO};
enum EstadosHermano{NO_TIENE_H_IZQUIERDO,NO_TIENE_H_DERECHO};

template<typename T>
class BT
{
    public:
        BT();
        BT(string name, int maxD,bool leer);
        class Key{
            public:
                Key();
                Key(T,int,int,int);
                T key;
                int posIndex;
                int nodoAnterior;
                int nodoSiguiente;
                virtual ~Key(){
                };
        };
        class Nodo{
            public:
                Nodo();
                Nodo(int);
                Nodo(int,vector<Key *>& ,int,bool);
                vector<Key *> keys;
                void erase(int med);
                bool findInNodo(T,int&);
                bool estoyLleno();
                bool puedoPrestar();
                bool estoyUnderFlow();
                bool soyHoja();
                int maxDegree;
                void deleteNodo();
                int index;
                bool flag;

        };
        Nodo readDisk(int);
        int writeDisk(Nodo &);
        void deleteDisk(int);
        void eliminarArbol();
        void del(T key);
        bool find(T key);
        void insert(T key, int pos);
        void insert(T key, list<string> entity);
        list<string> getEntity(T key);
        bool findEntity(T key);
        void imprimirEntity(list<string> entity);
        void generarDeArchivo(string file);
        void print();
        void printDot();
        void split(Nodo *padre, Nodo * hijo,int);
        int altura;
        string file;
        int size(){return siz;};
        int sizeNod(){return siz_nodo;};
        int root;
        virtual ~BT();
    protected:
    private:
        string name;
        fstream archivo;
        bool _find(T key,int&, vector<Nodo*>&);
        void delMem(vector<Nodo *>&);
        void actualizarArbol();
        bool _del(Nodo *,Nodo *, T);
        void giroNormal(Nodo *, Nodo *, Nodo *,bool,int);
        void merge(Nodo *, Nodo *,Nodo *,bool,int,int);
        void getArbol();
        int maxDegree;
        int siz;
        int siz_nodo;

};

template<typename T>
bool BT<T>::findEntity(T key){
    list<string> res = getEntity(key);
    if(res.empty()){
        cout<<"La clave no existe"<<endl;
        return false;
    }
    imprimirEntity(res);
    return true;
}

template<typename T>
void BT<T>::imprimirEntity(list<string> entity){
    for(string s : entity) cout<<s<<endl;
}


template<typename T>
list<string> BT<T>::getEntity(T key){
    list<string> res;
    int pos;
    vector<Nodo *> nodos;
    if(!_find(key,pos,nodos)) return res;
    ifstream archivo(file);
    if(archivo.fail()){
        cout<<"El archivo no se puede abrir"<<endl;
        return res;
    }
    archivo.seekg(nodos.back()->keys[pos]->posIndex);
    char caracter[300];
    archivo.getline(caracter,300);
    string en(caracter);
    string r;
    for(char c : en){
        if(c != ','){
            r.insert(r.end(),c);
        }
        else{
            res.push_back(r);
            r.clear();
        }
    }
    res.push_back(r);
    return res;
}

template<typename T>
void BT<T>::insert(T key, list<string> entity){
    ofstream archivo(file, ios::app);
    if(archivo.fail()){
        cout<<"El archivo no se puede abrir"<<endl;
        return;
    }
    int pos = archivo.tellp();
    if(find(key)) return;
    archivo<<key<<',';
    auto en = entity.end();
    en--;
    for(auto iter = entity.begin(); iter != en; ++iter){
        archivo<<*iter<<',';
    }
    archivo<<*en<<endl;
    insert(key,pos);
}

template<typename T>
void BT<T>::generarDeArchivo(string file){
    this->file = file;
    ifstream archivo(file);
    if(archivo.fail()){
        cout<<"El archivo no se puede abrir"<<endl;
        return;
    }
    int pos = archivo.tellg();
    char caracter[200];
    std::string::size_type sz;
    while(archivo.getline(caracter,200)){
        char c = '1';
        int i = 0;
        string key;
        while(i != 200){
            c = caracter[i];
            i++;
            if(c != ',') key.insert(key.end(),c);
            else break;
        }
        insert(stoi(key,&sz),pos);
        pos = archivo.tellg();
    }
    cout<<"Arbol levantado correctamente"<<endl;
    archivo.close();
}

template<typename T>
void BT<T>::delMem(vector<Nodo *>& v){
    for(Nodo * n : v){
        n->deleteNodo();
    }
    v.clear();
}

template<typename T>
void BT<T>::del(T key){
    int pos;
    vector<Nodo*> noditos;
    if(!_find(key,pos,noditos)) return;
    siz--;
    if(noditos.back()->soyHoja()){
        noditos.back()->keys.erase(noditos.back()->keys.begin() + pos);
        auto iter = noditos.end();
        iter--;
        for(iter; iter != noditos.begin();--iter){
            int a = (*iter)->keys.size();
            if((*iter)->estoyUnderFlow()){
                if(_del(*(iter - 1),*iter,key))return;
            }
            else{
                writeDisk(*(*iter));
                return;
            }
        }
    }
    else{
        int posActual = noditos.size() - 1;
        Nodo nodito = readDisk(noditos.back()->keys[pos]->nodoAnterior);
        noditos.push_back(new Nodo(nodito.index,nodito.keys,nodito.maxDegree, nodito.flag));
        while(!nodito.soyHoja()){
            nodito = readDisk(nodito.keys.back()->nodoSiguiente);
            noditos.push_back(new Nodo(nodito.index,nodito.keys,nodito.maxDegree, nodito.flag));
        }
        T temp = noditos.back()->keys.back()->key;
        noditos[posActual]->keys[pos]->key = temp;
        writeDisk(*(noditos[posActual]));
        noditos.back()->keys.erase(noditos.back()->keys.end() - 1);
        auto iter = noditos.end();
        iter--;
        for(iter; iter != noditos.begin(); --iter){
            if((*iter)->estoyUnderFlow()){
                if(_del(*(iter - 1),*iter,temp))return;
            }
            else{
                writeDisk(*(*iter));
                return;
            }
        }
    }
    actualizarArbol();
    delMem(noditos);
}

template<typename T>
void BT<T>::merge(Nodo * padre, Nodo * hijo, Nodo * hermano, bool ladoH,int TH, int pos){
    if(ladoH == IZQUIERDO){
        if(TH != NO_TIENE_H_DERECHO){
            padre->keys[pos + 1]->nodoAnterior = padre->keys[pos]->nodoAnterior;
        }
        hermano->keys.push_back(new Key(padre->keys[pos]->key,hermano->keys[hermano->keys.size() - 1]->nodoSiguiente,hijo->keys[0]->nodoAnterior,padre->keys[pos]->posIndex));
        hermano->keys.insert(hermano->keys.end(),hijo->keys.begin(),hijo->keys.end());
    }
    else{
        hermano->keys.insert(hermano->keys.begin(),new Key(padre->keys[pos]->key,hijo->keys[hijo->keys.size() - 1]->nodoSiguiente,hermano->keys[0]->nodoAnterior,padre->keys[pos]->key));
        hermano->keys.insert(hermano->keys.begin(),hijo->keys.begin(),hijo->keys.end());
    }
    writeDisk(*hermano);
    //deleteDisk(hijo->name);
    padre->keys.erase(padre->keys.begin() + pos);
    if(padre->keys.empty() and padre->index == root){
        root = hermano->index;
        //deleteDisk(padre->name);
    }
    else writeDisk(*padre);

}

template<typename T>
void BT<T>::deleteDisk(int key){
    string comando = "rm " + name + to_string(key) + ".dat";
    system(comando.c_str());
}

template<typename T>
bool BT<T>::Nodo::puedoPrestar(){
    if(keys.size() > maxDegree / 2)return true;
    return false;
}

template<typename T>
bool BT<T>::_del(Nodo * padre, Nodo * hijo, T key){
    int pos;
    padre->findInNodo(key,pos);
    int flag = -1;
    if(pos == 0) flag = NO_TIENE_H_IZQUIERDO;
    else if(pos == padre->keys.size()){
        pos--;
        flag = NO_TIENE_H_DERECHO;
    }
    Nodo der;
    Nodo izq;
    if(flag == NO_TIENE_H_IZQUIERDO){
        der = readDisk(padre->keys[pos]->nodoSiguiente);
        if(der.puedoPrestar()){
            giroNormal(padre,hijo,&der,DERECHO,pos);
            return true;
        }
        merge(padre,hijo,&der,DERECHO,flag,pos);
    }
    else if(flag == NO_TIENE_H_DERECHO){
        izq = readDisk(padre->keys[pos]->nodoAnterior);
        if(izq.puedoPrestar()){
            giroNormal(padre,hijo,&izq,IZQUIERDO,pos);
            return true;
        }
        merge(padre,hijo,&izq,IZQUIERDO,flag,pos);
    }
    else{
        der = readDisk(padre->keys[pos]->nodoSiguiente);
        if(der.puedoPrestar()){
            giroNormal(padre,hijo,&der,DERECHO,pos);
            return true;
        }
        pos--;
        izq = readDisk(padre->keys[pos]->nodoAnterior);
        if(izq.puedoPrestar()){
            giroNormal(padre,hijo,&izq,IZQUIERDO,pos);
            return true;
        }
        merge(padre,hijo,&izq,IZQUIERDO,flag,pos);
    }
    return false;
}

template<typename T>
void BT<T>::printDot(){
    string temp = name;
    auto iter = temp.end();
    iter--;
    temp.erase(iter);
    string file = name + temp + ".dot";
    ofstream archivo(file);
    archivo<<"digraph{graph [];node [fontsize = \"16\"shape = \"record\"];edge [];"<<endl;
    queue<int> noditos;
    if(root != -1)noditos.push(root);
    while(!noditos.empty()){
        Nodo nodito = readDisk(noditos.front());
        noditos.pop();
        archivo<<"nodo"<<to_string(nodito.index)<<" [label = \"<c0> ";
        int c = 1;
        for(Key * key : nodito.keys){
            archivo<<"| <c"<<to_string(c)<<"> "<<key->key<<" | <c"<<to_string(c+1)<<"> ";
            c += 2;
        }
        archivo<<"\"];"<<endl;
        bool flag = true;
        c = 2;
        for(Key * key : nodito.keys){
            if(flag){
                if(key->nodoAnterior != -1){
                    archivo<<"nodo"<<to_string(nodito.index)<<":c0 -> nodo"<<to_string(key->nodoAnterior)<<";"<<endl;
                    noditos.push(key->nodoAnterior);
                }
                flag = false;
            }
            if(key->nodoAnterior != -1){
                archivo<<"nodo"<<to_string(nodito.index)<<":c"<<to_string(c)<<" -> nodo"<<to_string(key->nodoSiguiente)<<";"<<endl;
                noditos.push(key->nodoSiguiente);
            }
            c += 2;
        }
    }
    archivo<<"}";
    archivo.close();
    string comando = "dot -Tpdf " + file + " -o " + name + temp + ".pdf";
    system(comando.c_str());
    string comando2 = "okular " + name + temp + ".pdf";
    system(comando2.c_str());
}



template<typename T>
void BT<T>::giroNormal(Nodo * padre, Nodo * hijo, Nodo * hermano, bool ladoH, int pos){
    if(ladoH == IZQUIERDO){
        hijo->keys.insert(hijo->keys.begin(),new Key(padre->keys[pos]->key,hermano->keys[hermano->keys.size() - 1]->nodoSiguiente,hijo->keys[0]->nodoAnterior,padre->keys[pos]->posIndex));
        padre->keys[pos]->key = hermano->keys[hermano->keys.size() - 1]->key;
        hermano->keys.pop_back();
    }
    else{
        hijo->keys.push_back(new Key(padre->keys[pos]->key,hijo->keys[hijo->keys.size() - 1]->nodoSiguiente,hermano->keys[0]->nodoAnterior,padre->keys[pos]->posIndex));
        padre->keys[pos]->key = hermano->keys[0]->key;
        hermano->keys.erase(hermano->keys.begin());
    }
    writeDisk(*hijo);
    writeDisk(*hermano);
    writeDisk(*padre);
}

template<typename T>
bool BT<T>::Nodo::estoyUnderFlow(){
    if(keys.size() < maxDegree / 2)return true;
    return false;
}

template<typename T>
bool BT<T>::Nodo::soyHoja(){
    if(keys.empty() or keys[0]->nodoAnterior == -1) return true;
    return false;
}

template<typename T>
void BT<T>::eliminarArbol(){
    string comando = "rm -r " + name;
    system(comando.c_str());
}

template<typename T>
void BT<T>::getArbol(){
    archivo.seekg(0, ios::beg);
    archivo.read(reinterpret_cast<char*>(&root),sizeof(int));
    archivo.read(reinterpret_cast<char *>(&siz),sizeof(int));
    archivo.read(reinterpret_cast<char *>(&siz_nodo),sizeof(int));
    archivo.read(reinterpret_cast<char *>(&altura),sizeof(int));
    archivo.read(reinterpret_cast<char *>(&maxDegree), sizeof(int));
}

template<typename T>
void BT<T>::actualizarArbol(){
    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<char *>(&root),sizeof(int));
    archivo.write(reinterpret_cast<char *>(&siz),sizeof(int));
    archivo.write(reinterpret_cast<char *>(&siz_nodo),sizeof(int));
    archivo.write(reinterpret_cast<char *>(&altura),sizeof(int));
    archivo.write(reinterpret_cast<char *>(&maxDegree), sizeof(int));
}

template<typename T>
void BT<T>::print(){
    if(root == -1)return;
    queue<int> nodos;
    nodos.push(root);
    while(!nodos.empty()){
        int actual = nodos.front();
        nodos.pop();
        Nodo nodito = readDisk(actual);
        int s = 0;
        for(Key * key : nodito.keys){
            cout<<key->key<<" ";
            if(s == 0){
                if(key->nodoAnterior != - 1) nodos.push(key->nodoAnterior);
                if(key->nodoSiguiente != - 1) nodos.push(key->nodoSiguiente);
            }
            else{
                if(key->nodoSiguiente != -1) nodos.push(key->nodoSiguiente);
            }
            s++;
        }
        cout<<endl;
    }
}

template<typename T>
void BT<T>::insert(T key, int pos){
    int actual;
    vector<Nodo *> noditos;
    if(_find(key,actual,noditos))return;
    siz++;
    auto iter = noditos.end();
    iter--;
    (*iter)->keys.insert((*iter)->keys.begin() + actual, new Key(key,-1,-1,pos));
    for(iter; iter != noditos.begin(); --iter){
        if((*iter)->estoyLleno()){
            split(*(iter - 1),*iter,(maxDegree) / 2);
        }
        else{
            (*iter)->index = writeDisk(*(*iter));
            return;
        }
    }
    if((*iter)->estoyLleno()){
        split(nullptr,*iter,(maxDegree) / 2);
    }
    else{
        (*iter)->index = writeDisk(*(*iter));
    }
    actualizarArbol();
    //delMem(noditos);
}

template<typename T>
bool BT<T>::Nodo::estoyLleno(){
    int a = maxDegree;
    int b = keys.size();
    if(maxDegree < keys.size())return true;
    return false;
}

template<typename T>
bool BT<T>::find(T key){
    if(siz == 0)return false;
    int actual;
    vector<Nodo*> noditos;
    if(_find(key,actual,noditos)) return true;
    return false;
}

template<typename T>
bool BT<T>::_find(T key, int& actual, vector<Nodo*> & noditos){
    Nodo nodito = readDisk(root);
    for(int i = 0; i < altura; i++){
        noditos.push_back(new Nodo(nodito.index,nodito.keys,nodito.maxDegree,nodito.flag));
        if(nodito.findInNodo(key,actual)) return true;
        if(actual == nodito.keys.size()) actual--;
        if(key < nodito.keys[actual]->key) nodito = readDisk(nodito.keys[actual]->nodoAnterior);
        else nodito = readDisk(nodito.keys[actual]->nodoSiguiente);
    }
    noditos.push_back(new Nodo(nodito.index,nodito.keys,nodito.maxDegree,nodito.flag));
    if(nodito.findInNodo(key,actual))return true;
    return false;
}

template<typename T>
bool BT<T>::Nodo::findInNodo(T key, int &actual){
    int limInferior = 0;
    int limSuperior = keys.size() - 1;
    actual = 0;
    if(limSuperior == -1)return false;
    int sum = (limSuperior - limInferior) / 2;
    while(sum){
        actual = limInferior + sum;
        if(key == keys[actual]->key) return true;
        if(key > keys[actual]->key) limInferior = actual;
        else limSuperior = actual;
        sum = (limSuperior - limInferior) / 2;
    }
    if(keys[limInferior]->key == key){
        actual = limInferior;
        return true;
    }
    else if(keys[limSuperior]->key == key){
        actual = limSuperior;
        return true;
    }
    if(key < keys[limInferior]->key) actual = limInferior;
    else if(key > keys[limSuperior]->key) actual = limSuperior + 1;
    else if(key > keys[limInferior]->key and key < keys[limSuperior]->key) actual = limSuperior;
    return false;
}

template<typename T>
void BT<T>::split(Nodo * padre, Nodo * hijo, int med){
    auto iter = hijo->keys.begin() + med;
    //vector<Key *> first;
    //vector<Key *> second;
    /*
    for(int i = 0; i < med; i++){
        first.push_back(*iter);
        iter++;
    }
    auto iter3 = iter;
    iter3++;
    for(iter3; iter3 != hijo->keys.end();++iter3){
        second.push_back(*iter3);
    }
    */
    Nodo * nuevo = new Nodo(maxDegree);
    nuevo->keys = vector<Key *>(hijo->keys.size() - (med + 1));
    move(iter + 1,hijo->keys.end(),nuevo->keys.begin());
    //cout<<"ITER->"<<(*iter)->key<<endl;
    siz_nodo++;
    int tempIndex = writeDisk(*nuevo);
    nuevo->index = tempIndex;
    if(padre){
        int flag = -1;
        int pos;
        padre->findInNodo((*iter)->key,pos);
        auto iter2 = padre->keys.begin() + pos;
        if(iter2 == padre->keys.begin()) flag = 1;
        else if(iter2 == padre->keys.end()) flag = 0;
        padre->keys.insert(iter2,*iter);
        padre->findInNodo((*iter)->key,pos);
        iter2 = padre->keys.begin() + pos;
        (*iter2)->nodoAnterior = hijo->index;
        (*iter2)->nodoSiguiente = nuevo->index;
        iter2++;
        if(flag == 1){
            (*iter2)->nodoAnterior = nuevo->index;
        }
        else if(flag == 0){
            iter2--;
            iter2--;
            (*iter2)->nodoSiguiente = hijo->index;
        }
        else{
            (*iter2)->nodoAnterior = nuevo->index;
            iter2--;
            iter2--;
            (*iter2)->nodoSiguiente = hijo->index;
        }
    }
    else{
        Nodo * nuevoRoot = new Nodo(maxDegree);
        siz_nodo++;
        (*iter)->nodoAnterior = hijo->index;
        (*iter)->nodoSiguiente = nuevo->index;
        nuevoRoot->keys.push_back(*iter);
        root = writeDisk(*nuevoRoot);
        altura++;
        nuevoRoot->deleteNodo();
    }
    hijo->keys.erase(iter,hijo->keys.end());
    //hijo->erase(med);
    writeDisk(*hijo);
    //delete(hijo);
    nuevo->deleteNodo();
}

template<typename T>
void BT<T>::Nodo::erase(int med){
    auto iter = keys.begin() + med;
    auto temp = iter;
    while(iter != keys.end()){
        delete(*iter);
        iter++;
    }
    keys.erase(temp,keys.end());
}

template<typename T>
int BT<T>::writeDisk(Nodo& nod){
    int index = 0;
    if(!nod.flag){
        archivo.seekp(0,ios::end);
        index = archivo.tellp();
        bool f = true;
        archivo.write(reinterpret_cast<char *>(&index), sizeof(int));
        archivo.write(reinterpret_cast<char *>(&f), sizeof(bool));
    }
    else{
        index = nod.index;
        archivo.seekp(nod.index, ios::beg);
        archivo.write(reinterpret_cast<char *>(&nod.index), sizeof(int));
        archivo.write(reinterpret_cast<char *>(&nod.flag), sizeof(bool));
    }
    int s = nod.keys.size();
    archivo.write(reinterpret_cast<char *>(&s),sizeof(int));
    for(Key * k : nod.keys){
        archivo.write(reinterpret_cast<char *>(k),sizeof(Key));
    }
    for(int i = maxDegree - s; i > 0; i--){
        Key vacio;
        archivo.write(reinterpret_cast<char *>(&vacio), sizeof(Key));
    }
    return index;
}

template<typename T>
typename BT<T>::Nodo BT<T>::readDisk(int index){
    Nodo res;
    int n;
    bool f;
    int k;
    archivo.seekg(index, ios::beg);
    archivo.read(reinterpret_cast<char *>(&n),sizeof(int));
    res.index = n;
    archivo.read(reinterpret_cast<char *>(&f),sizeof(bool));
    res.flag = f;
    archivo.read(reinterpret_cast<char *>(&k),sizeof(int));
    for(int i = 0; i < k; i++){
        Key key;
        archivo.read(reinterpret_cast<char *>(&key),sizeof(Key));
        res.keys.push_back(new Key(key.key, key.nodoAnterior, key.nodoSiguiente,key.posIndex));
    }
    res.maxDegree = maxDegree;
    return res;
}

template<typename T>
BT<T>::Nodo::Nodo(int index,vector<Key *>& key,int maxD, bool flag){
    this->keys = key;
    this->maxDegree = maxD;
    this->flag = flag;
    this->index = index;
}

template<typename T>
BT<T>::Nodo::Nodo(int maxD){
    this->maxDegree = maxD;
    this->index = -1;
    this->flag = false;
}

template<typename T>
BT<T>::Nodo::Nodo(){
    this->index = -1;
    flag = false;
}

template<typename T>
BT<T>::Key::Key(T key, int anterior, int siguiente, int pos){
    this->key = key;
    nodoAnterior = anterior;
    nodoSiguiente = siguiente;
    posIndex = pos;
}

template<typename T>
BT<T>::Key::Key(){
    nodoAnterior = -1;
    nodoSiguiente = -1;
    posIndex = 0;
}

template<typename T>
BT<T>::~BT(){
    archivo.close();
}

template<typename T>
BT<T>::BT(string name, int maxD,bool leer){
    string file = name + "/";
    this->name = file;
    if(!leer){
        string com = "rm -R " + file;
        system(com.c_str());
        string comando = "mkdir " + this->name;
        system(comando.c_str());
    }
    string file2 = file + name + ".dat";
    string comando2 = "touch " + file2;
    system(comando2.c_str());
    archivo.open(file2, ios::out | ios::in | ios::binary);
    maxDegree = maxD;
    siz = 0;
    siz_nodo = 1;
    altura = 0;
    if(!leer){
        actualizarArbol();
        root = archivo.tellg();
        Nodo r(maxD);
        actualizarArbol();
        writeDisk(r);
    }
    else{
        getArbol();
    }
}

template<typename T>
void BT<T>::Nodo::deleteNodo(){
    for(Key * key : keys){
        delete(key);
    }
    keys.clear();
    delete this;
}

template<typename T>
BT<T>::BT(){
    name = "BTDefault/";
    file = "BTDefault.dat";
    string comando = "mkdir " + name;
    system(comando.c_str());
    system("touch BTDefault/BTDefault.dat");
    archivo.open("BTDefault/BTDefault.dat", ios::out | ios::in | ios::binary);
    maxDegree = 0;
    siz = 0;
    siz_nodo = 1;
    root = 0;
    Nodo r(0,0);
    writeDisk(r);
    actualizarArbol();
    altura = 0;
}

#endif // BT_H
