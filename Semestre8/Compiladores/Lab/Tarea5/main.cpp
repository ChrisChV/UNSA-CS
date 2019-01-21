#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstring>
#include "scanner.h"

using namespace std;


int main(int argc, char **argv)
{
    FILE * f;
    f = stdin;
    int token = -1;
    string lexema;
    int comentario = SIN_COM;
    if(argc == 2){
        f = fopen(argv[1], "rt");
        if(f == NULL) f = stdin;
    }
    if(f == stdin) cout<<"Ingrese texto..."<<endl;
    while(1){
        token = scanner(lexema, comentario, f);
        if(token == EOF) break;
        mostrarToken(token,lexema);
    }
    if (f != stdin) fclose(f);
    return 0;
}
