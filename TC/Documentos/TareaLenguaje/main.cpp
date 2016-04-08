#include <iostream>
#include "cctype"
#include "fstream"

using namespace std;

bool isSimbol(string::iterator &iter){
    if(*iter == '+' or *iter == '-' or *iter == '*' or *iter == '/')return true;
    return false;
}

bool isSimbol(const char iter){
    if(iter == '+' or iter == '-' or iter == '*' or iter == '/')return true;
    return false;
}

bool verificarBegin(char* linea){
    if(linea[0] == 'B' and linea[1] == 'e'  and linea[2] == 'g' and linea[3] == 'i' and linea[4] == 'n')return true;
    return false;
}

bool verificarEnd(char* linea){
    if(linea[0] == 'E' and linea[1] == 'n' and linea[2] == 'd')return true;
    return false;
}

bool pascal(){
    ifstream archivo("cod.txt");
    if(archivo.fail()){
        cout<<"El archivo no se puede abrir"<<endl;
        return false;
    }
    int estado = 0;
    char linea[128];
    while(archivo.getline(linea,128)){
        if(verificarBegin(linea) and estado == 0) estado = 1;
        else if(verificarEnd(linea) and (estado == 1 or estado == 4)) estado = 5;
        else{
            for(int i = 0; linea[i] != '\0'; i++){
                switch(estado){
                    case 1:
                        if(linea[i] == '_' or isalpha(linea[i])) estado = 2;
                        else return false;
                        break;
                    case 2:
                        if(isdigit(linea[i])) estado = 2;
                        else if(linea[i] == '<'){
                            if(linea[i + 1] == '-'){
                                estado = 3;
                                i++;
                            }
                            else return false;
                        }
                        else return false;
                        break;
                    case 3:
                        if(isdigit(linea[i])) estado = 4;
                        else return false;
                        break;
                    case 4:
                        if(isdigit(linea[i])) estado = 4;
                        else if(isSimbol(linea[i])) estado = 3;
                        else if(linea[i] == '_' or isalpha(linea[i])) estado = 2;
                        else return false;
                        break;
                }
            }
        }
    }
    archivo.close();
    if(estado == 5)return true;
    return false;
}

bool eje2Tabla(){
    string op;
    cin>>op;
    int tablita[6][5] ={{1,3,2,-1,-1},
                {1,1,-1,-1,10},
                {-1,-1,-1,3,-1},
                {-1,1,-1,-1,10},
                {-1,4,-1,-1,10},
                {-1,-1,-1,-1,-1}};
    int estado = 0;
    int entrada = 0;
    auto iter = op.begin();
    while(estado != 10){
        if(iter == op.end()) entrada = 4;
        else if(*iter == ':') entrada = 2;
        else if(*iter == '=') entrada = 3;
        else if(isalpha(*iter)) entrada = 0;
        else if(isdigit(*iter)) entrada = 1;
        else entrada = 5;
        estado = tablita[estado][entrada];
        if(estado == -1)return false;
        iter++;
    }
    return true;
}

bool eje2(){
    string op;
    cin>>op;
    int estado = 0;
    for(auto iter = op.begin(); iter != op.end(); ++iter){
        switch(estado){
            case 0:
                if(isalpha(*iter)) estado = 1;
                else if(*iter == ':') estado = 2;
                else if(isdigit(*iter)) estado = 3;
                else return false;
                break;
            case 1:
                if(isdigit(*iter)) estado = 1;
                else if(isalpha(*iter)) estado = 1;
                else return false;
                break;
            case 2:
                if(*iter == '=') estado = 4;
                else return false;
                break;
            case 3:
                if(isdigit(*iter)) estado = 3;
                else return false;
                break;
        }
    }
    if(estado == 1 or estado == 3 or estado == 4)return true;
    return false;
}

bool operacion(){
    string op;
    cin>>op;
    int estado = 0;
    for(auto iter = op.begin(); iter != op.end(); ++iter){
        switch(estado){
            case 0:
                if(isdigit(*iter)) estado = 1;
                else if(isSimbol(iter)) estado = 2;
                else return false;
                break;
            case 1:
                if(isdigit(*iter)) estado = 1;
                else if(isSimbol(iter)) estado = 0;
                else return false;
                break;
            case 2:
                if(isdigit(*iter)) estado = 2;
                else if(isSimbol(iter)) estado = 2;
                else return false;
                break;
        }
    }
    if(estado == 1)return true;
    return false;
}

int main()
{
    if(pascal())cout<<"La operacion es correcta"<<endl;
    else cout<<"La operacion es incorrecta"<<endl;

}
