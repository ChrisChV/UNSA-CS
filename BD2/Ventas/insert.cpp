#include <iostream>
#include <cstdlib>
#include <time.h>
#include "../connector.h"
#include <vector>
#include <thread>

using namespace std;

void insertPais(MYSQL * connect){
	mysql_query(connect, "insert into Pais values(1,'Chile');");
	mysql_query(connect, "insert into Pais values(2,'Peru');");
	mysql_query(connect, "insert into Pais values(3,'Colombia');");
	cout<<"paises terminados"<<endl;
}

void insertProducto(MYSQL * connect){
	for(int i = 0; i < 500; i++){
		string prod = "Producto" + to_string(i+1);
		string query = "insert into Producto values(" + to_string(i+1) + ",'" + prod + "');";
		mysql_query(connect,query.c_str());
	}
	cout<<"productos terminados"<<endl;
}
void insertCliente(MYSQL * connect){
	for(int i = 0; i < 300; i++){
		string cli = "Cliente" + to_string(i+1);
		string query = "insert into Cliente values(" + to_string(i+1) + ",'"
				 + cli + "'," + to_string(rand()%3+1) + ");";
		mysql_query(connect,query.c_str());
	}
	cout<<"paises cliente"<<endl;
}

void insertVentas(string *r,long ini, long fin){
	string query = " ";
	for(long i = ini; i < fin; i++){
		int prod = rand()%500 + 1;
		int cli = rand()%300 + 1;
		int cant = rand()%1000 + 1;
		int month = rand()%12 + 1;
		int day = 0;
		if(month == 2){
			day = rand()%28 + 1;	
		}
		else day = rand()%30 + 1; 
		int tt = rand()%2;
		int year = 2015;
		if(tt == 1) year = 2016;
		string gg = "(" + to_string(i+1) + "," + to_string(cli) + "," + to_string(prod) + ","
			+ to_string(cant) + ",'" + to_string(year) + "-" + to_string(month)
				+ "-" + to_string(day) + "'),";
		query = query + gg;
		/*string query = "insert into Ventas values(" + to_string(i+1) + "," + to_string(cli) 
		   + "," + to_string(prod) + "," + to_string(cant) + ",'" + to_string(year) + "-"
		   		 + to_string(month) + "-" + to_string(day) + "');";
		   		*/
		
		//cout<<"I->"<<i<<endl;
	}
	*r = query;
}

int main(){
	srand(time(NULL));
	clock_t start;
	clock_t end;
	double timeUsed = 0;
	start = clock();
	MYSQL * connect = getConnect();
	long ii = 100400000;
	for(; ii < 101400000; ii = ii + 10000){
		vector<string *> res;
		for(int i = 0; i < 100; i++) res.push_back(new string);
		vector<thread> tt;
		long ini = ii;
		long fin  = ini + 100;
		for(int i = 0; i < 100; i++){
			tt.push_back(thread(insertVentas,res[i],ini,fin));
			ini = fin;
			fin += 100;	
		}
		for(auto iter = tt.begin(); iter != tt.end(); ++iter){
			(*iter).join();
		}
		string query = "insert into ventas values ";
		for(int i = 0; i < 50; i++){
			if(i == 99) res[i]->pop_back();
			query = query + (*(res[i]));
		}
		query.push_back(';');
	//insertPais(connect);
	//insertCliente(connect);
		mysql_query(connect,query.c_str());	
		cout<<ii<<endl;
		for(string * ss : res) delete(ss);
	}
	end = clock();
	mysql_close(connect);
	timeUsed = ((double)(end - start)) / CLOCKS_PER_SEC;
	cout<<"Time->"<<timeUsed<<endl;
}


