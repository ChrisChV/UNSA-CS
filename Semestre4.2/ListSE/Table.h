#ifndef TABLE_H
#define TABLE_H
#include "List.h"
#include <fstream>
#include <cstring>
#include <iostream>

/*void   strcpy(char * p, char *q)
{
      while(*p++=*q++);
}*/

/*int strlen(char * p)
{
   register int i =0;
   for(;*p;p++,i++);
   return i;

}*/
struct Record{
    char key[5];
    long start;
    long end;
};

template<class T>
class Table{
    private:
        List<T> m_record;
        char *file;
    public:
        Table(char *file);
        void index();
        T GetRecord(char *key);
        void printKey();
};

template<class T>
Table<T> :: Table(char *file){
    this->file = file;
}

template<class T>
void Table<T> :: index(){
    /** variables para guardar el key y la data temporalmente **/
    char key[10];
    char buffer[1000];

    std::ifstream in(file); /** abrimos el archivo para manipularlo **/
    long offset = 0; /** variable para determinar el inicio de cada registro **/
    while(!in.eof())/** recorrecmos hasta que encuentre el final del archivo **/
    {
        /*std::cout << in.tellg() << " ";*/

        in.getline(key, 11, ','); /** capturamos los 11 primeros caracteres o hasta encontrar una coma **/
        in.getline(buffer, 1000); /** capturamos los siguientes 1000 caracteres **/

        /*std::cout << key << buffer << std::endl;*/

        Record R;
        strcpy(R.key , key); /** copiamos el contenido de key en el key de Record **/
        R.start = offset; /** guardamos el inicio del registro **/
        m_record.pushBack(R); /** ingresamos el registro a lista **/
        offset += strlen(key) + strlen(buffer) + 3; /** actualizamos la variable para el inicio de la siguiente linea **/
    }
    in.close();
}

template<class T>
T Table<T> :: GetRecord(char * key)
{
       char data[1000];
       Record *r = m_record.findElem(key, new Record); /** funcion especial que devuelve el puntero de el Record buscado **/

       /*std::cout << r->start << std::endl;*/

       if(r){
           std::ifstream file(this->file); /** abrimos el archivo para manipularlo **/
           file.seekg(r->start, std::ios::beg); /** Nos posicionamos en el archivo segun la posicion guardada del registro **/
           file.getline(data, 1000); /** capturamos los la linea **/
           std::cout<< data << std::endl;
       }
}

template<class T>
void Table<T> :: printKey(){
    m_record.printStruct();
}

#endif // TABLE_H
