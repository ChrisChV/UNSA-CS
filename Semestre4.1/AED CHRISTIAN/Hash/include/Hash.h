#ifndef HASH_H
#define HASH_H
#include "list"
#include "algorithm"

using namespace std;

template <typename T, typename K>
class Hash
{
    public:
        Hash(){
            tam  = 11;
            tabla = new list<T>[tam];
        }
        Hash(int(*fun)(K), int siz){
            tam = siz;
            tabla = new list<T>[tam];
            fd = fun;
        }
        void add(T,K);
        bool _find(K,T,T&);
        virtual ~Hash(){

        }
    protected:
    private:
        list<T> *tabla;
        int tam;
        int (*fd)(K);
};

template <typename T, typename K>
void Hash<T,K>::add(T objeto, K key){
    int p = fd(key) % tam;
    tabla[p].push_back(objeto);
}

template <typename T, typename K>
bool Hash<T,K>::_find(K key, T objeto, T &resultado){
    int p = fd(key) % tam;
    auto result = find(tabla[p].begin(), tabla[p].end(), objeto);
    if(result != tabla[p].end()){
        resultado = *result;
        return true;
    }
    return false;
}




#endif // HASH_H
