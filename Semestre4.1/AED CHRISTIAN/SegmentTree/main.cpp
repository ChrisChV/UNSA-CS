#include <iostream>
#include "vector"
#include "SegmentTree.h"
#include "stdlib.h"
#include "time.h"


using namespace std;

int suma(vector<int>::iterator init, vector<int>::iterator fin){
    int result = 0;
    while(init != fin){
        result += *init;
        init++;
    }
    return result + *fin;
}

int main()
{
    srand(time(NULL));
    int (*funcion) (vector<int>::iterator, vector<int>::iterator);
    funcion = suma;

    SegmentTree<int> arbolito(funcion);
    vector<int> vec;
    for(int i = 0; i < 10000; i++){
        vec.push_back(i);
    }
    arbolito.generate(vec);
    cout<<arbolito.find(0,10000)<<endl;
    //arbolito.print();
}
