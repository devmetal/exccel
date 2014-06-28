/* 
 * Egy adathalmaz k-adik legnagyobb elemét adja eredményül. 
 * A függvényt egy érték relatív elhelyezkedése szerinti kiválasztására használhatja. 
 * A NAGY függvénnyel például adott szempontok szerint megállapíthatja az első, 
 * a második és a harmadik helyezett értéket.
 * 
 * Szintaxis:
 * NAGY(array, k)
 * 
 * A NAGY függvény szintaxisa az alábbi argumentumokat foglalja magában:
 * Tömb: Megadása kötelező. Az a tömb vagy adattartomány, amelynek k-adik legnagyobb elemét keresi.
 * K:    Megadása kötelező. Azt adja meg, hogy (a legnagyobbtól kezdve visszafelé) 
 *       hányadik legnagyobb elemet kell megkeresni.
 */

#ifndef exccel_function_nagy
#define	exccel_function_nagy

#include <stdlib.h>
#include <stdio.h>
#include "exccel_table.h"
#include "exccel_function.h"
#include "exccel_utils.h"

int nagySig[3] = {2,FN_SIG_RANGE,FN_SIG_LITERAL};

double* _sort(double *vector, int n) {
    int i, j;
    double tmp;
    
    for (i = 0; i<n-1; i++) {
        for (j = i + 1; j<n; j++) {
            if (vector[i] < vector[j]) {
                tmp = vector[i];
                vector[i] = vector[j];
                vector[j] = tmp;
            }
        }
    }
    
    return vector;
}

void nagy(exccel_function *ef) {
    exccel_var *ret;
    exccel_var *kVar;
    exccel_var *rVar;
    table      *t;
    
    double *tmp;
    int row,col,k,n;
    
    struct _exccel_var_data_range *range;
    
    t = ef->table;
    
    rVar  = getFunctionParam(ef,0);
    if (rVar == NULL) {
        ret = createExccelStringVar("#NÉV?");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    range = getExccelVarRangeValue(rVar);
    if (cellIsInRange(range,ef->cell)) {
        ret = createExccelStringVar("#NÉV?");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    tmp = getDoubleRangeVector(range,t,&row,&col,&n);
    
    if (tmp == NULL) {
        ret = createExccelStringVar("#NÉV?");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    kVar  = getFunctionParam(ef,1);
    if (kVar == NULL) {
        ret = createExccelStringVar("#NÉV?");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    if (getExccelVarDataType(kVar) == VAR_CELL) {
        
        if (cellIsInRange(getExccelVarRangeValue(kVar),ef->cell)) {
            ret = createExccelStringVar("#NÉV?");
            assignReturnValueToFunction(ef,ret);
            return;
        }
        
        kVar = getCellLinkValue(getExccelVarRangeValue(kVar),t);
    }
    
    k = getExccelVarIntValue(kVar);

    tmp = _sort(tmp,n);
    
    k--;
    if (k >= n) {
        k = n - 1;
    } else if (k < 0) {
        k = 0;
    }
    
    ret = createExccelDoubleVar(tmp[k]);
    assignReturnValueToFunction(ef,ret);
    
    free(tmp);
}


#endif

