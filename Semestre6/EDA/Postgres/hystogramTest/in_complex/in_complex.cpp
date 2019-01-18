#include <iostream>
#include <fstream>
#include <string>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]){
	
	string img;
	string path = string(argv[1]);
	string db = string(argv[3]);
	string user = string(argv[4]);
	string pass = "";
	if(string(argv[5]) != "null"){
		pass = string(argv[5]);
	}
	string table = string(argv[6]);
	string sql;		

	connection C("dbname="+db+" user="+user+" password="+pass+" hostaddr=127.0.0.1 port=5432");
	if (C.is_open()) {
		cout << "Opened database successfully: " << C.dbname() << endl;
	} else {
		cout << "Can't open database" << endl;
		return 1;
	}
	
	ifstream infile(argv[2]);

    int limit = 1000, count = 0;
	if (infile.is_open())
	{
		while ( getline (infile, img) )
		{
			if(count++ % limit == 0){
				sql = "INSERT INTO " + table + " VALUES(DEFAULT, '" + path + img + "', '" + path + img + "')";
			}else{
				sql += ",(DEFAULT, '" + path + img + "', '" + path + img + "')";	
			}
			if(count % limit == 0){
				sql += ";";
				work W(C);
				W.exec(sql);
				W.commit();
				cout << count << " Records created successfully" << endl;
			}
		}
		sql += ";";
		work W(C);
		W.exec(sql);
		W.commit();
		infile.close();
	}
	cout << "All Records created successfully" << endl;
	C.disconnect();
	return 0;
}

// g++ -I /usr/local/include/pqxx/ -L /usr/local/lib/ -I /usr/local/pgsql/include/ -L /usr/local/pgsql/lib/ in_complex.cpp -lpqxx -lpq

