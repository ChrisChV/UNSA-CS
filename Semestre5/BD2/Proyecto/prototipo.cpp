#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <ctype.h>
#include <map>
#include "connector.h"


using namespace std;

bool isNumber(string &s){
	for(auto iter = s.begin(); iter != s.end(); ++iter){
		if(!isdigit(*iter)) return false;
	}
	return true;
}

bool s_find(string &s, string &c){
	for(auto iter = s.begin(); iter != s.end(); ++iter){
		for(auto iter2 = c.begin(); iter2 != c.end(); ++iter2){
			if(*iter == *iter2) return true;
		}
	}
	return false;
}

void replace(string &s, char a, string b) {
	for(int i = 0; i < s.size(); i++){
		if(s[i] == a){
			s.replace(i,1,b);
			i += b.size() - 1;
		}
	}
    /*size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
    */
}


string ToLower(string word){
	transform(word.begin(), word.end(), word.begin(), ::tolower);
	return word;
}

void llenarIDF(){
	MYSQL * connect = getConnect();
	/*string query = "select distinct idWord1 from Relations";

	mysql_query(connect,query.c_str());
	res_set = mysql_store_result(connect);
	*/
	MYSQL_RES * res_set;
	int N = 96464;
	string query2 = "select idWord2, count(idWord2) from Relations group by idWord2;";
	mysql_query(connect,query2.c_str());
	res_set = mysql_store_result(connect);
	int num = mysql_num_rows(res_set);
	MYSQL_ROW row;
	int n = 0; //52496
	for(int j = 0; j < n; j++){
		row = mysql_fetch_row(res_set);	
	}
	for(int i = n; i < num; i++){
		cout<<"NIDF->"<<i<<endl;
		row = mysql_fetch_row(res_set);
		double idf = log(N/stof(row[1]));
		string query3 = "update Relations set IDF = " + to_string(idf) + " where idWord2 = " + row[0] + ";";
		mysql_query(connect,query3.c_str());
	}	
	mysql_free_result(res_set);
	mysql_close(connect);
}




int hallarMayor(int idWord){
	MYSQL * connect = getConnect();
	string query = "select max(F) from Relations where idWord1 = " + to_string(idWord);
	MYSQL_RES * res_set;
	MYSQL_ROW row;
	mysql_query(connect,query.c_str());
	res_set = mysql_store_result(connect);
	row = mysql_fetch_row(res_set);
	mysql_free_result(res_set);
	mysql_close(connect);
	return stoi(row[0]);
}

void llenarTF(){
	MYSQL * connect = getConnect();
	string query = "select distinct idWord1 from Relations";
	MYSQL_RES * res_set;
	MYSQL_ROW row;
	mysql_query(connect,query.c_str());
	res_set = mysql_store_result(connect);
	int num = mysql_num_rows(res_set);
	int n = 0;
	for(int j = 0; j < n; j++){
		row = mysql_fetch_row(res_set);	
	}
	for(int i = n; i < num; i++){
		cout<<"NTF->"<<i<<endl;
		row = mysql_fetch_row(res_set);
		int id = stoi(row[0]);
		int mayor = hallarMayor(id);
		string query2 = "update Relations set TF = F / " + to_string(mayor) + " where idWord1 = " + to_string(id) + ";";
		mysql_query(connect,query2.c_str());
	}
	mysql_free_result(res_set);
	mysql_close(connect);
}

void insertRelation(int word1, int word2, int F){
	MYSQL * connect = getConnect();
	string query = "select idWord1,idWord2 from Relations where idWord1 = " + to_string(word1)
					+ " and idWord2 = "	+ to_string(word2) + ";";
	mysql_query(connect, query.c_str());
	MYSQL_RES * res_set;
	res_set = mysql_store_result(connect);
	if(mysql_num_rows(res_set) == 0){
		string query2 = "insert into Relations(idWord1,idWord2,F) values (" + to_string(word1)
						+ "," + to_string(word2) + "," + to_string(F) + ");";
		mysql_query(connect,query2.c_str());
	}
	else{
		string query2 = "update Relations set F = F + " + to_string(F) + " where idWord1 = " + to_string(word1)
						+ " and idWord2 = " + to_string(word2) + ";";
		mysql_query(connect,query2.c_str());
	}
	mysql_free_result(res_set);
	mysql_close(connect);

}

int getWordId(string word){
	MYSQL * connect = getConnect();
	string query = "select idWord from Word where Word ='" + word + "';";
	mysql_query(connect,query.c_str());
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	res_set = mysql_store_result(connect);
	if(mysql_num_rows(res_set) == 0){

		string query2 = "insert into Word(Word) values ('" + word + "');";

		mysql_query(connect,query2.c_str());
		mysql_query(connect,query.c_str());
		res_set = mysql_store_result(connect);
	}
	
	row = mysql_fetch_row(res_set);
	mysql_free_result(res_set);
	mysql_close(connect);
	return stoi(row[0]);
}

