#include <iostream>
#include <cmath>
 
using namespace std;
 
float t = 0.001;
float terminus = 100000;
float presicion = 0.1;
 
float funcion(float x){
    return exp(x);  
}
 
float derivada_media(float xo, float xf){
    float dx =  xf - xo;
    float dy = funcion(xf) - funcion(xo);
    float Dy = dy / dx;
    return Dy;
}
 
float derivada(float xo){
    float dx = 0.001;
    float xf = xo + dx;
    float Dya = derivada_media(xo,xf);
    float Dy = 0;
    float c = 0;
    do{
        dx += t;
        xf = xo + dx;
        Dy = derivada_media(xo,xf);
        c++;
        //cout<<c<<endl<<"Dya "<<Dya<<endl<<"Dy "<<Dy<<endl<<"Dif "<<abs(Dy-Dya)/abs(Dya);
    }while(abs(Dy-Dya) /abs(Dya) <= presicion and c != terminus);
    if(c == terminus) cout<<"El programa no pudo resolver la derevada"<<endl;
    else cout<<"Dya->"<<Dya<<endl<<"Dy->"<<Dy<<endl<<"Xf->"<<xf<<endl<<"dx->"<<dx<<endl<<"Diferencia:"<< abs(Dy-Dya)/abs(Dya)<<endl;
    return Dy;
}