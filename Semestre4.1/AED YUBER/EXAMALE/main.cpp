#include <iostream>

#include <map>
#include <list>
#include <vector>
#include <tuple>
#include <algorithm>

#include <fstream>


using namespace std;

typedef map<int, map<int,float>> Mygrafo;

vector<int> listarNodos(Mygrafo);
vector<tuple<int,int>> listarRelaciones(Mygrafo grafo);
bool existeCam(Mygrafo,int,int,vector<int>&);
bool conexoFuerteYSImple(Mygrafo grafo); ///1 y 6
bool deteccionCiclos(Mygrafo grafo); ///2
bool existeCam(Mygrafo grafo, int a, int b, vector<int>& visitados); ///3
bool caminoEuler(Mygrafo grafo, int origen, int destino); ///4
bool caminoHamilton(Mygrafo grafo, int origen, int destino); ///5
vector<int> clausuraTransitiva(Mygrafo grafo); ///7
map<int,int> dijkstra(Mygrafo &grafo, int i); ///9
vector<tuple<int,int>> caminosPares(Mygrafo grafo); ///12
int caminoMasLargo(Mygrafo grafo,int a, int b, vector<int> visitado, bool flag); ///15

void print(Mygrafo grafo){
    for(int i = 0; i < grafo.size(); i++){
        for(int j = 0; j < grafo.size(); j++){
            cout<<grafo[i][j]<<" ";
        }
        cout<<endl;
}
}

bool deteccionCiclos(Mygrafo grafo){ ///2
    for(Mygrafo::iterator i = grafo.begin(); i != grafo.end(); i++)
    {
        if(i->second.find(i->first) != i->second.end())
        {
            return true;
        }
    }
    return false;
}

bool conexoFuerteYSImple(Mygrafo grafo){ ///1
    vector<int> nodos = listarNodos(grafo);
    for(int i = 0; i < nodos.size(); i++)
    {
        vector<int> temporal = nodos;
        int temp = nodos[i];
        temporal.erase(temporal.begin() + i);
        for(int j = 0; j < temporal.size(); j++)
        {
            vector<int> visitado;
            if(!existeCam(grafo,nodos[i],temporal[j],visitado)){
                return false;
            }
        }
    }
    return true;
}

vector<int> listarNodos(Mygrafo grafo){
    vector<int> nodos;
    for(Mygrafo::iterator i = grafo.begin(); i != grafo.end(); ++i)
    {
        nodos.push_back(i->first);
    }
    return nodos;
}

vector<tuple<int,int>> listarRelaciones(Mygrafo grafo){
    vector<tuple<int,int>> resultado;
    for(Mygrafo::iterator i = grafo.begin(); i != grafo.end(); ++i)
    {
        for(map<int,float>::iterator j = i->second.begin(); j != i->second.end(); ++j)
        {
            resultado.push_back(make_tuple(i->first,j->first));
        }
    }
    return resultado;
}

void _caminosPares(Mygrafo grafo, int a, vector<int> visitado, vector<int> &resultado, int peso){
    if(find(visitado.begin(), visitado.end(),a) != visitado.end())
    {
        return;
    }
    if(peso != 0 and peso % 2 == 0)
    {
        resultado.push_back(a);
    }
    visitado.push_back(a);
    for(map<int,float>::iterator i = grafo[a].begin(); i != grafo[a].end(); i++)
    {
        _caminosPares(grafo,i->first,visitado,resultado,peso + i->second);
    }
}

bool existeCam(Mygrafo grafo, int a, int b, vector<int>& visitados){ ///3
    for(vector<int>::iterator i = visitados.begin(); i != visitados.end(); i++)
    {
        if(a == *i)
        {
            return false;
        }
    }
    visitados.push_back(a);
    if(grafo[a].find(b) != grafo[a].end())
    {
        return true;
    }
    for(auto iter = grafo[a].begin(); iter != grafo[a].end(); ++iter)
    {
        if(existeCam(grafo,iter->first,b,visitados))
        {
             return true;
        }
    }
    return false;
}

template <typename T>
int find(vector<T> valores, T a){
    for(int i = 0; i < valores.size(); i++)
    {
        if(valores[i] == a)
        {
            return i;
        }
    }
    return -1;
}

int caminoMasLargo(Mygrafo grafo,int a, int b, vector<int> visitado, bool flag){
    flag = false;
    if(find(visitado.begin(), visitado.end(),a) != visitado.end() or a == b)
    {
        flag = false;
        return -1;
    }
    int resultado = -1;
    visitado.push_back(a);
    if(grafo[a].find(b) != grafo[a].end())
    {
        flag = true;
        resultado = grafo[a][b];
    }
    int mayor = -1;
    for(map<int,float>::iterator i = grafo[a].begin(); i != grafo[a].end(); i++)
    {
        if(flag)
        {
            mayor = max(mayor,resultado);
        }
        resultado = caminoMasLargo(grafo,i->first,b,visitado,flag) + i->second;
    }
    if(flag)
    {
        mayor = max(mayor,resultado);
    }
    if(mayor == -1)
    {
        flag = false;
    }
    else
    {
        flag = true;
    }
    return mayor;

}

