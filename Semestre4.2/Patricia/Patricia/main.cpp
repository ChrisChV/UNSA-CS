#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include "vector"

using namespace std;

class AutomataAdaptativo{
  public:
    class NodoP{
      public:
        NodoP(){
          hijosBin[0] = -1;
          hijosBin[1] = -1;
          medio = -1;
          first = true;
          terminal = true;
          dir = -1;
        }
        NodoP(string s){
          hijosBin[0] = -1;
          hijosBin[1] = -1;
          medio = -1;
          first = true;
          terminal = true;
          data = s;
          dir = -1;
        }
        void imprimir(){
            cout<<"hijo 0->"<<hijosBin[0]<<endl;
            cout<<"hijo 0->"<<hijosBin[1]<<endl;
            cout<<"medio ->"<<medio<<endl;
            cout<<"first ->"<<first<<endl;
            cout<<"terminal ->"<<terminal<<endl;
            cout<<"dir ->"<<dir<<endl;
            cout<<"data ->"<<data<<endl;
        }
        long hijosBin[2];
        long medio;
        bool first;
        bool terminal;
        long dir;
        string data;
        vector<string> respuestas;
    };

    void saveData(){
        ofstream ar("arbplP/principal.dot", ios::binary);
        ar.write(reinterpret_cast<char*>(&root),sizeof(long));
        ar.write(reinterpret_cast<char*>(&_siz),sizeof(int));
        ar.write(reinterpret_cast<char*>(&_siz2),sizeof(int));
    }

    void loadData(){
        ifstream ar("arbplP/principal.dot", ios::binary);
        ar.read(reinterpret_cast<char*>(&root),sizeof(long));
        ar.read(reinterpret_cast<char*>(&_siz),sizeof(int));
        ar.read(reinterpret_cast<char*>(&_siz2),sizeof(int));
    }

    AutomataAdaptativo(bool flag){
    if(flag){
        root = -1;
        system("rm -R arbplP/");
        system("mkdir arbplP");
        _siz2 = 0;
        _siz = 0;
    }
    else{
        loadData();
    }
    }

    long write(NodoP &n)
        {
            if(n.dir == -1){
                n.dir = _siz2;
                _siz2++;
            }
            string file = "arbplP/" + to_string(n.dir) + ".dat";
            ofstream f(file, ios::binary);
            //n.imprimir();
            //char y;
            //cin>>y;
            f.write(reinterpret_cast<char*>(&(n.hijosBin[0])),sizeof(long));
            f.write(reinterpret_cast<char*>(&(n.hijosBin[1])),sizeof(long));
            f.write(reinterpret_cast<char*>(&(n.medio)),sizeof(long));
            f.write(reinterpret_cast<char*>(&(n.dir)),sizeof(long));
            f.write(reinterpret_cast<char*>(&(n.terminal)),sizeof(bool));
            int t = n.data.size();
            f.write(reinterpret_cast<char*>(&t),sizeof(int));
            for(int i = 0; i < t; i++){
                char a = n.data[i];
                f.write(reinterpret_cast<char*>(&a),sizeof(char));
            }
                int x = n.respuestas.size();
                f.write(reinterpret_cast<char*>(&x),sizeof(int));
                for(int i = 0; i < x; i++){
                    int l = n.respuestas[i].size();
                    f.write(reinterpret_cast<char*>(&l),sizeof(int));
                    for(int j = 0; j < l; j++){
                        char a = n.respuestas[i][j];
                        f.write(reinterpret_cast<char*>(&a),sizeof(char));
                    }
                }
            return n.dir;
        }

