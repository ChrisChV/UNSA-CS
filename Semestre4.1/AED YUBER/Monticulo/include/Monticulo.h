#ifndef MONTICULO_H
#define MONTICULO_H
#include <vector>
#include <iostream>
using namespace std;

class Monticulo
{
    public:
        Monticulo();
        virtual ~Monticulo();
        void ingresar(int);
        void print();
        int size();
        int del();
    protected:
    private:
        vector<int> monti;
};

Monticulo::Monticulo(){}
Monticulo::~Monticulo(){}

int Monticulo::size(){
    return monti.size();
}

int Monticulo::del(){
    int resultado = monti.front();
    if(monti.size() == 0)return resultado;
    monti[0] = monti[monti.size() - 1];
    monti.pop_back();
    int pos = 0;
    while(pos <= monti.size() - 1){
        if(2 * pos + 1 > monti.size() - 1)return resultado;
        if(2 * pos + 2 > monti.size() - 1){
            if(monti[pos] > monti[2 * pos + 2]){
                auto temp = monti[pos];
                monti[pos] = monti[2 * pos + 2];
                monti[2 * pos + 2] = temp;
                return resultado;
            }
        }
        if(monti[pos] > monti[2 * pos + 1] or monti[pos] > monti[2 * pos + 2]){
            if(monti[2 * pos + 1] < monti[2 * pos + 2]){
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

void Monticulo::print(){
    for(int i = 0; i < monti.size(); i++){
        cout<<monti[i]<<endl;
    }
}

void Monticulo::ingresar(int valor){
    monti.push_back(valor);
    int pos = monti.size() - 1;
    while(pos > 0){
        if(monti[pos] < monti[(pos - 1) / 2]){
            auto temp = monti[pos];
            monti[pos] = monti[(pos - 1) / 2];
            monti[(pos - 1) / 2] = temp;
        }
        pos = (pos - 1) / 2;
    }
}

#endif // MONTICULO_H
