#include <iostream>
#include "FibonacciHeap.h"
#include "stdlib.h"
#include "time.h"

using namespace std;

int main()
{
    srand(time(NULL));
    FibonacciHeap<int> fib;
    for(int i = 0; i < 10000; i++){
        int j = rand() % 10000;
        fib.insert(j);
    }
    fib.popMin();
    /*
    fib.decreaseKey(27,0);
    fib.decreaseKey(31,-1);
    fib.decreaseKey(30,-2);
    fib.popMin();
    */
    cout<<fib.getMin()<<endl;
    fib.print();

}
