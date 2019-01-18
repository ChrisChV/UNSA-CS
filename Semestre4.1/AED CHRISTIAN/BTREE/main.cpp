#include <iostream>
#include "BT.h"
#include "time.h"
#include "stdlib.h"

void generarArchivo(int n, string file){
    ofstream archivo(file);
    char c = 97;
    for(int i = 0; i < n; i++){
        if(c == 123) c = 97;
        archivo<<i<<","<<c<<endl;
        c++;
    }
    archivo.close();
}


using namespace std;

int main()
{
    //generarArchivo(100000,"test2.txt");
    srand(time(NULL));
    cout << "Hello world!" << endl;
    //system("rm -R superArbolito/");

    BT<int> arbolito("superArbolito",1024,false);

    arbolito.generarDeArchivo("test2.txt");
    int k = 0;

    //for(int i = 0; i < 1024; i++){
        arbolito.del(99973);
    //}

    while(true){
        cin>>k;
        cout<<endl;
        arbolito.findEntity(k);
    }



    //733;

    /*
    for(int i = 0; i < 100000; i++){
        int j = rand()  % 3000000 ;
        arbolito.insert(i,i);
        if(arbolito.size() % 1000 == 0) cout<<arbolito.size()<<endl;
    }

    for(int i = 0 ; i < 1024; i++){
        arbolito.del(i);
    }
    for(int i = 0; i < 1024; i++){
        arbolito.insert(i,i);
    }
    int k = 0;
    while(true){
        cin>>k;
        cout<<arbolito.find(k)<<endl;
    }
    */



    //cout<<arbolito.root<<endl;

    /*arbolito.insert(10);
    arbolito.insert(20);
    arbolito.insert(30);
    arbolito.insert(40);
    arbolito.insert(50);
    arbolito.insert(60);
    arbolito.insert(5);
    arbolito.insert(15);
    arbolito.insert(70);
    arbolito.insert(80);
    arbolito.insert(25);
    arbolito.insert(90);
    arbolito.insert(100);
    arbolito.insert(110);
    arbolito.insert(45);
    arbolito.insert(55);
    arbolito.insert(58);
    arbolito.insert(46);
    arbolito.insert(47);
    arbolito.insert(48);
    arbolito.insert(39);
    arbolito.insert(26);
    arbolito.insert(27);
    arbolito.insert(28);
    arbolito.insert(38);
    arbolito.insert(37);
    arbolito.insert(85);
    arbolito.insert(87);
    arbolito.insert(89);
    arbolito.insert(52);
    arbolito.insert(53);
    arbolito.insert(54);
    arbolito.insert(120);
    arbolito.insert(130);
    arbolito.insert(140);
    arbolito.insert(1);
    arbolito.insert(2);
    arbolito.insert(3);
    arbolito.insert(6);
    arbolito.insert(7);
    arbolito.insert(8);
    arbolito.insert(11);
    arbolito.insert(12);
    arbolito.insert(13);
    */
    //arbolito.print();

    //BT<int> arbolito("arbolito",4,false);


    //arbolito.generarDeArchivo("test.txt");


    cout<<"ALTURA->"<<arbolito.altura<<endl;
    cout<<"NODOS->"<<arbolito.sizeNod()<<endl;
/*
    arbolito.del(8);
    arbolito.del(17);
    arbolito.del(0);
    arbolito.del(2);
    arbolito.del(1);
    arbolito.del(3);
    arbolito.del(5);
    arbolito.del(4);
    arbolito.del(21);
*/

/*
    list<string> r;
    r.push_back("Melissa");
    r.push_back("67");

    arbolito.insert(8,r);
    r.clear();
    r.push_back("Ale");
    r.push_back("27");
    arbolito.insert(9,r);
    arbolito.imprimirEntity(arbolito.getEntity(9));
    */

    //arbolito.printDot();
    //arbolito.eliminarArbol();
}
