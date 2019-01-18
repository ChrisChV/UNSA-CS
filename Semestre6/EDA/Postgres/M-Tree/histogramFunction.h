#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "postgres.h"

#include "access/gist.h"
#include "access/itup.h"
#include "../hystogramTest/histogram.h"

static const char LOG_FILE[] = "/home/xnpio/Documentos/Xnpio/EDA-UNSA/EDA-UNSA/Postgres/log";

#define LIMIT 2400000;

Histogram * htg_union(Histogram ** ent, Counter numranges){
	Histogram * uni = (Histogram*) palloc(sizeof(Histogram));
	Dis dMin = LIMIT;
    Dis dMax = 0;
    Dis temp = 0;
    Dis rMax = 0;

    Histogram * key;

    for(int d = 0; d < HTG_SIZE; d++){        
        dMin = LIMIT;
        dMax = 0;
        for(int i = 0; i < numranges; i++){
            key = ent[i];
            temp = key->htg[d] - key->radius;
            if(temp < dMin) dMin = temp;
            temp = key->htg[d] + key->radius;
            if(temp > dMax) dMax = temp;
        }
        uni->htg[d] = (VALH) (dMin + dMax) / 2;
    }

    for(int i = 0; i < numranges; i++){
        key = ent[i];
        Dis dis = histogramDistance(uni,key) + key->radius;
        if(dis > rMax) rMax = dis;
    }

    uni->radius = rMax;

    return uni;
}


void writeLog(const char * filename, char * word){
	FILE * file = fopen(filename,"a");
	fwrite(word,sizeof(char),strlen(word),file);
	fclose(file);
}

void writeLogInSplit(GIST_SPLITVEC * v){
	writeLog(LOG_FILE,"LEFT:");
	for(int i = 0; i < v->spl_nleft; i++){
		 int temp = v->spl_left[i];
		 char * word;
		 sprintf(word,"%d",temp);
		 writeLog(LOG_FILE,word);
		 writeLog(LOG_FILE," ");
	}
	writeLog(LOG_FILE,"RIGHT:");
	for(int i = 0; i < v->spl_nright; i++){
		int temp = v->spl_right[i];
		 char * word;
		 sprintf(word,"%d",temp);
		 writeLog(LOG_FILE,word);
		 writeLog(LOG_FILE," ");	
	}
	writeLog(LOG_FILE,"\n");
}