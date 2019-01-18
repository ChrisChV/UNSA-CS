#include <iostream>
#include <fstream>

using namespace std;

void unirArchivos(string file1, string file2, string file3){
    ifstream archivo1(file1.c_str());
    if(archivo1.fail()){
        cout<<"El archivo "<<file1<<"no existe"<<endl;
        return;
    }
    ifstream archivo2(file2.c_str());
    if(archivo2.fail()){
        cout<<"El archivo "<<file2<<"no existe"<<endl;
        return;
    }
    ofstream archivo3(file3.c_str());
    char caracter;
    while(archivo1.get(caracter)){
        archivo3<<caracter;
    }
    archivo3<<endl;
    while(archivo2.get(caracter)){
        archivo3<<caracter;
    }
    archivo1.close();
    archivo2.close();
    archivo3.close();
}

int main(int argc,char **argv)
{
    if(argc != 4){
        cout<<"Debe ingresar tres archivos"<<endl;
        return 0;
    }
    string l[3];
    for(int i = 1; i < argc; i++){
        string temp(argv[i]);
        l[i-1] = temp;
    }
    unirArchivos(l[0],l[1],l[2]);
    return 0;
}
