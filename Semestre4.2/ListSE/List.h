#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <cstring>
#include "Nodo.h"

template<class T>
class List{
    private:
        Nodo<T> *head;
        Nodo<T> *last;
        void reverse();
        void reversePrint();
    public:
        List();
        void pushFront(T);
        void pushBack(T);
        void reversePrint(Nodo<T> *);
        bool find(T);
        T* findElem(char *, T *);
        void print();
        void printStruct();
        int size();
        void insert(T);
        T maxValue();
        void reverse(Nodo<T> *);
        List<T>* unionList(List<T> &);
        List<T>* unionListSort(List<T> &);
        List<T>* intersectionList(List<T> &);
        List<T>* intersectionListSort(List<T> &);
        bool find(T, Nodo<T> **&);
        bool add(T);
        bool erase(T);
};

template<class T>
List<T> :: List(){
    head = 0;
    last = 0;
}

template<class T>
void List<T> :: pushFront(T d){
    Nodo<T> *temp = new Nodo<T>(d);
    if(head){
        temp->sig = head;
    }else{
        last = temp;
    }
    head = temp;
}

template<class T>
void List<T> :: pushBack(T d){
    Nodo<T> *temp = new Nodo<T>(d);
    if(head){
        last->sig = temp;
    }else{
        head = temp;
    }
    last = temp;
}

template<class T>
void List<T> :: reversePrint(){
    if(head){
        reversePrint(head);
    }
}

template<class T>
void List<T> :: reversePrint(Nodo<T> *n){
    if(n != last){
        reversePrint(n->sig);
    }
    std::cout << n->dato << " ";
}

template<class T>
bool List<T> :: find(T d){
    Nodo<T> *temp = head;
    while(temp){
        if(temp->dato == d){
            return true;
        }
        temp = temp->sig;
    }
    return false;
}

template<class T>
T* List<T> :: findElem(char *d, T *r){
    Nodo<T> *temp = head;
    while(temp){
        if(!strcmp((temp->dato).key, d)){ /** comparamos el key con el dato ingresado **/
            /*std::cout << (temp->dato).key << std::endl;*/
            return &(temp->dato);
        }
        temp = temp->sig;
    }
    return 0;
}

template<class T>
void List<T> :: print(){
    Nodo<T> *temp = head;
    while(temp){
        std::cout << temp->dato << " ";
        temp = temp->sig;
    }
}

template<class T>
void List<T> :: printStruct(){
    Nodo<T> *temp = head;
    while(temp){
        std::cout << (temp->dato).key << " ";
        temp = temp->sig;
    }
}

template<class T>
int List<T> :: size(){
    int c = 0;
    Nodo<T> *temp = head;
    while(temp){
        c++;
        temp = temp->sig;
    }
    return c;
}

template<class T>
T List<T> :: maxValue(){
    T max;
    if(head){
        max = head->dato;
        Nodo<T> *temp = head->sig;
        while(temp){
            if(max < temp->dato){
                max = temp->dato;
            }
            temp = temp->sig;
        }
    }
    return max;
}

template<class T>
void List<T> :: insert(T d){
    Nodo<T> *val = new Nodo<T>(d);
    if(head){
        if(d < head->dato){
            val->sig = head;
            head = val;
        }else if(d > last->dato){
            last->sig = val;
            last = val;
        }else{
            Nodo<T> *temp = head;
            while(d > temp->sig->dato){
                temp = temp->sig;
            }
            val->sig = temp->sig;
            temp->sig = val;
        }
    }else{
        head = val;
        last = val;
    }
}

template<class T>
void List<T> :: reverse(){
    if(head->sig){
        Nodo<T> *temp = head;
        reverse(temp);
        temp->sig = 0;
        last = temp;
    }
}

template<class T>
void List<T> :: reverse(Nodo<T> *n){
    Nodo<T> *temp = n->sig;
    if(temp != last){
        reverse(temp);
    }else{
        head = temp;
    }
    temp->sig = n;
}

template<class T>
List<T>* List<T> :: unionList(List<T> &l){
    List<T> *lista = new List<T>();
    Nodo<T> *temp = head;
    while(temp){
        lista->insertSort(temp->dato);
        temp = temp->sig;
    }
    temp = l.head;
    while(temp){
        if(!find(temp->dato)){
            lista->insertSort(temp->dato);
        }
        temp = temp->sig;
    }
    return lista;
}

template<class T>
List<T>* List<T> :: unionListSort(List<T> &l){
    List<T> *lista = new List<T>();
    Nodo<T> *s1 = head, *s2 = l.head;
    while(s1 or s2){
        if((s1 and s2) and s1->dato == s2->dato){
            lista->pushBack(s1->dato);
            s1 = s1->sig; s2 = s2->sig;
        }else if(s2 == 0 or (s1 and s1->dato < s2->dato)){
            lista->pushBack(s1->dato);
            s1 = s1->sig;
        }else if(s2){
            lista->pushBack(s2->dato);
            s2 = s2->sig;
        }
    }
    return lista;
}

template<class T>
List<T>* List<T> :: intersectionList(List<T> &l){
    List<T> *lista = new List<T>();
    Nodo<T> *temp = head;
    while(temp){
        if(l.find(temp->dato)){
            lista->insert(temp->dato);
        }
        temp = temp->sig;
    }
    return lista;
}

template<class T>
List<T>* List<T> :: intersectionListSort(List<T> &l){
    List<T> *lista = new List<T>();
    Nodo<T> *s1 = head, *s2 = l.head;
    while(s1 and s2){
        if(s1->dato == s2->dato){
            lista->pushBack(s1->dato);
            s1 = s1->sig; s2 = s2->sig;
        }else if(s1->dato < s2->dato){
            s1 = s1->sig;
        }else{
            s2 = s2->sig;
        }
    }
    return lista;
}

template<class T>
bool List<T> :: find(T d, Nodo<T> **&p){
    p = &head;
    while(*p){
        if((*p)->dato == d){
            return true;
        }
        if((*p)->dato > d){
            return false;
        }
        p = &((*p)->sig);
    }
    return false;
}

template<class T>
bool List<T> :: add(T d){
    Nodo<T> **q;
    if(find(d, q)){
        return false;
    }
    Nodo<T> *pNuevo = new Nodo<T>(d);
    pNuevo->sig = *q;
    *q = pNuevo;
    return true;
}

template<class T>
bool List<T> :: erase(T d){
    Nodo<T> **q;
    if(!find(d, q)){
        return false;
    }
    Nodo<T> *pDel = *q;
    *q = (*q)->sig;
    delete pDel;
    return true;
}

#endif // LIST_H
