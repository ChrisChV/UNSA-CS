#ifndef REDBLACK_H
#define REDBLACK_H
#include "iostream"
#include "fstream"
#include "list"

using namespace std;

enum Colores{NEGRO, ROJO, DOBLENEGRO};
enum Rotaciones{IZQUIERDA, DERECHA};
template <typename T>
class RedBlack
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T);
                T valor;
                int color;
                Nodo * hijos[2];
                Nodo * padre;
                int size;
                void destruirme();
        };
        RedBlack();
        void rotacionSImple(Nodo *&, bool);
        void print();
        void del(T);
        void insert(T);
        void rotacionCompleja(Nodo *&, bool);
        T select(int);
        int rank(T);
        virtual ~RedBlack();
    protected:
    private:
        Nodo * root;
        bool _maxIzquierda(Nodo **&);
        int find(T, Nodo **&, Nodo *&);
        bool tipoHijo(Nodo *);
        Nodo * _select(Nodo *, int);
};

template <typename T>
int RedBlack<T>::rank(T k){
	Nodo ** nodo;
	Nodo * temp;
	if(!find(k,nodo,temp)) return -1000;
	Nodo * y = *nodo;
	int r = 1; 
	if(y->hijos[IZQUIERDA]) r = y->hijos[IZQUIERDA]->size + 1;
	while(y != root){
		if(y == y->padre->hijos[DERECHA]){
			int a = 0;
			if(y->padre->hijos[IZQUIERDA]) a = y->padre->hijos[IZQUIERDA]->size;
			r += a + 1;
		}
		y = y->padre;
	}
	return r;
}

template <typename T>
T RedBlack<T>::select(int i){
	Nodo * temp =  _select(root,i);
	return temp->valor;
}

template <typename T>
typename RedBlack<T>::Nodo * RedBlack<T>::_select(Nodo * nodo, int i){
	int r = 1;
	if(nodo->hijos[IZQUIERDA]) r = nodo->hijos[IZQUIERDA]->size + 1;
	if(i == r) return nodo;
	else if(i < r) return _select(nodo->hijos[IZQUIERDA],i);
	return _select(nodo->hijos[DERECHA],i-r);
}

template <typename T>
bool RedBlack<T>::_maxIzquierda(Nodo **& nodo){
    nodo = &((*nodo)->hijos[0]);
    int a = 1;
    while(a){
        if(!(*nodo)->hijos[1])break;
        nodo = &((*nodo)->hijos[1]);
        a++;
    }
    if(a == 1){
        return false;
    }
    return true;
}

template <typename T>
void RedBlack<T>::del(T valor){
    Nodo ** nodo;
    Nodo * tt;
    bool f = false;
    int val = find(valor, nodo, tt);
    if(val == 0 or val == -1)return;
    if((*nodo)->hijos[0] and (*nodo)->hijos[1]){
        Nodo ** nodoTemp;
        nodoTemp = nodo;
        if(!_maxIzquierda(nodoTemp)){
            f = true;
        }
        swap((*nodo)->valor, (*nodoTemp)->valor);
        nodo = nodoTemp;
    }
    cout<<"HHNODO->"<<(*nodo)->valor<<endl;
    cout<<"HHPADRE->"<<(*nodo)->padre->valor<<endl;
    if((*nodo)->hijos[0]){
        cout<<"HHHIJO IZQ->"<<(*nodo)->hijos[0]->valor<<endl;
        auto temp = *nodo;
        *nodo = (*nodo)->hijos[0];
        (*nodo)->padre = temp->padre;
        (*nodo)->color = NEGRO;
        ///delete temp;
    }
    else if((*nodo)->hijos[1]){
        auto temp = *nodo;
        *nodo = (*nodo)->hijos[1];
        (*nodo)->padre = temp->padre;
        (*nodo)->color = NEGRO;
        ///delete temp;
    }
    else{
        if((*nodo)->color == ROJO){
            (*nodo)->padre->hijos[(*nodo)->padre->valor < (*nodo)->valor] = nullptr;
            ///delete *nodo;
        }
        else{
            Nodo *temp = (*nodo);
            bool flagg;
            bool flag = (*nodo)->padre->valor > (*nodo)->valor;
            if(f) flag = !flag;
            flagg = flag;
            while(true){
                cout<<"NODO->"<<(*nodo)->valor<<endl;
                cout<<"PADRE->"<<(*nodo)->padre->valor<<endl;
                if((*nodo) == root){
                    root->color = NEGRO;
                    break;
                }
                flag = (*nodo)->padre->valor > (*nodo)->valor;
                if(f) flag = !flag;
                Nodo * padre = (*nodo)->padre;
                if(padre->hijos[flag]->color == NEGRO){
                    if(!padre->hijos[flag]->hijos[flag] or padre->hijos[flag]->hijos[flag]->color == NEGRO){
                        if(!padre->hijos[flag]->hijos[!flag] or padre->hijos[flag]->hijos[!flag]->color == NEGRO){
                            padre->hijos[flag]->color = ROJO;
                            if(padre->color == NEGRO){
                                padre->color = NEGRO;
                                nodo = &(padre);
                            }
                            else{
                                padre->color = NEGRO;
                                break;
                            }
                        }
                        else{
                            padre->color = NEGRO;
                            padre->hijos[flag]->color = NEGRO;
                            padre->hijos[flag]->hijos[!flag]->color = NEGRO;
                            rotacionSImple(padre, flag);
                            padre->color = ROJO;
                            break;
                        }
                    }
                    else{
                        padre->hijos[flag]->color = NEGRO;
                        padre->hijos[flag]->hijos[flag]->color = NEGRO;
                        rotacionSImple(padre, flag);
                        padre->color = NEGRO;
                        //print();
                        break;
                    }
                }
                else{
                    rotacionSImple(padre,flag);
                    break;
                }
            }
            cout<<"GGGGGGGGGG->"<<temp->valor<<endl;
            cout<<"GGGGGGGGGG22->"<<temp->padre->valor<<endl;
            if(temp->padre->hijos[!flagg]){
                cout<<"dddddddddddddddd->"<<temp->padre->hijos[!flagg]->valor<<endl;;
            }
            temp->padre->hijos[!flagg] = nullptr;
            ///delete temp;
        }
    }
}

