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
    arbolito.insert(26);
    arbolito.insert(17);
    arbolito.insert(41);
    arbolito.insert(14);
    arbolito.insert(21);
    arbolito.insert(30);
    arbolito.insert(47);
    arbolito.insert(10);
    arbolito.insert(16);
    arbolito.insert(19);
    arbolito.insert(21);
    arbolito.insert(28);
    arbolito.insert(38);
    arbolito.insert(7);
    arbolito.insert(12);
    arbolito.insert(14);
    arbolito.insert(20);
    arbolito.insert(35);
    arbolito.insert(39);
    arbolito.insert(3);
    /*
    for(int i = 0; i < 50; i++){
     	number = rand() % 10000;
        arbolito.insert(number);
    }
    */
    cout<<arbolito.rank(20)<<endl;
    cout<<arbolito.select(13)<<endl;

    arbolito.print();
}
