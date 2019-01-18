#include <iostream>
#include "AVL.h"
#include "stdlib.h"
#include "time.h"
using namespace std;

int main()
{
    srand(time(NULL));
    AVL<int> arbolito;
    int number = 0;
    for(int i = 0; i < 100; i++){
        //arbolito.print2();
        number = rand() % 10000;
        arbolito.add(number);
        //arbolito.print();
    }
    arbolito.print();








}