vector<int> clausuraTransitiva(Mygrafo grafo){
    vector<int> nodos = listarNodos(grafo);
    vector<int> resultado;
    for(int i = 0; i < nodos.size(); i++)
    {
        vector<int> temporal = nodos;
        temporal.erase(temporal.begin() + i);
        bool flag = true;
        for(int j = 0; j < temporal.size(); j++)
        {
            vector<int> visitado;
            if(!existeCam(grafo,j,i,visitado))
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {
            resultado.push_back(i);
        }
    }
    return resultado;
}

vector<tuple<int,int>> caminosPares(Mygrafo grafo){
    vector<int> nodos = listarNodos(grafo);
    vector<tuple<int,int>> resultado;
    for(int i = 0; i < nodos.size(); i++)
    {
        vector<int> result;
        vector<int> visitado;
        _caminosPares(grafo,nodos[i],visitado,result,0);
        for(int j = 0; j < result.size(); j++)
        {
            resultado.push_back(make_tuple(nodos[i],result[j]));
        }
    }
    return resultado;
}

bool _caminoHamilton(Mygrafo grafo, int origen, int destino, vector<int> faltantes){
    for(vector<int>::iterator i = faltantes.begin(); i != faltantes.end(); i++)
    {
        return false;
    }
    if(origen == destino and faltantes.empty())
    {
        return true;
    }
    faltantes.erase(faltantes.begin() + find(faltantes,origen));
    for(map<int,float>::iterator i = grafo[origen].begin(); i != grafo[origen].end(); ++i)
    {
        if(_caminoHamilton(grafo,i->first,destino,faltantes))
        {
            return true;
        }
    }
    return false;
}

bool caminoHamilton(Mygrafo grafo, int origen, int destino){///5
    vector<int> nodos = listarNodos(grafo);
    return _caminoHamilton(grafo,origen,destino,nodos);
}



bool _caminoEuler(Mygrafo grafo,int x, int origen, int destino, vector<tuple<int,int>> faltantes){
    tuple<int,int> temp = make_tuple(x,origen);
    if(x != -1)
    {
        for(vector<tuple<int,int>>::iterator i = faltantes.begin(); i != faltantes.end(); i++)
        {
            if(temp == *i)
            {
                return false;
            }
        }
    }
    faltantes.erase(faltantes.begin() + find(faltantes,temp));
    for(auto i = grafo[origen].begin(); i != grafo[origen].end(); i++)
    {
        if(_caminoEuler(grafo,origen,i->first,destino,faltantes))
        {
            return true;
        }
    }
    return false;
}

bool caminoEuler(Mygrafo grafo, int origen, int destino){///4
    vector<tuple<int,int>> relaciones = listarRelaciones(grafo);
    return _caminoEuler(grafo,-1,origen,destino,relaciones);
}

void generarVectores(Mygrafo &grafo, vector<int> &valores, map<int,int> &distancias, int i, map<int,bool> &candidatos){
    for(int j = 0; j < valores.size(); j++)
    {
        if(grafo[i].find(valores[j]) != grafo[i].end())
        {
            distancias[valores[j]] = grafo[i][valores[j]];
        }
        else
        {
            distancias[valores[j]] = 1000;
        }
        candidatos[valores[j]] = false;
    }
}

int getMin(map<int, bool> &candidatos, map<int,int> &distancias){
    int menor = 1001;
    int resultado = -1;
    for(map<int,bool>::iterator i = candidatos.begin(); i != candidatos.end(); i++)
    {
        if(!i->second)
        {
            if(menor > distancias[i->first])
            {
                menor = distancias[i->first];
                resultado  = i->first;
            }
        }
    }
    return resultado;
}

bool hayCero(map <int,bool> &candidatos){
    for(map<int,bool>::iterator i = candidatos.begin(); i != candidatos.end(); i++)
    {
        if(i->second == false)
        {
            return true;
        }
    }
    return false;
}

map<int,int> dijkstra(Mygrafo &grafo, int i){
    vector<int> valores = listarNodos(grafo);
    map<int,int> distancias;
    map<int,bool> candidatos;
    generarVectores(grafo,valores,distancias,i,candidatos);
    candidatos[i] = true;
    while(hayCero(candidatos))
    {
        int p = getMin(candidatos,distancias);
        candidatos[p] = true;
        for(map<int,int>::iterator i = distancias.begin(); i != distancias.end(); i++)
        {
            if(candidatos[i->first] == false)
            {
                int q = 1000;
                if(grafo[p].find(i->first) != grafo[p].end())
                {
                    q = grafo[p][i->first];
                }
                distancias[i->first] = min(distancias[i->first], distancias[p] + q);
            }
        }
    }
    return distancias;
}

int main()
{
    Mygrafo g;
    g[0][1] = 1;
    g[1][2] = 2;
    g[2][1] = 3;
    g[3][1] = 5;
    g[4][1] = 3;
    g[2][4] = 1;
    g[1][4] = 5;
    g[5][4] = 1;
    g[1][5] = 1;
    g[0][6] = 5;
    g[6];
    /*
    g[0][3] = 5;
    g[3][0] = 5;
    g[0][4] = 6;
    g[4][0] = 6;
    g[1][4] = 7;
    g[4][1] = 7;
    g[2][4] = 8;
    g[4][2] = 8;
    g[2][5] = 9;
    g[5][2] = 9;
    g[2][6] = 10;
    g[6][2] = 10;
    */


}
