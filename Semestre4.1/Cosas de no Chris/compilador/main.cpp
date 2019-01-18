#include <iostream>

using namespace std;

int getFirstType(char a){
    if( a <='9' && a>='0')
        return 0; // numero o digito
    else if( (a >='a' && a<='z') || (a>='A' && a<='Z') )
            return 1;
    else
        return 2;
}


void whatis(){
    string cadena;
    cin>>cadena;
    int state;
    int tipo = getFirstType(cadena[0]);

    cout<<tipo<<endl;

    while(true)
    {
        if(tipo == 2)
            break;
        switch(state){

        }
}



int main()
{
    whatis();

    cout << "Hello world!" << endl;
    return 0;
}