      NodoP read(long dir)
        {
            NodoP n;
            if(dir == -1) return n;
            string file = "arbplP/" + to_string(dir) + ".dat";
            ifstream f(file, ios::binary);
            f.read(reinterpret_cast<char*>(&(n.hijosBin[0])),sizeof(long));
            f.read(reinterpret_cast<char*>(&(n.hijosBin[1])),sizeof(long));
            f.read(reinterpret_cast<char*>(&(n.medio)),sizeof(long));
            f.read(reinterpret_cast<char*>(&(n.dir)),sizeof(long));
            f.read(reinterpret_cast<char*>(&(n.terminal)),sizeof(bool));
            int t;
            f.read(reinterpret_cast<char*>(&t),sizeof(int));
            string d;
            for(int i = 0; i < t; i++){
                char c;
                f.read(reinterpret_cast<char*>(&c),sizeof(char));
                d.insert(d.end(),c);
            }
            n.data = d;
                int x;
                f.read(reinterpret_cast<char*>(&x),sizeof(int));
                for(int i = 0; i < x; i++){
                    int l;
                    string s;
                    f.read(reinterpret_cast<char*>(&l),sizeof(int));
                    for(int j = 0; j < l; j++){
                        char a;
                        f.read(reinterpret_cast<char*>(&a),sizeof(char));
                        s.insert(s.end(),a);
                    }
                    n.respuestas.push_back(s);
                }
            return n;
        }
    virtual ~AutomataAdaptativo(){
      f.close();
    }
    // -1 las cadenas no tienen nada en comun
    // 0 las cadenas tienen algo en comun y las dos tienen algo sArbolRestante
    // 1 las cadenas son iguales
    // 2 el sArbol esta contenido en sNArbol ejemplo-> palabra palabrando
    // 3 el sNArbol esta contenido en sArbol ejemplo-> palabrando palabra
    int compare(string &sArbol, string &sNArbol, string &igual, string &sArbolRestante, string &sNArbolRestante){
        if(sArbol == "abra" and sNArbol == "a"){
            int i = 0;
            cout<<i<<endl;
        }
        int i = 0;
        int flag = 1;
        for(i = 0; i < sArbol.size(); i++){
          if(i == sNArbol.size()){
            flag = -1;
            break;
          }
          if(sArbol[i] != sNArbol[i]){
            if(i == 0) return -1;
            flag = 0;
            break;
          }
          igual.insert(igual.end(),sArbol[i]);
        }
        if(flag and i == sNArbol.size() and sNArbol.size() == sArbol.size()) return 1;
        sArbolRestante = sArbol;
        int a = i;
        sArbolRestante.erase(sArbolRestante.begin(),sArbolRestante.begin() + i);
        sNArbolRestante = sNArbol;
        sNArbolRestante.erase(sNArbolRestante.begin(),sNArbolRestante.begin() + i);
        if(flag and i != sNArbol.size()) return 2;
        if(flag == -1) return 3;
        return 0;
    }

    void printN(){
        ofstream archivo("file.dot");
        deque<long> c;
        if(root != -1) c.push_back(root);
        while(!c.empty()){
            NodoP t = read(c.front());
            c.pop_front();
            t.imprimir();
            if(t.hijosBin[0] != -1){
                c.push_back(t.hijosBin[0]);
            }
            if(t.medio != -1){
                c.push_back(t.medio);
            }
            if(t.hijosBin[1] != -1) {
                c.push_back(t.hijosBin[1]);
            }
        }
    }


    void print(){
      ofstream archivo("file.dot");
      deque<long> c;
      if(root != -1) c.push_back(root);
      archivo<<"digraph{ graph [ splines=polyline]";
      while(!c.empty()){
        NodoP t = read(c.front());
        c.pop_front();
        string color = "red";
        if(!t.terminal) color = "black";
        archivo<<t.dir<<" [label = \"" + t.data +  "\" color =\"" + color + "\"];"<<endl;
        if(t.hijosBin[0] != -1){
          archivo<<t.dir<<":nw ->"<<t.hijosBin[0]<<":sw [  color = \"green\"];"<<endl;
          c.push_back(t.hijosBin[0]);
        }
        if(t.medio != -1){
          cout<<"holaaaaaaaaaaa"<<endl;
          archivo<<t.dir<<":se->"<<t.medio<<":sw [ color = \"blue\"];"<<endl;
          c.push_back(t.medio);
        }
        if(t.hijosBin[1] != -1) {
          archivo<<t.dir<<" :ne->"<<t.hijosBin[1]<<":se  [ color = \"green\"];"<<endl;
          c.push_back(t.hijosBin[1]);
        }
      }
      archivo<<"}";
      archivo.close();
      system("dot -Tpdf file.dot -o file.pdf");
    }

    bool search(string cadena, long &actualDir){
        if(root == -1) return false;
        if(insert(cadena,read(root),actualDir,false) == -5) return true;
        return false;
    }

    void insert(string sArbol){
        if(root == -1){
            NodoP temp(sArbol);
            write(temp);
            root = temp.dir;
            return;
        }
        long actualDir;
        insert(sArbol,read(root),actualDir,true);
        saveData();
    }

