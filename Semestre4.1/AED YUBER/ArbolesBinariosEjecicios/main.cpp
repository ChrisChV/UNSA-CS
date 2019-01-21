#include <iostream>
#include "ArbolBinario.h"

using namespace std;

int main()
{
    cout<<"INGRESE SU N"<<endl;
    int n;
    cin>>n;
    ArbolBinario arbolito;
    list<ArbolBinario> arbolitos;
    for(int i = 0; i < n; i++){
        cout<<"CUANTOS NODOS TENDRA EL ARBOL"<<i+1<<"??"<<endl;
        int n2;
        cin>>n2;
        ArbolBinario temp;
        for(int j = 0; j < n2; j++){
            cout<<"INGRESE EL NODO "<<j +1<<endl;
            int val;
            cin>>val;
            temp.insert(val);
        }
        arbolitos.push_back(temp);
    }
    arbolito.uni(arbolitos);
    arbolito.print();

}
