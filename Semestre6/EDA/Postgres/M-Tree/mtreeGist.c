#include <stdio.h> 

#include "postgres.h"
#include "fmgr.h"

#include "stratnum.h"
#include "gist.h"
#include "itup.h"
#include "palloc.h"
#include "geo_decls.h"
#include "elog.h"

#include "../hystogramTest/histogram.h"
#include "extrafunctions.h"

#define PENALTY_LIMIT 2400000; //2349296 -> 1024 * 768 * 3 

PG_MODULE_MAGIC;

typedef struct Mkey{
    Histogram htg;
    Dis radius;
} Mkey;

PG_FUNCTION_INFO_V1(histogram_consistent);

Datum histogram_consistent(PG_FUNCTION_ARGS)
{
    ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("ERRROOOORR -> CONSISTENT")));




    GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
    //Histogram  *query = PG_GETARG_DATA_TYPE_P(1);
    StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
    /* Oid subtype = PG_GETARG_OID(3); */
    bool       *recheck = (bool *) PG_GETARG_POINTER(4);
    Mkey  *key = DatumGetPointer(entry->key);
    bool        retval;

    /*
     * determine return value as a function of strategy, key and query.
     *
     * Use GIST_LEAF(entry) to know where you're called in the index tree,
     * which comes handy when supporting the = operator for example (you could
     * check for non empty union() in non-leaf nodes and equality in leaf
     * nodes).
     */

    *recheck = true;        /* or false if check is exact */

    PG_RETURN_BOOL(retval);
}


PG_FUNCTION_INFO_V1(histogram_union);

Datum histogram_union(PG_FUNCTION_ARGS){
    ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("ERRROOOORR -> UNION")));
    GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
    GISTENTRY  *ent = entryvec->vector;
    Mkey * out = (Mkey *) palloc(sizeof(Mkey));
    Histogram * uni = (Histogram *) palloc(sizeof(Histogram));
    
    Dis dMin = PENALTY_LIMIT;
    Dis dMax = 0;
    Dis temp = 0;
    Dis rMax = 0;


    Counter numranges = entryvec->n;

    Mkey * key;

    for(int d = 0; d < HTG_SIZE; d++){        
        dMin = PENALTY_LIMIT;
        dMax = 0;
        for(int i = 0; i < numranges; i++){
            key = DatumGetPointer(ent[i].key);
            temp = key->htg.htg[d] - key->radius;
            if(temp < dMin) dMin = temp;
            temp = key->htg.htg[d] + key->radius;
            if(temp > dMax) dMax = temp;
        }
        uni->htg[d] = (VALH) (dMin + dMax) / 2;
    }

    for(int i = 0; i < numranges; i++){
        key = DatumGetPointer(ent[i].key);
        Dis dis = histogramDistance(uni,&(key->htg)) + key->radius;
        if(dis > rMax) rMax = dis;
    }

    out->radius = rMax;
    out->htg = *uni;
    
    PG_RETURN_POINTER(out);
}

PG_FUNCTION_INFO_V1(histogram_compress);

Datum histogram_compress(PG_FUNCTION_ARGS){
    //writeLog(LOG_FILE,"COMPRESS\n");
    //ereport(ERROR,
      //          (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
        //         errmsg("ERRROOOORR -> COMPRESS")));
    PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}

PG_FUNCTION_INFO_V1(histogram_decompress);

Datum histogram_decompress(PG_FUNCTION_ARGS){
    //writeLog(LOG_FILE,"DECOMPRESS\n");
    //ereport(ERROR,
      //          (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
        //         errmsg("ERRROOOORR -> DECOMPRESS")));
    PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}

PG_FUNCTION_INFO_V1(histogram_picksplit);