    long insert(string sArbol,NodoP t, long &actualDir, bool flag){
      string igual;
      string sNArbolRestante;
      string sArbolRestante;
      t.imprimir();
      if(t.dir == -1)
      {
          if(flag){
                NodoP temp(sArbol);
                return write(temp);
          }
          else return -4;
      }
      int maybe = compare(t.data,sArbol,igual,sArbolRestante,sNArbolRestante);
        if(maybe==-1){
          if(t.data[0] < sArbol[0]){
            long dt = insert(sArbol,read(t.hijosBin[1]),actualDir,flag);
            if(dt == -2) return -2;
            if(dt == -4){
                actualDir = t.dir;
                return -6;
            }
            if(dt == -5) return -5;
            if(dt == -6) return -6;
            if(dt != -3){
                t.hijosBin[1] = dt;
                write(t);
            }
            //NodoP p = read(t.hijosBin[1]);
            //p.terminal = true;
            //write(p);
          }
          else {
              long dt = insert(sArbol,read(t.hijosBin[0]),actualDir,flag);
              if(dt == -2) return -2;
              if(dt == -4){


                return -6;
                }
            if(dt == -5) return -5;
            if(dt == -6) return -6;
              if(dt != -3){
                t.hijosBin[0] = dt;
                write(t);
              }
            //NodoP p = read(t.hijosBin[0]);
            //p.terminal = true;
            //write(p);
          }
        }
        else if(maybe == 0)
        {

          if(t.medio == -1){
                long dt = insert(sArbolRestante,read(t.medio),actualDir,flag);
                if(dt == -2) return -2;
                if(dt == -4){
                    actualDir = t.dir;
                    return -6;
                }
                if(dt == -5) return -5;
                if(dt == -6) return -6;
                t.medio = dt;
                write(t);
            }
          else{
            NodoP tm(sArbolRestante);
            tm.medio = t.medio;
            t.medio = write(tm);
            write(t);
          }
          long dt2 = insert(sNArbolRestante,read(t.medio),actualDir,flag) == -2;
          if(dt2 == -2) return -2;
          if(dt2 == -4){
               actualDir = t.dir;
               return -6;
            }
            if(dt2 == -5) return -5;
            if(dt2 == -6) return -6;
            t.data = igual;
          t.terminal = false;
          write(t);
          //NodoP p = read(t.hijosBin[1]);
          //p.terminal = true;
          //write(p);
          //NodoP m = read(t.medio);
          //m.terminal = true;
          //write(m);
        }
        else if(maybe==1) {
            if(t.terminal == false){
                t.terminal = true;
                write(t);
            }
            else{
                actualDir = t.dir;
                return -5;
            }
        }
        else if(maybe==2){
          //NodoP p = read(t.hijosBin[1]);
          //p.terminal = true;
          //write(p);
          long dt = insert(sNArbolRestante,read(t.medio),actualDir,flag);
          if(dt == -2) return -2;
          if(dt == -4){

               actualDir = t.dir;

               return -6;
            }
            if(dt == -5) return -5;
            if(dt == -6) return -6;
          if(dt != -3){
                t.medio = dt;
                write(t);
          }
          //NodoP m = read(t.medio);
          //m.terminal = true;
          //write(m);
        }
        else if(maybe==3){
          //NodoP p = read(t.hijosBin[1]);
          //p.terminal = true;
          //write(p);
          long dt = insert(sArbolRestante,read(t.medio),actualDir,flag);
          if(dt == -2) return -2;
          if(dt == -4){

               actualDir = t.dir;

               return -6;
            }
            if(dt == -5) return -5;
            if(dt == -6) return -6;
          if(dt != -3){
            t.data = sArbol;
            t.medio = dt;
            write(t);
        }
          //NodoP m = read(t.medio);
          //m.terminal = true;
          //write(m);
        }
        _siz++;
        return -3;
      }
    string hablarOaprender(string caracter,bool flag){
        if(flag){
            insert(caracter);
            return "Aprendido Correctamente";
        }
        else{
            long actual;
            cout<<search(caracter,actual)<<endl;
            return "rrr";
        }
    }

  private:
    long root;
    int _siz;
    int _siz2;
    fstream f;
};


int main(){
  AutomataAdaptativo arbolito(true);
  char c = '2';
  while(c != '1'){
    string cadena;
    cout<<"Aprende o habla"<<endl;
    cin>>c;
    if(c == 'a'){
        cin>>cadena;
        cout<<arbolito.hablarOaprender(cadena,true)<<endl;
        cout<<cadena<<endl;
    }
    else if (c== 'h'){
        cin>>cadena;
        cout<<arbolito.hablarOaprender(cadena,false)<<endl;
    }
  }
  arbolito.print();
}



