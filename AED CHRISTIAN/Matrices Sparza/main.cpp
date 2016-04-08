#include <iostream>
#include "MatrizSparza.h"

using namespace std;

int main()
{
    MatrizSparza<char> Chris(5,17,' ');
    for(int i = 0; i < 5; i ++){
        Chris[i][0] = 'x';
    }
    Chris[0][1] = 'x';
    Chris[4][1] = 'x';
    Chris[0][2] = 'x';
    Chris[4][2] = 'x';

    for(int i = 0; i < 5; i ++){
        Chris[i][4] = 'x';
    }

    Chris[2][5] = 'x';
    Chris[2][5] = 'x';

    for(int i = 0; i < 5; i ++){
        Chris[i][6] = 'x';
    }
    for(int i = 0; i < 5; i ++){
        Chris[i][8] = 'x';
    }
    Chris[0][9] = 'x';
    Chris[2][9] = 'x';
    Chris[0][10] = 'x';
    Chris[1][10] = 'x';
    Chris[3][10] = 'x';
    Chris[4][10] = 'x';
    for(int i = 0; i < 5; i ++){
        Chris[i][12] = 'x';
    }
    Chris[0][14] = 'x';
    Chris[1][14] = 'x';
    Chris[2][14] = 'x';
    Chris[4][14] = 'x';

    Chris[0][15] = 'x';
    Chris[2][15] = 'x';
    Chris[4][15] = 'x';

    Chris[0][16] = 'x';
    Chris[2][16] = 'x';
    Chris[3][16] = 'x';
    Chris[4][16] = 'x';
    //Chris.delFila(2);
    //Chris.delFila(0);
    //Chris.intercambiarFilas(2,0);
    //Chris.intercambiarColumnas(0,5);
    //Chris.delCol(4);
    //Chris.agregarFila(2);
    Chris.agregarColumna(2);
    Chris.intercambiarColumnas(16,17);
    //Chris.intercambiarColumnas(15,16);
    //hris.intercambiarColumnas(14,15);
    Chris.print();
}
