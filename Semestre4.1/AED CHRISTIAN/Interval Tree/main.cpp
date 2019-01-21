#include <iostream>
#include "IntervalTree.h"
#include "stdlib.h"
#include "time.h"

using namespace std;

int main()
{
    srand(time(NULL));
    IntervalTree<int> arbolito;
    int low = 0;
    int high = 0;
    for(int i = 0; i < 1000; i++){
        low = rand() % 10000;
        high = low + (rand() % 1000);
        arbolito.insert(low,high);
    }
    arbolito.print();

}
