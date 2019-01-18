#ifndef LISTAPOL_H
#define LISTAPOL_H
#include "NodoPol.h"

class LIstaPol
{
    public:
        LIstaPol();
        void ingresar(int, int);
        NodoPol* _find(int);
        NodoPol* _findMAyor(int);
        void print();
        virtual ~LIstaPol();
    protected:
    private:
        NodoPol *cabeza;
        NodoPol *cola;
};

#endif // LISTAPOL_H
