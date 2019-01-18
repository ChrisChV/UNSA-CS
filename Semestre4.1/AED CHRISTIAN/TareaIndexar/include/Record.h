#ifndef RECORD_H
#define RECORD_H
#include "iostream"
#include "sad.h"

using namespace std;

class Record
{
    public:
        Record();
        char key[10];
        PositionInFile inicio;
        PositionInFile fila;
};


Record::Record()
{
    inicio = 0;
    fila = 0;
}

#endif // RECORD_H

