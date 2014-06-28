/* 
 * Egy tömb mátrix-determinánsát számítja ki.
 * 
 * Szintaxis:
 * MDETERM(array)
 * 
 * Az MDETERM függvény szintaxisa az alábbi argumentumokat foglalja magában:
 * Tömb:    Megadása kötelező. Azonos számú sort és oszlopot tartalmazó numerikus tömb.
 */

#ifndef exccel_funcion_mdeterm
#define	exccel_funcion_mdeterm

#include <stdlib.h>
#include "exccel_table.h"
#include "exccel_function.h"
#include "determinant.h"
#include "exccel_utils.h"

int mdetermSig[2] = {1,FN_SIG_RANGE};

void mdeterm(exccel_function *ef) {
    exccel_var *param;
    exccel_var *ret;
    table      *t;
    double     **tmp;
    int row, col, i;
    struct _exccel_var_data_range *range;
    
    t      = ef->table;
    param  = getFunctionParam(ef,0);
    
    if (param == NULL) {
        ret = createExccelStringVar("#NÉV?");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    range = getExccelVarRangeValue(param);
    
    if (cellIsInRange(range,ef->cell)) {
        ret = createExccelStringVar("#NÉV?");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    tmp = getDoubleRangeMatrix(range,t,&row,&col);
    if (tmp == NULL) {
        ret = createExccelStringVar("#NÉV?");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    if (row != col) {
        ret = createExccelStringVar("#NÉV?");
        assignReturnValueToFunction(ef,ret);
        return;
    }
    
    ret = createExccelDoubleVar(determinant(tmp,row));
    assignReturnValueToFunction(ef,ret);
    
    for (i = 0; i<row; i++) {
        free(tmp[i]);
    }
    free(tmp);
}


#endif