Datum histogram_picksplit(PG_FUNCTION_ARGS)
{
    
    GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
    OffsetNumber i;
    OffsetNumber maxoff = entryvec->n - 1;
    GIST_SPLITVEC *v = (GIST_SPLITVEC *) PG_GETARG_POINTER(1);
    GISTENTRY **raw_entryvec;

    VALH dimTemp = 0;

    Counter left = 0;
    Counter right = 0;
    VALH sumLeft = 0;
    VALH sumRight = 0;

    int nbytes;

    Histogram * prom = (Histogram *) palloc(sizeof(Histogram));

    nbytes = (maxoff + 2) * sizeof (OffsetNumber);
    v-> spl_left = (OffsetNumber *) palloc (nbytes);
    v-> spl_right = (OffsetNumber *) palloc (nbytes);
    v->spl_ldatum =  PointerGetDatum(palloc (sizeof(Mkey)));
    v->spl_rdatum =  PointerGetDatum(palloc (sizeof(Mkey)));
    v-> spl_nleft = 0;
    v-> spl_nright = 0;

    Mkey * key;


    raw_entryvec = (GISTENTRY **) malloc(entryvec->n * sizeof(void *));
    for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)){
        raw_entryvec[i] = &(entryvec->vector[i]);
    }


    Counter n = 0;

    for(int d = 0; d < HTG_SIZE; d++){
        dimTemp = 0;
        n = 0;
        for(i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)){
            key = DatumGetPointer(raw_entryvec[i]->key);
            dimTemp = dimTemp + key->htg.htg[d];
            n = n + 1;
        }
        dimTemp = dimTemp / n;
        prom->htg[d] = dimTemp;
    }

    int e = 0;

    for(i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)){
        left = 0;
        right = 0;
        sumLeft = 0;
        sumRight = 0;
        key = DatumGetPointer(raw_entryvec[i]->key);

        for(int d = 0; d < HTG_SIZE; d++){
           if(key->htg.htg[d] >= prom->htg[d]){
            right++;
            sumRight = sumRight + key->htg.htg[d] - prom->htg[d];
           }
           else{
            left++;
            sumLeft = sumLeft + prom->htg[d] - key->htg.htg[d];
           }
        }

        if(right > left){
            v->spl_right[v->spl_nright] = i;
            v->spl_nright++;
        }
        else if(left > right){
            v->spl_left[v->spl_nleft] = i;
            v->spl_nleft++;
        }
        else if(sumLeft < sumRight){
            v->spl_left[v->spl_nleft] = i;
            v->spl_nleft++;       
        }
        else if(sumRight < sumLeft){
            v->spl_right[v->spl_nright] = i;
            v->spl_nright++;
        }
        else{
            v->spl_right[v->spl_nright] = i;
            v->spl_nright++;
        }
        e = e + 1;
    }

    //writeLogInSplit(v);

    PG_RETURN_POINTER(v);
}

PG_FUNCTION_INFO_V1(histogram_penalty);

Datum histogram_penalty(PG_FUNCTION_ARGS)
{
    ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("ERRROOOORR -> penalty")));
    //writeLog(LOG_FILE,"PENALTY of ");
    ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("ERRROOOORR -> PENALTY")));
    GISTENTRY * origentry = (GISTENTRY *) PG_GETARG_POINTER(0);
    GISTENTRY * newentry = (GISTENTRY *) PG_GETARG_POINTER(1);
    float * result = (float *) PG_GETARG_POINTER(2);
    float tempResult = 0;
    Mkey * orig = DatumGetPointer(origentry->key);
    Mkey * new = DatumGetPointer(newentry->key);
    Dis dis = histogramDistance(&(orig->htg), &(new->htg));
    Dis radius = orig->radius; 
    char * v1 = getFirstFiveHTG(&(orig->htg));
    char * v2 = getFirstFiveHTG(&(new->htg));
    char * v3;

    /*
    writeLog(LOG_FILE,v1);
    writeLog(LOG_FILE," - ");
    writeLog(LOG_FILE,v2);
    writeLog(LOG_FILE," : ");
    */

    if(dis > radius){
        tempResult = dis - radius + PENALTY_LIMIT;
    }
    else{
        tempResult = dis;
    }

    *result = tempResult;
    sprintf(v3, "%f", tempResult);
    /*
    writeLog(LOG_FILE,v3);
    writeLog(LOG_FILE,"\n");
    
    */

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(histogram_same);

Datum histogram_same(PG_FUNCTION_ARGS)
{
    ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("ERRROOOORR -> SAME")));
    //prefix_range *v1 = PG_GETARG_PREFIX_RANGE_P(0);
    //prefix_range *v2 = PG_GETARG_PREFIX_RANGE_P(1);
    ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("ERRROOOORR -> SAME")));
    bool       *result = (bool *) PG_GETARG_POINTER(2);

    *result = false;
    PG_RETURN_POINTER(result);
}


