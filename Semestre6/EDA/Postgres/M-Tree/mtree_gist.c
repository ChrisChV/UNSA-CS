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
#include "histogramFunction.h"

#define PENALTY_LIMIT 2400000; //2349296 -> 1024 * 768 * 3 

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(gist_histogram_consistent);

Datum gist_histogram_consistent(PG_FUNCTION_ARGS)
{
    GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
    //Histogram  *query = PG_GETARG_DATA_TYPE_P(1);
    StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
    /* Oid subtype = PG_GETARG_OID(3); */
    bool       *recheck = (bool *) PG_GETARG_POINTER(4);
    Histogram  *key = DatumGetPointer(entry->key);
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

PG_FUNCTION_INFO_V1(gist_histogram_union);

Datum gist_histogram_union(PG_FUNCTION_ARGS){
    GistEntryVector * entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
    GISTENTRY  *ent = entryvec->vector;
    Counter numranges = entryvec->n;

    Histogram ** vec = palloc(sizeof(void*) * numranges);

    for(int i = 0; i < numranges; i++){
        vec[i] = &(ent[i].key);
    }

    PG_RETURN_POINTER(htg_union(vec,numranges));
}

PG_FUNCTION_INFO_V1(gist_histogram_compress);

Datum gist_histogram_compress(PG_FUNCTION_ARGS){
    PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}

PG_FUNCTION_INFO_V1(gist_histogram_decompress);

Datum gist_histogram_decompress(PG_FUNCTION_ARGS){
    PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}


PG_FUNCTION_INFO_V1(gist_histogram_picksplit);

Datum gist_histogram_picksplit(PG_FUNCTION_ARGS)
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

    Counter nbytes;

    Histogram * prom = (Histogram *) palloc(sizeof(Histogram));

    nbytes = (maxoff + 2) * sizeof (OffsetNumber);
    v-> spl_left = (OffsetNumber *) palloc (nbytes);
    v-> spl_right = (OffsetNumber *) palloc (nbytes);
    v->spl_ldatum =  PointerGetDatum(palloc (sizeof(Histogram)));
    v->spl_rdatum =  PointerGetDatum(palloc (sizeof(Histogram)));
    v-> spl_nleft = 0;
    v-> spl_nright = 0;

    Histogram * key;

    raw_entryvec = (GISTENTRY **) palloc(entryvec->n * sizeof(void *));
    for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)){
        raw_entryvec[i] = &(entryvec->vector[i]);
    }

    Counter n = 0;

    for(int d = 0; d < HTG_SIZE; d++){
        dimTemp = 0;
        n = 0;
        for(i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)){
            key = DatumGetPointer(raw_entryvec[i]->key);
            dimTemp = dimTemp + key->htg[d];
            n = n + 1;
        }
        dimTemp = dimTemp / n;
        prom->htg[d] = dimTemp;
    }

    Counter e = 0;

    for(i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)){
        left = 0;
        right = 0;
        sumLeft = 0;
        sumRight = 0;
        key = DatumGetPointer(raw_entryvec[i]->key);

        for(int d = 0; d < HTG_SIZE; d++){
           if(key->htg[d] >= prom->htg[d]){
            right++;
            sumRight = sumRight + key->htg[d] - prom->htg[d];
           }
           else{
            left++;
            sumLeft = sumLeft + prom->htg[d] - key->htg[d];
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


    Histogram ** htgleft = palloc(sizeof(void*) * v->spl_nleft);
    Histogram ** htgright = palloc(sizeof(void*) * v->spl_nright);

    for(int j = 0; j < v->spl_nleft; j++){
        key = DatumGetPointer(raw_entryvec[v->spl_left[j]]->key);
        htgleft[j] = key;
    }
    
    for(int j = 0; j < v->spl_nright; j++){
        key = DatumGetPointer(raw_entryvec[v->spl_right[j]]->key);
        htgright[j] = key;
    }

    Histogram * unileft = htg_union(htgleft,(Counter) v->spl_nleft);
    Histogram * uniright = htg_union(htgright,(Counter) v->spl_nright);

    v->spl_ldatum = PointerGetDatum(unileft);
    v->spl_rdatum = PointerGetDatum(uniright);

    //writeLogInSplit(v);

    PG_RETURN_POINTER(v);
}

PG_FUNCTION_INFO_V1(gist_histogram_penalty);

Datum gist_histogram_penalty(PG_FUNCTION_ARGS)
{
    GISTENTRY * origentry = (GISTENTRY *) PG_GETARG_POINTER(0);
    GISTENTRY * newentry = (GISTENTRY *) PG_GETARG_POINTER(1);
    Dis * result = (Dis *) PG_GETARG_POINTER(2);
    Dis tempResult = 0;
    Histogram * orig = DatumGetPointer(origentry->key);
    Histogram * new = DatumGetPointer(newentry->key);
    Dis dis = histogramDistance(orig, new);
    Dis radius = orig->radius; 

    if(dis > radius){
        tempResult = dis - radius + PENALTY_LIMIT;
    }
    else{
        tempResult = dis;
    }

    *result = tempResult;
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(gist_histogram_same);

Datum gist_histogram_same(PG_FUNCTION_ARGS)
{
    bool       *result = (bool *) PG_GETARG_POINTER(2);
    *result = false;
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(gist_histogram_distance);

Datum gist_histogram_distance(PG_FUNCTION_ARGS)
{
    GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
    Histogram  *query = PG_GETARG_POINTER(1);
    StrategyNumber strategy = (StrategyNumber)  PG_GETARG_UINT16(2);
    /* Oid subtype = PG_GETARG_OID(3); */
    Histogram  *key = DatumGetPointer(entry->key);
    Dis      retval;

    if(strategy == 15){
        if(GIST_LEAF(entry)) retval = histogramDistance(query,key);
        else retval = histogramDistance(query,key) - key->radius;
    }

    //float8 res = (float8) retval;

    PG_RETURN_FLOAT8(retval);
}



