#include <iostream>
#include "fstream"
#include "list"
#include "vector"
#include "fstream"
//#include "Hombresito.h"
#include "map"
#include "time.h"
#include "math.h"
#include "mysql/mysql.h"


//using namespace std;

/*float convertirNumero(string numero){
    double resultado = 0;
    auto iter = numero.end();
    iter--;
    double contador = 0;
    for(iter; iter!= numero.begin(); iter--){
        if((*iter) == '.'){
            resultado /= pow(10,contador);
            contador = -1;
        }
        else{
            resultado += pow(10,contador) * ((*iter) - 48);
        }
        contador++;
    }
    resultado += pow(10,contador) * ((*iter) - 48);
    return resultado;
}

*/

int main()
{

    MYSQL * connect;
    connect = mysql_init(NULL);
    if (!connect)
    {
        std::cout<<"MySQL Initialization failed";
        return 1;
    }
    MYSQL_RES * res_set;
    MYSQL_ROW row;



    /*
    vector<int> n;

    list<int> num;
    num.push_back(1);
    num.push_back(2);
    num.push_back(3);
    num.push_back(4);
    num.push_back(5);
    num.push_back(6);
    auto iter = num.begin();
    num.insert(iter,10);
    cout<<"Y"<<*iter<<endl;
    for(int i : num){
        cout<<i<<endl;
    }
    */
    /*
    Hombresito *h = new Hombresito(70989549,true,'C');
    ofstream archivo("prueba/prueba.dat", ios::out | ios::binary | ios::app);
    cout<<"Pos Actual->"<<archivo.tellp()<<endl;
    archivo.write(reinterpret_cast<char *>(h),sizeof(Hombresito));
    archivo.close();
    */

    /*
    Hombresito *h;
    ifstream archivo("prueba/prueba.dat", ios::in | ios::binary);
    archivo.read(reinterpret_cast<char *>(&h),sizeof(Hombresito));
    h.print();
    */
}

