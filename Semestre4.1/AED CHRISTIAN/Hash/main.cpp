#include <iostream>
#include "Hash.h"
#include "stdlib.h"
#include "time.h"

using namespace std;



int suma(string key){
    int suma = 0;
    for(int i  = 0; i < key.size(); i++){
        suma += key[i];
    }
    return suma;
}

int main()
{
    srand(time(NULL));
    int(*x)(string);
    x = suma;
    Hash<string,string> h(x, 13);
    h.add("Chris","C001");
    string t;
    if(h._find("C001","Chris",t)){
        cout<<t<<endl;
    }
}
