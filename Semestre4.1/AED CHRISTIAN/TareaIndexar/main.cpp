#include <iostream>
#include <Table.h>
#include "list"
#include "sad.h"

using namespace std;

int main()
{
    Table cursos("cursos.dat");
    Campo campo;
    cout<<cursos.find("1001102",1,campo)<<endl;
    for(auto iter = campo.begin(); iter != campo.end(); ++iter){
        cout<<*iter<<endl;
    }
}
