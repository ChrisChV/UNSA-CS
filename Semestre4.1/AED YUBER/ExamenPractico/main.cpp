#include <iostream>
#include "list"
#include "map"
#include "algorithm"
#include "vector"
#include "fstream"
#include "tuple"


#define INFINITOPOSITIVO 10000
#define MENOSINFINITO -10000



using namespace std;
typedef map<int, map<int,float>> Grafo;
typedef map<int,tuple<int,map<int,float>>> GrafoColoreado;



bool esGrafo(Grafo, vector<int>&);
bool hayCamino(Grafo,int,int,vector<int>&);
void printG(Grafo);
int caminoMinimo(Grafo,int,int,vector<int>&,bool&);
bool tieneRelacion(Grafo, int, int);

template <typename T>
vector<T> filtreRepeat(vector<T> valores){ ///////////////77777
    vector<T> resultado;
    for(int i = 0; i < valores.size(); i++){
        if(find(resultado.begin(),resultado.end(),valores[i]) == resultado.end()) resultado.push_back(valores[i]);
    }
    return resultado;
}

int minRelacion(map<int,float> g, int a){ ///////////////
    int menor = 10000;
    int index = 0;
    for(auto iter = g.begin(); iter != g.end(); ++iter){
        if(iter->second < menor){
            menor = iter->second;
            index = iter->first;
        }
    }
    return index;
}

template <typename T>
int find(vector<T> valores, T a){ //////////////////
    for(int i = 0; i < valores.size(); i++){
        if(valores[i] == a)return i;
    }
    return -1;
}

void print(Grafo grafo){
    for(int i = 0; i < grafo.size(); i++){
        for(int j = 0; j < grafo.size(); j++){
            cout<<grafo[i][j]<<" ";
        }
        cout<<endl;
    }
}

bool ciclos(Grafo grafo){ /////////////////////////
    for(auto iter = grafo.begin(); iter != grafo.end(); ++iter){
        if(iter->second.find(iter->first) != iter->second.end()) return true;
    }
    return false;
}

bool tieneRelacion(Grafo grafo, int a, int b){
    if(grafo[a].find(b) != grafo[a].end())return true;
    return false;
}

bool conexoFuerte(Grafo grafo){ /////////////////////////
    vector<int> valores;
    if(!esGrafo(grafo,valores)) return false;
    for(int i = 0; i < valores.size(); i++){
        vector<int> valTemp = valores;
        int temp = valores[i];
        valTemp.erase(valTemp.begin() + i);
        for(int j = 0; j < valTemp.size(); j++){
            vector<int> visitado;
            if(!hayCamino(grafo,valores[i],valTemp[j],visitado))return false;
        }
    }
    return true;
}

bool esGrafo(Grafo grafo, vector<int> &valores){ ///////////////////
    int siz = grafo.size();
    for(auto iter = grafo.begin(); iter != grafo.end(); ++iter){
        if(iter->second.size() > siz) return false;
        valores.push_back(iter->first);
    }
    for(auto iter = grafo.begin(); iter != grafo.end(); ++iter){
        for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
            if(find(valores.begin(), valores.end(), iter2->first) == valores.end())return false;
        }
    }
    return true;
}

int caminoMinimo(Grafo grafo, int a, int b, vector<int>& visitado, bool &flag){ ///////////////////7
    flag = false;
    if(find(visitado.begin(), visitado.end(),a) != visitado.end() or a == b){
        flag = false;
        return INFINITOPOSITIVO;
    }
    int resultado = 0;
    visitado.push_back(a);
    if(grafo[a].find(b) != grafo[a].end()){
        flag = true;
        resultado = grafo[a][b];
    }
    int menor = INFINITOPOSITIVO;
    for(auto iter = grafo[a].begin(); iter != grafo[a].end(); ++iter){
        if(flag) menor = min(menor,resultado);
        resultado = caminoMinimo(grafo,iter->first,b,visitado,flag) + iter->second;
    }
    if(flag) menor = min(menor,resultado);
    if(menor == INFINITOPOSITIVO) flag = false;
    else flag = true;
    return menor;
}

void _caminosPares(Grafo grafo, int a, vector<int> visitado, vector<int> &resultado, int peso){ //////////////////
    if(find(visitado.begin(), visitado.end(),a) != visitado.end())return;
    if(peso != 0 and peso % 2 == 0){
        resultado.push_back(a);
    }
    visitado.push_back(a);
    for(auto iter = grafo[a].begin(); iter != grafo[a].end(); ++iter){
        _caminosPares(grafo,iter->first,visitado,resultado,peso + iter->second);
    }
}

bool hayCamino(Grafo grafo, int a, int b, vector<int>& visitado){ ///////////////////////
    if(find(visitado.begin(), visitado.end(),a) != visitado.end())return false;
    visitado.push_back(a);
    if(grafo[a].find(b) != grafo[a].end())return true;
    for(auto iter = grafo[a].begin(); iter != grafo[a].end(); ++iter){
        if(hayCamino(grafo,iter->first,b,visitado)) return true;
    }
    return false;
}

