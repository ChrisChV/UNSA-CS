#include <iostream>
#include "Lista.h"

using namespace std;

int main()
{
    Lista<int> miLista;
    for(int i = 0; i < 10; i++){
        miLista.push_back(i);
    }
    Lista<int> miLista2;
    miLista2.push_back(3);
    miLista2.push_back(5);
    miLista2.push_back(13);
    miLista2.push_back(20);
    miLista2.push_back(30);
    miLista2.push_back(-1);
    auto miLista3 = miLista.intersect(miLista2);
    miLista3.print();

}
