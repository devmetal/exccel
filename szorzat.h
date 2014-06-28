/* 
 * A SZORZAT függvény összeszorozza az argumentumként megadott összes számot, 
 * és visszaadja a szorzatot.
 * 
 * Szintaxis
 * SZORZAT(array)
 */

#ifndef exccel_function_szorzat
#define	exccel_function_szorzat

#include <stdlib.h>
#include "exccel_table.h"
#include "exccel_function.h"
#include "exccel_utils.h"

int szorzatSig[2] = {1,FN_SIG_LIST};

double multipleRange(exccel_var *range, table *t) {
    double *tmp, res;
    int row, col, n, i;
    
    tmp = getDoubleRangeVector(getExccelVarRangeValue(range),t,&row,&col,&n);
    i = 0;
    while(i < n) {
        if (i == 0) {
            res = tmp[i];
        } else {
            res *= tmp[i]; 
        }
        i++;
    }
    
    free(tmp);
    return res;
}

void szorzat(exccel_function *ef) {
    exccel_var **param, *current, *ret;
    int n, i, type;
    double result,tmp;
    
    param  = ef->par;
    n      = ef->paramsCount;
    i      = 0;
    result = 0.0;
    
    while(i<n) {
        current = param[i];
        type = getExccelVarDataType(current);
        
        if (current == NULL) {
            ret = createExccelStringVar("#NÉV?");
            assignReturnValueToFunction(ef,ret);
            return;
        }
        
        if (type == VAR_RANGE) {
            
            if (cellIsInRange(getExccelVarRangeValue(current),ef->cell)) {
                ret = createExccelStringVar("#NÉV?");
                assignReturnValueToFunction(ef,ret);
                return;
            }
            
            tmp = multipleRange(current,ef->table);
            
        } else if (type == VAR_CELL) {
            
            if (cellIsInRange(getExccelVarRangeValue(current),ef->cell)) {
                ret = createExccelStringVar("#NÉV?");
                assignReturnValueToFunction(ef,ret);
                return;
            }
            
            current = getCellLinkValue(getExccelVarRangeValue(current),ef->table);
            if (current == NULL) {
                ret = createExccelStringVar("#NÉV?");
                assignReturnValueToFunction(ef,ret);
                return;
            }
            
            tmp = getExccelVarDoubleValue(current);
            
        } else {
            tmp = getExccelVarDoubleValue(current);
        }
        
        if (i == 0) {
            result = tmp;
        } else {
            result *= tmp;
        }
        
        i++;
    }
    
    ret = createExccelDoubleVar(result);
    assignReturnValueToFunction(ef,ret);
}

#endif

