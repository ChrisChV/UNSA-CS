#include <iostream>
#include "../../AFNToAFD/compiladorAFNToAFD.h"
#include "../../Error/error.h"


using namespace std;

int main(int argc, char ** argv)
{
    try{
        if(argc != 3){
            cout<<"Faltan argumentos <fileIn> <fileOut>"<<endl;
            return 0;
        }
        string fileName(argv[1]);
        string fileOut(argv[2]);
        CompiladorAFNToAFD compilador;
        compilador.run(fileName, fileOut);
        
    }
    catch(Error e){
        manejarError(e);
    }
    
}
