#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    char cadena;
    ifstream fe("datos.txt");
    while(!fe.eof()) {
        fe>>cadena;
        cout<<cadena<<" ";
    }
    cout<<endl;
    fe.close();
    return 0;
}