template <typename T>
void RedBlack<T>::print(){
    ofstream archivo("eje.dot");
    if(archivo.fail()){
        cout<<"No se puede abrir el archivo"<<endl;
        return;
    }
    archivo<<"digraph{"<<endl;
    list<Nodo *> result;
    if(root)result.push_back(root);
    while(!result.empty()){
        list<Nodo *> temp;
        for(auto iter = result.begin(); iter != result.end(); ++iter){
            string color;
            if((*iter)->color) color = "red";
            else color = "black";
            archivo<<(*iter)->valor<<" [color=\""<<color<<"\" label = \""<<(*iter)->valor<<" size="<<(*iter)->size<<"\"]"<<endl;
            if((*iter)->hijos[0]){
                archivo<<(*iter)->valor<<"->"<<(*iter)->hijos[0]->valor<<endl;
                temp.push_back((*iter)->hijos[0]);
            }
            if((*iter)->hijos[1]){
                archivo<<(*iter)->valor<<"->"<<(*iter)->hijos[1]->valor<<endl;
                temp.push_back((*iter)->hijos[1]);
            }
        }
        result = temp;
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf eje.dot -o eje.pdf");
}

template <typename T>
void RedBlack<T>::insert(T valor){
    Nodo ** nodo;
    Nodo *padre;
    auto flag = this->find(valor,nodo,padre);
    if(flag == 1)return;
    *nodo = new Nodo(valor);
    (*nodo)->padre = padre;
    Nodo * tt = padre;
    while(tt){
    	tt->size += 1;
    	tt = tt->padre;
    }
    if(flag == -1){
        root->color = NEGRO;
    }
    Nodo * iter = *nodo;
    while(iter->padre){
        if(iter->padre->color == NEGRO)break;
        if(iter == root)break;
        if(iter->padre->color == ROJO){
            if(!iter->padre->padre->hijos[iter->padre->valor < iter->padre->padre->valor] or iter->padre->padre->hijos[iter->padre->valor < iter->padre->padre->valor]->color == NEGRO){
                if(iter->valor > iter->padre->valor){
                    if(iter->padre->valor > iter->padre->padre->valor) rotacionSImple(iter->padre->padre, DERECHA);
                    else rotacionCompleja(iter->padre->padre, IZQUIERDA);
                }
                else{
                    if(iter->padre->valor > iter->padre->padre->valor) rotacionCompleja(iter->padre->padre, DERECHA);
                    else rotacionSImple(iter->padre->padre, IZQUIERDA);
                }
                break;
            }
            else{
                iter->padre->padre->color = ROJO;
                iter->padre->padre->hijos[0]->color = NEGRO;
                iter->padre->padre->hijos[1]->color = NEGRO;
                iter = iter->padre->padre;
            }
        }
    }
    if(root->color == ROJO) root->color = NEGRO;
}

template <typename T>
int RedBlack<T>::find(T valor, Nodo **&nodo, Nodo *&iter){
    iter = nullptr;
    nodo = &(root);
    if(!root)return -1;
    while(*nodo){
        if((*nodo)->valor == valor)return 1;
        if(!iter) iter = root;
        else iter = *nodo;
        nodo = &((*nodo)->hijos[(*nodo)->valor < valor]);
    }
    return 0;
}

template <typename T>
void RedBlack<T>::rotacionCompleja(Nodo *&padre, bool flag){
    Nodo * hijo = padre->hijos[flag];
    Nodo * nieto = hijo->hijos[!flag];
    Nodo * abuelo = padre->padre;
    nieto->color = NEGRO;
    padre->color = ROJO;
    padre->hijos[flag] = nieto->hijos[!flag];
    if(nieto->hijos[!flag])
        nieto->hijos[!flag]->padre = padre;
    hijo->hijos[!flag] = nieto->hijos[flag];
    if(nieto->hijos[flag])
        nieto->hijos[flag]->padre = hijo;
    nieto->hijos[!flag] = padre;
    padre->padre = nieto;
    nieto->hijos[flag] = hijo;
    int a = 0;
    int b = 0;
    if(hijo->hijos[0]) a = hijo->hijos[0]->size;
    if(hijo->hijos[1]) b = hijo->hijos[1]->size;
    hijo->size = a + b + 1;
    a = 0;
    b = 0;
    if(padre->hijos[0]) a = padre->hijos[0]->size;
    if(padre->hijos[1]) b = padre->hijos[1]->size;
    padre->size = a + b + 1;
    a = 0;
    b = 0;
    if(nieto->hijos[0]) a = nieto->hijos[0]->size;
    if(nieto->hijos[1]) b = nieto->hijos[1]->size;
    nieto->size = a + b + 1;
    hijo->padre = nieto;
    if(padre == root){
        nieto->padre = nullptr;
        root = nieto;
    }
    else{
        if(abuelo){
        	abuelo->hijos[abuelo->valor < padre->valor] = nieto;
        	a = 0;
        	b = 0;
        	if(abuelo->hijos[0]) a = abuelo->hijos[0]->size;
        	if(abuelo->hijos[1]) b = abuelo->hijos[1]->size;
        	abuelo->size = a + b + 1;
        } 
        nieto->padre = abuelo;
    }

}

template <typename T>
void RedBlack<T>::rotacionSImple(Nodo *& padre, bool flag){
    Nodo * hijo = padre->hijos[flag];
    Nodo * abuelo = padre->padre;
    padre->hijos[flag] = hijo->hijos[!flag];
    if(padre->hijos[flag]){
        padre->hijos[flag]->padre = padre;
    }
    hijo->hijos[!flag] = padre;
    padre->padre = hijo;
    padre->color = ROJO;
    hijo->color = NEGRO;
    int a = 0;
    int b = 0;
    if(padre->hijos[0]) a = padre->hijos[0]->size;
    if(padre->hijos[1]) b = padre->hijos[1]->size;
    padre->size = a + b + 1;
   	a = 0;
   	b = 0;
   	if(hijo->hijos[0]) a = hijo->hijos[0]->size;
   	if(hijo->hijos[1]) b = hijo->hijos[1]->size;
   	hijo->size = a + b + 1;
    if(padre == root){
        hijo->padre = nullptr;
        root = hijo;
    }
    else{
        if(abuelo){
        	abuelo->hijos[abuelo->valor < padre->valor] = hijo;
        	a = 0;
        	b = 0;
        	if(abuelo->hijos[0]) a = abuelo->hijos[0]->size;
        	if(abuelo->hijos[1]) b = abuelo->hijos[1]->size;
        	abuelo->size = a + b + 1;
        } 
        hijo->padre = abuelo;
    }
}

template <typename T>
RedBlack<T>::Nodo::Nodo(){
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    padre = nullptr;
    color = ROJO;
}

template <typename T>
RedBlack<T>::Nodo::Nodo(T valor){
    this->valor = valor;
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    padre = nullptr;
    color = ROJO;
    size = 1;
}

template <typename T>
void RedBlack<T>::Nodo::destruirme(){
    if(this->hijos[0])this->hijos[0]->destruirme();
    if(this->hijos[1])this->hijos[1]->destruirme();
    delete this;
}

template <typename T>
RedBlack<T>::RedBlack(){
    root = nullptr;
}

template <typename T>
RedBlack<T>::~RedBlack(){
    root->destruirme();
}


#endif // REDBLACK_H