void insertWords(){
	ifstream palabras("palabras.txt");
	string word;
	MYSQL * connect = getConnect();
	while(palabras>>word){
		string query = "insert into Word(Word) values ('" + word + "');";
		mysql_query(connect,query.c_str());
	}
	mysql_close(connect);
}

void llenarF(){
	ofstream res("res.txt");
	for(int n = 24; n < 32; n++){
		int total = 0;
		res<<n<<endl;
		cout<<n<<endl;
		string file = "Resultados2/000" + to_string(n);
		ifstream archivo(file.c_str());
		string word1;
		string word2;
		int F;
		while(archivo>>word1 and archivo>>word2 and archivo>>F){
			total += 1;
			cout<<total<<endl;
			word1 = ToLower(word1);
			word2 = ToLower(word2);
			ifstream palabras("palabras.txt");
			string word;
			while(palabras>>word){
				if(word == word1){
					replace(word2, '\\', "\\\\");
					replace(word2, '\'',"\\'");					
					int id1 = getWordId(word1);
					int id2 = getWordId(word2);
					insertRelation(id1,id2,F);
				}
				else if(word == word2){
					replace(word1, '\\', "\\\\");
					replace(word1, '\'',"\\'");
					int id1 = getWordId(word2);
					int id2 = getWordId(word1);
					insertRelation(id1,id2,F);
				}
			}
			palabras.close();
		}
		archivo.close();
	}
}

void filtroCaracteresEspeciales(){
	ifstream specialCaracter("specialCar.txt");
	string c;
	specialCaracter>>c;
	specialCaracter.close();
	ofstream res("res.txt");
	for(int n = 0; n < 32; n++){
		int total = 0;
		res<<n<<endl;
		string file = "Resultados3/000" + to_string(n);
		string fi = "Resultados4/000" + to_string(n);
		ifstream archivo(file.c_str());
		ofstream result(fi.c_str());
		string word1;
		string word2;
		int F;
		while(archivo>>word1 and archivo>>word2 and archivo>>F){
			cout<<total<<endl;
			total++;
			if(!s_find(word1,c) and !s_find(word2,c)){
				result<<word1<<" "<<word2<<"	"<<F<<endl;
			}
		}
		string t = "Resultados4/total000" + to_string(n);
		ofstream tt(t.c_str());
		tt<<total<<endl;
		tt.close();
		archivo.close();
		result.close();
	}
	res.close();
}

void filtroNumbersyTolower(){
	ofstream res("res.txt");
	for(int n = 0; n < 32; n++){
		res<<n<<endl;
		int total = 0;
		string file = "Resultados4/000" + to_string(n);
		string fi = "Resultados5/000" + to_string(n);
		ifstream archivo(file.c_str());
		ofstream result(fi.c_str());
		string word1;
		string word2;
		int F;
		while(archivo>>word1 and archivo>>word2 and archivo>>F){
			cout<<total<<endl;
			total++;
			if(!isNumber(word1) or !isNumber(word2)){
				result<<ToLower(word1)<<" "<<ToLower(word2)<<"	"<<F<<endl;
			}
		}
		string t = "Resultados5/total000" + to_string(n);
		ofstream tt(t.c_str());
		tt<<total<<endl;
		tt.close();
		archivo.close();
		result.close();
	}
	res.close();
}

map<string,int> generarMap(){
	string query = "select * from Word";
	map<string,int> res;
	MYSQL * connect = getConnect();
	MYSQL_RES * res_set;
	MYSQL_ROW row;
	mysql_query(connect,query.c_str());
	res_set = mysql_store_result(connect);
	int n = mysql_num_rows(res_set);
	for(int i = 0; i < n; i++){
		cout<<"M->"<<i<<endl;
		row = mysql_fetch_row(res_set);
		res[row[1]] = stoi(row[0]);
	}
	mysql_free_result(res_set);
	mysql_close(connect);
	return res;
}

void insertGoogleWord(){
	ofstream res("res.txt");
	string plan1 = "insert ignore into Word(Word) values ";
	string query = plan1;
	MYSQL * connect = getConnect();
	int contador = 0;
	for(int n = 8; n < 32; n++){
		res<<n<<endl;
		int total = 0;
		string file = "Resultados7/000" + to_string(n);
		ifstream archivo(file.c_str());
		string word1;
		string word2;
		int F;
		while(archivo>>word1 and archivo>>word2 and archivo>>F){
			if(contador == 10000){
				contador = 0;
				query.pop_back();
				query += ";";
				mysql_query(connect,query.c_str());
				query = plan1;
			}
			replace(word1, '\\', "\\\\");
			replace(word1, '\'',"\\'");	
			replace(word2, '\\', "\\\\");
			replace(word2, '\'',"\\'");	
			cout<<total<<endl;
			total++;
			query += "('" + word1 + "'),('" + word2 + "'),";
			contador += 2;
		}
		archivo.close();
	}
	if(contador != 0){
		query.pop_back();
		query += ";";
		mysql_query(connect,query.c_str());
	}
	res.close();
	mysql_close(connect);
}



