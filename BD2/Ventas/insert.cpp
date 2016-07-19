#include <iostream>
#include <cstdlib>
#include <time.h>
#include "../connector.h"
#include <vector>
#include <thread>

using namespace std;

void insertPais(MYSQL * connect){
	mysql_query(connect, "insert into pais values(1,'Chile');");
	mysql_query(connect, "insert into pais values(2,'Peru');");
	mysql_query(connect, "insert into pais values(3,'Colombia');");
}

void insertProducto(MYSQL * connect){
	for(int i = 0; i < 500; i++){
		string prod = "Producto" + to_string(i+1);
		string query = "insert into producto values(" + to_string(i+1) + ",'" + prod + "');";
		mysql_query(connect,query.c_str());
	}
}
void insertCliente(MYSQL * connect){
	for(int i = 0; i < 300; i++){
		string cli = "Cliente" + to_string(i+1);
		string query = "insert into clientes values(" + to_string(i+1) + ",'"
				 + cli + "'," + to_string(rand()%3+1) + ");";
		mysql_query(connect,query.c_str());
	}
}

void insertVentas(string *r,long ini, long fin){
	string query = " ";
	for(long i = ini; i < fin; i++){
		int prod = rand()%500 + 1;
		int cli = rand()%300 + 1;
		int cant = rand()%1000 + 1;
		int month = rand()%12 + 1;
		int day = 0;
		int tt = rand()%2;
		int year = 2015;
		if(tt == 1) year = 2016;
		if(month == 2){
			if(year == 2016) day = rand()%29 + 1;
			else day = rand()%28 + 1;	
		}
		else day = rand()%30 + 1; 
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
	insertPais(connect);
	insertCliente(connect);
	insertProducto(connect);
	long ii = 90080000;
	int numThreads = 100;
	for(; ii <= 100000000; ii = ii + 25000){
		vector<string *> res;
		for(int i = 0; i < numThreads; i++) res.push_back(new string);
		vector<thread> tt;
		long ini = ii;
		long fin  = ini + 250;
		for(int i = 0; i < numThreads; i++){
			tt.push_back(thread(insertVentas,res[i],ini,fin));
			ini = fin;
			fin += 250;	
		}
		for(auto iter = tt.begin(); iter != tt.end(); ++iter){
			(*iter).join();
		}
		string query = "insert into ventas values ";
		for(int i = 0; i < numThreads; i++){
			if(i == numThreads-1) res[i]->pop_back();
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