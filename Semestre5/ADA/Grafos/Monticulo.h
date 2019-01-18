#ifndef MONTICULO_H
#define MONTICULO_H
#include <vector>
#include <iostream>
using namespace std;


template<typename T>
class Monticulo
{
    public:
        Monticulo();
        virtual ~Monticulo();
        void push(int);
        void print();
        int size();
        int pop();
    protected:
    private:
        vector<T> monti;
};


template<typename T>
Monticulo<T>::Monticulo(){}

template<typename T>
Monticulo<T>::~Monticulo(){}

template<typename T>
int Monticulo<T>::size(){
    return monti.size();
}

template<typename T>
int Monticulo<T>::pop(){
    int resultado = monti.front();
    if(monti.size() == 0)return resultado;
    monti[0] = monti[monti.size() - 1];
    monti.pop_back();
    int pos = 0;
    while(pos <= monti.size() - 1){
        if(2 * pos + 1 < monti.size() - 1)return resultado;
        if(2 * pos + 2 < monti.size() - 1){
            if(monti[pos] < monti[2 * pos + 2]){
                auto temp = monti[pos];
                monti[pos] = monti[2 * pos + 2];
                monti[2 * pos + 2] = temp;
                return resultado;
            }
        }
        if(monti[pos] < monti[2 * pos + 1] or monti[pos] < monti[2 * pos + 2]){
            if(monti[2 * pos + 1] > monti[2 * pos + 2]){
                auto temp = monti[pos];
                monti[pos] = monti[2 * pos + 1];
                monti[2 * pos + 1] = temp;
                pos = 2 * pos + 1;
            }
            else{
                auto temp = monti[pos];
                monti[pos] = monti[2 * pos + 2];
                monti[2 * pos + 2] = temp;
                pos = 2 * pos + 2;
            }
        }
        else{
            break;
        }
    }
    return resultado;
}

template<typename T>
void Monticulo<T>::print(){
    for(int i = 0; i < monti.size(); i++){
        cout<<monti[i]<<endl;
    }
}

template<typename T>
void Monticulo<T>::push(int valor){
    monti.push_back(valor);
    int pos = monti.size() - 1;
    while(pos > 0){
        if(monti[pos] > monti[(pos - 1) / 2]){
            auto temp = monti[pos];
            monti[pos] = monti[(pos - 1) / 2];
            monti[(pos - 1) / 2] = temp;
        }
        pos = (pos - 1) / 2;
    }
}

#endif // MONTICULO_H
