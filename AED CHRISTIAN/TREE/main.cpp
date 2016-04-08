#include <iostream>
#include "Tree.h"

using namespace std;

int main()
{
    Tree<int> arbolito;
    arbolito.insert(9);
    arbolito.insert(5);
    arbolito.insert(14);
    arbolito.insert(3);
    arbolito.insert(8);
    arbolito.insert(11);
    arbolito.insert(20);
    arbolito.insert(16);
    arbolito.insert(18);
    arbolito.printDot();
    arbolito.del(18);
    arbolito.del(14);
    arbolito.del(20);

}
