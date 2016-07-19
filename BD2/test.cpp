#include <iostream>
#include <thread>
#include <vector>
#include "connector.h"

using namespace std;


int main(){
	MYSQL * connect = getConnect();
	int dia = 1;
	int mes = 1;
	int year = 2015;
	while(year != 2016){
		string query = "insert into Dias values('" + to_string(year) + "-" + to_string(mes)
						+ "-" + to_string(dia) + "');";
		mysql_query(connect,query.c_str());
		dia++;
		if(dia == 29 and mes == 2){
			dia = 1;
			mes = 3;
		}
		else if(dia == 31 and (mes == 4 or mes == 6 or mes == 9 or mes == 11)){
			dia = 1;
			mes++;
		}
		else if(dia == 32){
			dia = 1;
			mes++;
		}
		if(mes == 13) year = 2016;
	}
	mysql_close(connect);
}