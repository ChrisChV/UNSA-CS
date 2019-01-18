#include <iostream>

using namespace std;

int main()
{
    int numero_trabajadores = 0;
    float afp = 0;
    float seguro = 0;
    cout<<"Ingrese el numero de trabajadores"<<endl;
    cin>>numero_trabajadores;
    for(int i = 0; i < numero_trabajadores; i++){
        string nombre_trabajador;
        cout<<"Ingrese nombre del trabajador"<<endl;
        cin>>nombre_trabajador;
        float sueldo_bruto;
        cout<<"Ingrese el sueldo bruto de "<<nombre_trabajador<<endl;
        cin>>sueldo_bruto;
        cout<<"El sueldo líquido de "<<nombre_trabajador<<"es "<<sueldo_bruto - (sueldo_bruto * 0.09) - (sueldo_bruto * 0.13)<<endl;
        afp += sueldo_bruto * 0.09;
        seguro += sueldo_bruto * 0.13;
    }
    cout<<"La afp total es "<<afp<<endl;
    cout<<"El seguro total es "<<seguro<<endl;
}

