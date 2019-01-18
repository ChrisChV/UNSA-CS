#include <iostream>
#include <fstream>
#include "GramaticaRegular.h"

using namespace std;

int main(int argc, char ** argv)
{
    if(argc != 3){
        cout<<"Solo deben entrar dos parametros"<<endl;
        return 0;
    }
    string file1(argv[1]);
    string file2(argv[2]);
    GramaticaRegular ejem(file1);
    ejem.imprimirTabla();
    ejem.generarAutomata();
    cout<<endl;
    ejem.reconocerCadena(file2);
    return 0;

}
