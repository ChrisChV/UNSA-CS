#include <iostream>
#include "ListaDoble.h"

using namespace std;

int main()
{
    ListaDoble<int> my;
    for(int i = 0; i < 10; i ++){
        my.add(i);
    }
    my.eliminar(0);
    my.eliminar(1);
    my.eliminar(5);
    my.eliminar(9);
    my.eliminar(100);
    my.print();
}
