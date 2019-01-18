#include <iostream>
#include <algorithm>
#include "vector"
#include "ListaESpecialidad.h"
#include "random"
#include "time.h"

using namespace std;

int main()
{
    srand (time(NULL));
    ListaESpecialidad especialidades;
    especialidades.insertar("Psicologia");
    especialidades.insertar("Pediatria");
    especialidades.insertar("Medicina General");
    especialidades.agregarPaciente("Chris","Psicologia",rand() % 10 + 1);
    especialidades.agregarPaciente("Nicoll", "Medicina General", rand() % 10 + 1);
    especialidades.agregarPaciente("Juan","Psicologia",rand() % 10 + 1);
    especialidades.agregarPaciente("Carlos", "Pediatria", rand() % 10 + 1);
    especialidades.agregarPaciente("Ruben","Psicologia",rand() % 10 + 1);
    especialidades.mostrarPAcientes("Psicologia");
    cout<<endl;
    especialidades.printAll();
}