int caminoMasLargo(Grafo grafo,int a, int b, vector<int> visitado, bool flag){ //////////////////////
    flag = false;
    if(find(visitado.begin(), visitado.end(),a) != visitado.end() or a == b){
        flag = false;
        return -1;
    }
    int resultado = -1;
    visitado.push_back(a);
    if(grafo[a].find(b) != grafo[a].end()){
        flag = true;
        resultado = grafo[a][b];
    }
    int mayor = -1;
    for(auto iter = grafo[a].begin(); iter != grafo[a].end(); ++iter){
        if(flag) mayor = max(mayor,resultado);
        resultado = caminoMasLargo(grafo,iter->first,b,visitado,flag) + iter->second;
    }
    if(flag)mayor = max(mayor,resultado);
    if(mayor == -1) flag = false;
    else flag = true;
    return mayor;

}


vector<int> clausuraTransitiva(Grafo grafo){ ////////////////////////
    vector<int> valores;
    vector<int> resultado;
    if(!esGrafo(grafo,valores)) return resultado;
    for(int i = 0; i < valores.size(); i++){
        vector<int> valTemp = valores;
        valTemp.erase(valTemp.begin() + i);
        bool flag = true;
        for(int j = 0; j < valTemp.size(); j++){
            vector<int> visitado;
            if(!hayCamino(grafo,j,i,visitado)){
                flag = false;
                break;
            }
        }
        if(flag)resultado.push_back(i);
    }
    return resultado;
}



void printG(Grafo grafo){
    vector<int> valores;
    if(!esGrafo(grafo,valores))return;
    ofstream archivo("grafo.dot");
    if(archivo.fail()){
        cout<<"El archivo no se pudo abrir"<<endl;
        return;
    }
    archivo<<"digraph{"<<endl;
    for(auto iter = grafo.begin(); iter != grafo.end(); ++iter){
        archivo<<iter->first<<endl;
        for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
            archivo<<iter->first<<"->"<<iter2->first<<" [label=\""<<iter2->second<<"\"];"<<endl;
        }
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf grafo.dot -o grafo.pdf");
}

void caminosCortosAPartirDeUnOrigen(Grafo &grafo, int a){ /////////////////////////
    vector<int> valores;
    if(!esGrafo(grafo,valores))return;
    for(int i = 0; i < valores.size(); i++){
        if(valores[i] != a){
            vector<int> visitado;
            bool flag = false;
            int menor = caminoMinimo(grafo,a,i,visitado,flag);
            if(flag) grafo[a][valores[i]] = menor;
        }
    }
}




vector<tuple<int,int>> caminosPares(Grafo grafo){ ///////////////////////////
    vector<int> valores;
    vector<tuple<int,int>> resultado;
    if(!esGrafo(grafo,valores))return resultado;
    for(int i = 0; i < valores.size(); i++){
        vector<int> res;
        vector<int> visitado;
        _caminosPares(grafo,valores[i],visitado,res,0);
        for(int j = 0; j < res.size(); j++){
            resultado.push_back(make_tuple(valores[i],res[j]));
        }
    }
    return resultado;
}

vector<tuple<int,int>> _pareamiento(Grafo grafo, int a, vector<int> valores){ ////////////////
    vector<tuple<int,int>> resultado;
    if(valores.empty())return resultado;
    int mayor = 0;
    int actual = find(valores, a);
    valores.erase(valores.begin() + actual);
    for(auto iter = grafo[a].begin(); iter != grafo[a].end(); ++iter){
        int temp = find(valores, iter->first);
        vector<tuple<int,int>> v;
        if(temp != -1){
            valores.erase(valores.begin() + temp);
            v = _pareamiento(grafo,valores.front(),valores);
            v.push_back(make_tuple(a,iter->first));
            if(mayor < v.size() and v.size() != 0){
                mayor = v.size();
                resultado = v;
            }
        }
    }
    auto t = _pareamiento(grafo,valores.front(),valores);
    resultado.insert(resultado.end(),t.begin(),t.end());
    return resultado;
}

int pareamiento(Grafo grafo, vector<tuple<int,int>>& resultado){ ///////////////////
    vector<int> valores;
    int mayor = 0;
    if(!esGrafo(grafo,valores))return 0;
    for(int i = 0; i < valores.size(); i++){
        vector<tuple<int,int>> temp = _pareamiento(grafo,valores[i],valores);
        if(mayor <  temp.size() and temp.size() != 0){
            mayor = temp.size();
            resultado = temp;
        }
    }
    resultado = filtreRepeat(resultado);
    return resultado.size();
}

