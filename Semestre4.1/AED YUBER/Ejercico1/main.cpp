#include <iostream>
#include "ListaEmpleados.h"

enum Areas{ID, INFORMATICA, CONTABILIDAD, TESORERIA};

using namespace std;

int main()
{
    ListaEmpleados empleados;
    empleados.addEmpleado("Chris");
    empleados.addEmpleado("Nicoll");
    empleados.addEmpleado("Carlos");
    empleados.addTarea(0,INFORMATICA, "Tarea 1 para Chris");
    empleados.addTarea(0, CONTABILIDAD, "Tarea 2 para Chris");
    empleados.addTarea(0, TESORERIA, "Tarea 3 para Chris");
    empleados.addTarea(1, TESORERIA, "Tarea 1 para Nicoll");
    empleados.addTarea(1, ID, "Tarea 2 para Nicoll");
    empleados.addTarea(ID, "Tarea para el que tiene menos tareas");
    empleados.printMax();
    empleados.printMin();
}
