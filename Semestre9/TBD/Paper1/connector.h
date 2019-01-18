#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include <stdio.h>
#include <mysql.h>

using namespace std;

typedef string QueryType; 

MYSQL * getConnect(string bd){
	MYSQL * connect;
	connect = mysql_init(NULL);
	if(!connect){
		string e = "Error en la coneccion MYSQL (11)";
		throw(e);
	}
	connect = mysql_real_connect(connect, "localhost", "root", "root", bd.c_str(), 0,NULL,0);
	if(!connect){
		string e = "Error en la coneccion MYSQL (12)";
		throw(e);
	}
	return connect;
}

void closeConnect(MYSQL * conn){
	mysql_close(conn);
}

MYSQL_RES * getQueryRes(MYSQL * conn, QueryType query){
	if(mysql_query(conn, query.c_str()) != 0){
		cout<<"Error en la query: "<<query<<endl;
		return NULL;
	}
	return mysql_store_result(conn);
}

void getQuery(MYSQL * conn, QueryType query){
	if(mysql_query(conn, query.c_str()) != 0){
		cout<<"Error en la query: "<<query<<endl;
	}
}


#endif