void arbolDeEpansionMinima(Grafo &grafo){ ///////////////////
    vector<int> valores;
    if(!esGrafo(grafo,valores))return;
    for(int i = 0; i < valores.size(); i++){
        for(int j = 0; j < valores.size(); j++){
            if(i != j){
                vector<int> visitado;
                bool flag = false;
                int menor = caminoMinimo(grafo,i,j,visitado,flag);
                if(flag) grafo[i][j] = menor;
            }
        }
    }
}

vector<tuple<int,int,int>> _asignacion(Grafo grafo, int a, vector<int> valores){ ////////////////7
    vector<tuple<int,int,int>> resultado;
    if(valores.empty())return resultado;
    int actual = find(valores, a);
    valores.erase(valores.begin() + actual);
    int index = minRelacion(grafo[a],a);
    int temp = find(valores, index);
    map<int,float> tt = grafo[a];
    while(!tt.empty() and temp == -1){
        tt.erase(index);
        index = minRelacion(tt,a);
        temp = find(valores, index);
    }
    vector<tuple<int,int,int>> v;
    if(!tt.empty()){
        valores.erase(valores.begin() + temp);
        v = _asignacion(grafo,valores.front(),valores);
        v.push_back(make_tuple(a,index,grafo[a][index]));
        resultado.insert(resultado.end(),v.begin(),v.end());
    }
    return resultado;
}

bool verificarColoracion(GrafoColoreado grafo, int a){ /////////////////
    int color = get<0>(grafo[a]);
    for(auto iter = get<1>(grafo[a]).begin(); iter != get<1>(grafo[a]).end(); ++iter){
        if(get<0>(grafo[iter->first]) == color)return false;
    }
    return true;
}

bool coloracion(Grafo grafo){ ////////////////
    vector<int> valores;
    if(!esGrafo(grafo,valores))return false;
    GrafoColoreado grafoColoreado;
    for(int i = 0; i < valores.size(); i++){
        grafoColoreado[valores[i]] = make_tuple(0,grafo[valores[i]]);
    }
    for(int i = 0; i < valores.size(); i++){
        if(get<0>(grafoColoreado[valores[i]]) == 0)get<0>(grafoColoreado[valores[i]]) = 1;
        if(!verificarColoracion(grafoColoreado,valores[i]))return false;
        int color = 0;
        if(get<0>(grafoColoreado[valores[i]]) == 1) color = 2;
        else color = 1;
        for(auto iter = get<1>(grafoColoreado[valores[i]]).begin(); iter != get<1>(grafoColoreado[valores[i]]).end(); ++iter){
            get<0>(grafoColoreado[iter->first]) = color;
        }
    }
    return true;
}

bool _caminoHamilton(Grafo grafo, int a, int b, vector<int> restantes){
    if(find(restantes.begin(),restantes.end(),a) == restantes.end())return false;
    if(a == b and restantes.empty())return true;
    restantes.erase(restantes.begin() + find(restantes,a));
    for(auto iter = grafo[a].begin(); iter != grafo[a].end(); ++iter){
        if(_caminoHamilton(grafo,iter->first,b,restantes))return true;
    }
    return false;
}

bool caminoHamilton(Grafo grafo, int a, int b){
    vector<int> valores;
    if(!esGrafo(grafo,valores))return false;
    return _caminoHamilton(grafo,a,b,valores);
}

void relaciones(Grafo grafo, vector<tuple<int,int>> &resultado){
    for(auto iter = grafo.begin(); iter != grafo.end(); ++iter){
        for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
            resultado.push_back(make_tuple(iter->first,iter2->first));
        }
    }
}

bool _caminoEuler(Grafo grafo,int x, int a, int b, vector<tuple<int,int>> restantes){
    auto temp = make_tuple(x,a);
    if(x != -1 and find(restantes.begin(),restantes.end(),temp) == restantes.end())return false;
    restantes.erase(restantes.begin() + find(restantes,temp));
    for(auto iter = grafo[a].begin(); iter != grafo[a].end(); ++iter){
        if(_caminoEuler(grafo,a,iter->first,b,restantes))return true;
    }
    return false;
}

bool caminoEuler(Grafo grafo, int a, int b){
    vector<int> valores;
    if(!esGrafo(grafo,valores))return false;
    vector<tuple<int,int>> rel;
    relaciones(grafo,rel);
    return _caminoEuler(grafo,-1,a,b,rel);
}

int asignacion(Grafo grafo, vector<tuple<int,int,int>>& resultado){ ////////////////
    vector<int> valores;
    int menor = 10000;
    if(!esGrafo(grafo,valores))return 0;
    for(int i = 0; i < valores.size(); i++){
        vector<tuple<int,int,int>> v = _asignacion(grafo,valores.front(),valores);
        int suma = 0;
        for(auto iter = v.begin(); iter != v.end(); ++iter){
            suma += get<2>(*iter);
        }
        if(menor > suma){
            menor = suma;
            resultado = v;
        }
    }
    resultado = filtreRepeat(resultado);
    return resultado.size();
}


int main()
{
    Grafo g;
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
    cout<<caminoEuler(g,0,4)<<endl;
    printG(g);

}
