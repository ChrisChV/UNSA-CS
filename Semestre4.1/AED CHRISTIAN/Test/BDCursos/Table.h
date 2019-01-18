#ifndef TABLE_H
#define TABLE_H
#include <list>
#include <fstream>
#include "Record.h"
#include <algorithm>
#include "sad.h"
#include <cstring>

using namespace std;

class Table
{
    public:
        Table();
        Table(const char *file);
        void _print();
        bool find(Key,Campo &);
        bool find(Key,int, Campo &);
        bool next(Campo &);
        bool prev(Campo &);
        bool insert(Campo &);
        bool del(Key);
        virtual ~Table();
    protected:
    private:
        list<Record *> m_RecordSet;
        void _index(const char *file);
        bool _find(Key, list<Record *>::iterator &iter, long &);
        bool _find(Key, list<Record *>::iterator &iter);
        void _llenarCampo(Campo &);
        void _llenarCampo(char *,Campo &);
        const char * file;
        list<Record *>::iterator recordActual;
        int numero_columnas;
};

void Table::_llenarCampo(char *linea, Campo & lista){
    string campo;
    for(int i = 0; i < strlen(linea); i++){
        if(linea[i] == ',' || linea[i] == '\n'){
            lista.push_back(campo);
            campo.clear();
        }
        else{
            campo.insert(campo.end(),linea[i]);
        }
    }
    lista.push_back(campo);
}

bool Table::find(Key busqueda, int numero_columna, Campo & lista){
    if(numero_columna > numero_columnas)return false;
    if(numero_columna == 1)return this->find(busqueda,lista);
    ifstream archivo(file);
    string temporal;
    char campo[1000];
    int contador = 1;
    while(archivo.getline(campo,1000)){
        contador = 1;
        temporal.clear();
        for(int i = 0; i < strlen(campo); i++){
            if(campo[i] == '\n')break;
            if(campo[i] == ','){
                if(contador == numero_columna){
                    if(temporal == busqueda){
                        _llenarCampo(campo, lista);
                    }
                }
                contador++;
                temporal.clear();
            }
            else{
                temporal.insert(temporal.end(),campo[i]);
            }
        }
    }
    archivo.close();
    if(lista.empty())return false;
    return true;

}

bool Table::del(Key key){
    list<Record*>::iterator temp;
    long linea;
    if(!this->_find(key,temp,linea))return false;
    char campo[1000];
    ifstream archivo(file);
    ofstream archivoTemp("temp.dat");
    m_RecordSet.erase(temp);
    for(int i = 0; i < linea; i++){
        archivo.getline(campo, 1000);
        archivoTemp<<campo<<'\n';
    }
    archivo.getline(campo,1000);
    while(archivo.getline(campo,1000)){
        archivoTemp<<campo<<'\n';
    }
    archivo.close();
    archivoTemp.close();
    remove(file);
    rename("temp.dat",file);

    return true;
}

bool Table::insert(Campo &lista){
    list<Record*>::iterator temp;
    char key[5];
    for(int i = 0; i < lista.front().size(); i++){
        key[i] = lista.front()[i];
    }
    if(this->_find(key,temp))return false;
    ofstream archivo(file, ios::app);
    Record *nuevo = new Record();
    strcpy(nuevo->key,key);
    nuevo->inicio = archivo.tellp();
    m_RecordSet.push_back(nuevo);
    auto temporal = lista.end();
    temporal--;
    for(auto iter = lista.begin(); iter != lista.end(); ++iter){
        if(iter == temporal)archivo<<*iter<<'\n';
        else archivo<<*iter<<',';
    }
    archivo.close();
    return true;
}

void Table::_llenarCampo(Campo & lista){
    ifstream archivo(file);
    archivo.seekg((*recordActual)->inicio,std::ios::beg);
    char caracter = ' ';
    string campo;
    while(caracter != '\n'){
        archivo.get(caracter);
        if(caracter == ',' || caracter == '\n'){
            lista.push_back(campo);
            campo.clear();
        }
        else{
            campo.insert(campo.end(),caracter);
        }
    }
}

bool Table::prev(Campo &lista){
    if(recordActual == m_RecordSet.begin())return false;
    recordActual--;
    _llenarCampo(lista);
    return true;
}

bool Table::next(Campo &lista){
    auto temp = m_RecordSet.end();
    temp--;
    if(recordActual == temp)return false;
    recordActual++;
    _llenarCampo(lista);
    return true;
}

bool Table::find(Key key, Campo &lista){
    recordActual = m_RecordSet.begin();
    if(!_find(key, recordActual))return false;
    _llenarCampo(lista);
    return true;
}

bool Table::_find(Key key, list<Record *>::iterator &iter){
    iter = m_RecordSet.begin();
    if(m_RecordSet.empty())return false;
    for(iter; iter != m_RecordSet.end(); ++iter){
        if(!strcmp((*iter)->key, key))return true;
    }
    return false;
}

bool Table::_find(Key key, list<Record *>::iterator &iter, long &contador){
    iter = m_RecordSet.begin();
    contador = 0;
    if(m_RecordSet.empty())return false;
    for(iter; iter != m_RecordSet.end(); ++iter){
        if(!strcmp((*iter)->key, key))return true;
        contador++;
    }
    return false;
}

void Table::_print(){
    for(auto iter = m_RecordSet.begin(); iter != m_RecordSet.end(); ++iter){
        cout<<(*iter)->key<<endl;
    }
}

void Table::_index(const char *file){
    this->file = file;
    ifstream archivo(file);
    char linea[1000];
    char caracter[10];
    long pos = 0;
    long fila = 1;
    while(archivo.good()){
        archivo.getline(caracter,11,',');
        archivo.getline(linea,1000);
        Record *temp = new Record();
        temp->inicio = pos;
        temp->fila = fila;
        strcpy(temp->key,caracter);
        m_RecordSet.push_back(temp);
        pos += strlen(caracter) + strlen(linea) + numero_columnas - 1;
        fila++;
    }
    auto iter = m_RecordSet.end();
    iter--;
    m_RecordSet.erase(iter--);
    archivo.close();
    recordActual = m_RecordSet.begin();
}


Table::Table(){

}

Table::Table(const char *file){
    numero_columnas = 0;
    ifstream archivo(file);
    char linea[1000];
    archivo.getline(linea, 1000);
    for(int i = 0; i < strlen(linea); i++){
        if(linea[i] == ',')numero_columnas++;
    }
    numero_columnas++;
    archivo.close();
    _index(file);
}

Table::~Table(){
    delete(file);
}

#endif // TABLE_H
