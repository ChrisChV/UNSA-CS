#include <iostream>
#include "Circular.h"

using namespace std;

int main()
{
    Circular<int> my;
    for(int i = 0; i < 10; i ++){
        my.insertar(i);
    }
    cout<<my.find(10)<<endl;
}
