#include <iostream>
#include "ArbolBinario.h"

using namespace std;

int main()
{
    ArbolBinario arbolito;
    arbolito.insert(17);
    arbolito.insert(25);
    arbolito.insert(32);
    arbolito.insert(43);
    arbolito.insert(44);
    arbolito.insert(72);
    arbolito.insert(106);
    arbolito.insert(27);
    arbolito.insert(12);
    arbolito.insert(8);
    arbolito.insert(1);
    cout<<"Preorden->"<<endl;
    arbolito.preorden();
    cout<<endl<<"Inorden->"<<endl;
    arbolito.inorden();
    cout<<endl<<"Posorden->"<<endl;
    arbolito.postorden();
    cout<<endl<<"Amplitud->"<<endl;
    arbolito.amplitud();
    cout<<endl<<"Preorden Inverso->"<<endl;
    arbolito.preordenInverso();
    cout<<endl<<"Inorden Inverso->"<<endl;
    arbolito.inordenInverso();
    cout<<endl<<"Posorden Inverso->"<<endl;
    arbolito.postordenInverso();
    cout<<endl<<"Amplitud Inversa->"<<endl;
    arbolito.amplitudInverso();
}
