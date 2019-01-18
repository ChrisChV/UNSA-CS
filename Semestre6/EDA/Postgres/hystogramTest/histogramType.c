#include <stdio.h> 
#include <string.h>	
#include "postgres.h"
#include "fmgr.h"
#include "histogram.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(htg_in);

Datum htg_in(PG_FUNCTION_ARGS){
	char *str = PG_GETARG_CSTRING(0);
	char buffer[30];
	VALH temp;
	Histogram *result;
	
	generateHistogram(str);
	//memcpy(result->path,str,256);

	str = strcat(str,".htg");
	FILE * hgrFile = fopen(str,"r");
	

	if(hgrFile == NULL){
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for histogram: \"%s\" The file doesn't exist",
						str)));
	}
	
	result = (Histogram *) palloc(sizeof(Histogram));

	for(int i = 0; i < HTG_SIZE; i++){	
		fgets(buffer,31,hgrFile);	
		sscanf(buffer, "%d", &temp);
		result->htg[i] = temp;
		
	}

	fclose(hgrFile);

	char * comp2 = (char*) palloc(strlen(str) + 3 + 5);
	comp2 = strcpy(comp2,"rm ");
	comp2 = strcat(comp2,str);
	Counter e = system(comp2);
	
	if(e == -1){
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("The histogram doesn't generate")));	
	}

	result->radius = 0;

	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(htg_out);

Datum htg_out(PG_FUNCTION_ARGS){
	Histogram * htg = (Histogram*) PG_GETARG_POINTER(0);
	char * result = "(";
	char * result2;
	double res = 0;
	
	for(int i = 0; i < 256; i++){
		/*if(i < 3){
			char * temp = psprintf("%d",htg->htg[i]);	
			char * temp2 = (char*) palloc(strlen(result));
			temp2 = strcpy(temp2,result);
			result = (char*) palloc(strlen(temp2) + strlen(temp) + 2);
			result = strcpy(result,temp2);
			result = strcat(result,",");
			result = strcat(result,temp);
		}
		*/
		res = res + htg->htg[i];

	}
	
	result2 = psprintf("Sum: %g - Last five: (%d,%d,%d,%d,%d) - R: %lf",res,htg->htg[251],htg->htg[252],htg->htg[253],htg->htg[254],htg->htg[255],htg->radius);
	
	PG_RETURN_CSTRING(result2);
}

PG_FUNCTION_INFO_V1(htg_same);

Datum htg_same(PG_FUNCTION_ARGS){
	Histogram * htg1 = (Histogram*) PG_GETARG_POINTER(0);
	Histogram * htg2 = (Histogram*) PG_GETARG_POINTER(1);

	CompNumber cn = histogramComp(htg1,htg2);
	bool result = false;

	if(cn == SAME) result = true;

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(htg_lt);

Datum htg_lt(PG_FUNCTION_ARGS){
	Histogram * htg1 = (Histogram*) PG_GETARG_POINTER(0);
	Histogram * htg2 = (Histogram*) PG_GETARG_POINTER(1);

	CompNumber cn = histogramComp(htg1,htg2);
	bool result = false;

	if(cn == LESS_THAN) result = true;
	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(htg_lte);

Datum htg_lte(PG_FUNCTION_ARGS){
	Histogram * htg1 = (Histogram*) PG_GETARG_POINTER(0);
	Histogram * htg2 = (Histogram*) PG_GETARG_POINTER(1);

	CompNumber cn = histogramComp(htg1,htg2);
	bool result = false;

	if(cn == LESS_THAN || cn == SAME) result = true;
	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(htg_gt);

Datum htg_gt(PG_FUNCTION_ARGS){
	Histogram * htg1 = (Histogram*) PG_GETARG_POINTER(0);
	Histogram * htg2 = (Histogram*) PG_GETARG_POINTER(1);

	CompNumber cn = histogramComp(htg1,htg2);
	bool result = false;

	if(cn == GREATER_THAN) result = true;
	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(htg_gte);

Datum htg_gte(PG_FUNCTION_ARGS){
	Histogram * htg1 = (Histogram*) PG_GETARG_POINTER(0);
	Histogram * htg2 = (Histogram*) PG_GETARG_POINTER(1);

	CompNumber cn = histogramComp(htg1,htg2);
	bool result = false;

	if(cn == GREATER_THAN || cn == SAME) result = true;
	PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(htg_distance);

Datum htg_distance(PG_FUNCTION_ARGS){
	Histogram * htg1 = (Histogram*) PG_GETARG_POINTER(0);
	Histogram * htg2 = (Histogram*) PG_GETARG_POINTER(1);

	Dis result = histogramDistance(htg1,htg2);

	PG_RETURN_FLOAT8(result);
}

