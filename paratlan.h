/* 
 * Felfelé kerekít egy számot a legközelebbi páratlan egészre.
 * 
 * Szintaxis:
 * PARATAN(szam)
 * 
 * A PÁRATLAN függvény szintaxisa az alábbi argumentumokat foglalja magában:
 * Szám:    Megadása kötelező. A kerekítendő érték.
 */

#ifndef exccel_function_paratlan
#define	exccel_function_paratlan

#include <math.h>
#include "exccel_table.h"
#include "exccel_function.h"
#include "exccel_utils.h"

int paratlanSig[2] = {1,FN_SIG_LITERAL};

void paratlan(exccel_function *ef) {
    double param = 0;
    int    intval;
    exccel_var *ret;
    exccel_var *par;
    
    par = getFunctionParam(ef,0);
    if (par == NULL) {
        ret = createExccelStringVar("#NÉV");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    if (getExccelVarDataType(par) == VAR_CELL) {
        
        if (cellIsInRange(getExccelVarRangeValue(par),ef->cell)) {
            ret = createExccelStringVar("#NÉV");
            assignReturnValueToFunction(ef,ret);
            return;
        }
        
        par = getCellLinkValue(getExccelVarRangeValue(par),ef->table);
    }
    
    param = getExccelVarDoubleValue(par);
    
    intval = (int)(ceil(param));
    if (intval % 2 == 1) {
        ret = createExccelIntVar(intval);
    } else {
        ret = createExccelIntVar(intval + 1);
    }
    
    assignReturnValueToFunction(ef,ret);
}

#endif

