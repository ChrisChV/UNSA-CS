#include <iostream>
#include "BinomialHeap.h"
#include "stdlib.h"
#include "time.h"

using namespace std;

int main()
{
    srand(time(NULL));
    BinomialHeap<int> miHeap("Chris_Heap",true);
    int number = 0;
    for(int i = 0; i < 20; i++){
        number = rand() % 10000;
        miHeap.insert(i);
    }
    miHeap.deleteMin();
    miHeap.print("mensaje");




}
