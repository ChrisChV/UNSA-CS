#include <stdio.h> 
#include <string.h>

static const char LOG_FILE[] = "/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/log";

void writeLog(const char * filename, char * word){
	FILE * file = fopen(filename,"a");
	fwrite(word,sizeof(char),strlen(word),file);
	fputc('\n',file);
	fclose(file);
}

int main(){
	writeLog(LOG_FILE,"hello friend");

}