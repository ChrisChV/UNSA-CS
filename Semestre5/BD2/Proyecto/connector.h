#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include <stdio.h>
#include <mysql.h>

using namespace std;

MYSQL * getConnect(){
	MYSQL * connect;
	connect = mysql_init(NULL);
	if(!connect){
		string e = "Error en la coneccion MYSQL (11)";
		throw(e);
	}
	connect = mysql_real_connect(connect, "localhost", "root", "root", "Google", 0,NULL,0);
	if(!connect){
		string e = "Error en la coneccion MYSQL (12)";
		throw(e);
	}
	return connect;
}

#endif