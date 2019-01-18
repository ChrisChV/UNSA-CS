#include <iostream>
#include "RedBlack.h"
#include "stdlib.h"
#include "time.h"
using namespace std;

int main()
{
    srand(time(NULL));
    RedBlack<int> arbolito;
    int number = 0;
    for(int i = 0; i < 1000; i++){
        number = rand() % 10000;
        arbolito.insert(number);
    }
    arbolito.print();






}
