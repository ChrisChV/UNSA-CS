#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "postgres.h"
#include "fmgr.h"
#include "math.h"
#include <utils/array.h>

#define HTG_SIZE 256
#define SAME 0
#define LESS_THAN 1
#define GREATER_THAN 2

typedef int VALH;
typedef float Dis;
typedef int Counter;
typedef int CompNumber;
typedef double Rad;


typedef struct Histogram{
	VALH htg[HTG_SIZE];
	Rad radius;
} Histogram;

void generateHistogram(char * path){
	char * prefix = "/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/";
	char * com1 = (char*) palloc(strlen(prefix) + 6);
	com1 = strcpy(com1,prefix);
	com1 = strcat(com1,"HTG ");

	char * comp = (char*) palloc(strlen(com1) + strlen(path) + 5);
	comp = strcpy(comp,com1);
	comp = strcat(comp,path);

	//int e = system("/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/HTG  /home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/hystogramTest/00372608.jpeg ");
	Counter e = system(comp);
	if(e == -1){
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("The histogram doesn't generate")));	
	}

}

Dis histogramDistance(Histogram * htg1, Histogram * htg2){
	Dis result = 0;
	for(int i = 0; i < HTG_SIZE; i++){
		result += pow(htg2->htg[i] - htg1->htg[i],2);
	}
	return sqrt(result);
}


//char * getFirstFiveHTG(Histogram * htg){
//	return psprintf("(%d,%d,%d,%d,%d)",htg->htg[0],htg->htg[1],htg->htg[2],htg->htg[3],htg->htg[4]);
//}

CompNumber histogramComp(Histogram * htg1, Histogram * htg2){
	Counter less = 0;
	Counter greater = 0;
	Counter same = 0;
	for(int i = 0; i < HTG_SIZE; i++){
		if(htg1->htg[i] == htg2->htg[i]) same = same + 1;
		else if(htg1->htg[i] < htg2->htg[i]) less = less + 1;
		else greater = greater + 1;
	}
	if(same == HTG_SIZE) return SAME;
	else if(less >= greater) return LESS_THAN;
	else return GREATER_THAN;
}



#endif