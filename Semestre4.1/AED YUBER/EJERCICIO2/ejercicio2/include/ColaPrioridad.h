#ifndef COLAPRIORIDAD_H
#define COLAPRIORIDAD_H
#include "vector"
#include <algorithm>

using namespace std;

class ColaPrioridad
{
    public:
        class Paciente{
            public:
                Paciente();
                Paciente(string, int);
                string nombrePaciente;
                int gravedad;
        };
        ColaPrioridad();
        int size();
        void print();
        virtual ~ColaPrioridad();
        void insertar(string, int);
        Paciente* pop();
        Paciente* front();
    protected:
    private:
        vector<Paciente *> cola;
};

ColaPrioridad::Paciente * ColaPrioridad::front(){
    return cola.front();
}

int ColaPrioridad::size(){
    return cola.size();
}
void ColaPrioridad::print(){
    for(int i = 0; i < cola.size(); i++){
        cout<<cola[i]->nombrePaciente<<"->";
    }
    cout<<endl;
}



ColaPrioridad::Paciente * ColaPrioridad::pop(){
    if(cola.empty())return nullptr;
    auto resultado = cola.front();
    int tam = cola.size() - 1;
    int pos = 0;
    cola[0] = cola[tam];
    cola.pop_back();
    tam--;
    while(pos <= tam){
        if(2 * pos + 1 > tam)return resultado;
        if(2 * pos + 2 > tam){
            if(cola[pos]->gravedad < cola[2 * pos + 2]->gravedad){
                swap(cola[pos]->gravedad,cola[2 * pos + 2]->gravedad);
                return resultado;
            }
        }
        if(cola[pos]->gravedad < cola[2 * pos + 1]->gravedad or cola[pos]->gravedad < cola[2 * pos + 2]->gravedad){
            if(cola[2 * pos + 1]->gravedad > cola[2 * pos + 2]->gravedad){
                swap(cola[pos],cola[2 * pos +1]);
                pos = 2 * pos + 1;
            }
            else{
                swap(cola[pos],cola[2 * pos + 2]);
                pos = 2 * pos + 2;
            }
        }
        else{
            break;
        }
    }
    return resultado;
}

void ColaPrioridad::insertar(string nombre, int gravedad){
    if(cola.size() == 20){
        cout<<"No se pueden ingresar mas de 20 pasientes"<<endl;
        return;
    }
    cola.push_back(new Paciente(nombre,gravedad));
    int pos = cola.size() - 1;
    while(pos > 0){
        if(cola[pos]->gravedad > cola[(pos - 1) / 2]->gravedad){
            swap(cola[pos], cola[(pos - 1) / 2]);
        }
        pos = (pos - 1) / 2;
    }
}

ColaPrioridad::Paciente::Paciente(string nombre, int gravedad){
    nombrePaciente = nombre;
    this->gravedad = gravedad;
}

ColaPrioridad::Paciente::Paciente(){
    nombrePaciente = nullptr;
    gravedad = 0;
}

ColaPrioridad::~ColaPrioridad(){

}

ColaPrioridad::ColaPrioridad(){

}

#endif // COLAPRIORIDAD_H