void insertGoogleRelations(){
	ofstream res("res.txt");
	int contador = 0;
	string plan1 = "insert into Relations(idWord1,idWord2,F) values ";
	string plan2 = " on duplicate key update F = F + values(F);";
	string query = plan1;
	MYSQL * connect = getConnect();
	auto Words = generarMap();
	for(int n = 0; n < 32; n++){
		res<<n<<endl;
		int total = 0;
		string file = "Resultados7/000" + to_string(n);
		ifstream archivo(file.c_str());
		string word1;
		string word2;
		int F;
		while(archivo>>word1 and archivo>>word2 and archivo>>F){
			if(contador == 10000){
				contador = 0;
				query.pop_back();
				query += plan2;
				mysql_query(connect,query.c_str());
				query = plan1;
			}
			cout<<total<<endl;
			total++;
			int idWord1 = Words[word1];
			int idWord2 = Words[word2];
			query += "(" + to_string(idWord1) + "," + to_string(idWord2) + "," + to_string(F) + "),";
			contador++; 
		}
		archivo.close();
	}
	if(contador != 0){
		query.pop_back();
		query += plan2;
		mysql_query(connect,query.c_str());
	}
	res.close();
	mysql_close(connect);
}


void eliminarNumeros(){
	ofstream res("res-txt");
	for(int n = 8; n < 32; n++){
		int total = 0;
		res<<n<<endl;
		string file = "Resultados5/000" + to_string(n);
		ifstream archivo(file.c_str());
		string fi = "Resultados6/000" + to_string(n);
		ofstream result(fi.c_str());
		string word1;
		string word2;
		int F;
		while(archivo>>word1 and archivo>>word2 and archivo>>F){
			if(!isNumber(word1) and !isNumber(word2)){
				total++;
				cout<<total<<endl;
				result<<word1<<" "<<word2<<"	"<<F<<endl;
			}
		}
		archivo.close();
		result.close();
		string fifi = "Resultados6/total000" + to_string(n);
		ofstream tt(fifi.c_str());
		tt<<total<<endl;
		tt.close();
	}
	res.close();
}

void generarDumbCSV(){
	ofstream archivo("google.csv");
	MYSQL * connect = getConnect();
	MYSQL_RES * res_set;
	MYSQL_ROW row;
	string query = "select * from Relations;";
	mysql_query(connect,query.c_str());
	res_set = mysql_store_result(connect);
	long n = mysql_num_rows(res_set);
	for(long i = 0; i < n; i++){
		row = mysql_fetch_row(res_set);
		archivo<<row[0]<<","<<row[1]<<","<<row[2]<<","<<row[3]<<","<<row[4]<<endl;
	}
	archivo.close();
	mysql_free_result(res_set);
	mysql_close(connect);
}

void llenarW(){
	MYSQL * connect = getConnect();
	MYSQL_RES * res_set;
	MYSQL_ROW row;
	string query = "select distinct idWord2 from Relations;";
	mysql_query(connect,query.c_str());
	res_set = mysql_store_result(connect);
	int num = mysql_num_rows(res_set);
	int n = 0;
	for(int i = 0; i < n; i++){
		row = mysql_fetch_row(res_set);
	}
	for(int i = n; i < num; i++){
		cout<<"WN->"<<i<<endl;
		row = mysql_fetch_row(res_set);
		string id = row[0];
		string query2 = "update Relations set W = TF * IDF where idWord2 = " + id + ";";
		mysql_query(connect,query2.c_str());
	}
	mysql_close(connect);
	mysql_free_result(res_set);
}

void generarVectores(){
	MYSQL * connect = getConnect();
	MYSQL_RES * res_set;
	MYSQL_ROW row;
	string query = "select * from vector where idWord1 = ";
	int num = 0;
	for(int i = 1; i <= num; i++){
		string qq = query;
		qq += to_string(i) + ";";
		mysql_query(connect,qq.c_str());
		res_set = mysql_store_result(connect);
		int n = mysql_num_rows(res_set);
		string result = "";
		for(int j = 0; j < n; j++){
			//result += row[2] + ",";
		}
	}
}


int main(){
	llenarW();
}