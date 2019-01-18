#include <iostream>
#include "Monticulo.h"

using namespace std;

int main()
{
    vector<int> a;
    for(int i = 10; i > 0; i--){
        a.push_back(i);
    }
    heapSort(a);
    for(int i = 0; i < a.size(); i++){
        cout<<a[i]<<"->";
    }


}
