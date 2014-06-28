#ifndef EXCCEL_TYPES_SOURCE
#define EXCCEL_TYPES_SOURCE

#include <ctype.h>
#include <string.h>

#include "exccel_types.h"
#include "exccel_utils.h"
#include "exccel_table.h"

exccel_var* _allocate() {
    exccel_var* tmp = malloc(sizeof(exccel_var));
    
    if (tmp == NULL) {
        printf("%s - _allocate() method:\n",__FILE__);
        printf("Out of memory error!\n");
        exit(EXIT_FAILURE);
    }
    
    return tmp;
}

void freeExccelVar(exccel_var *v) {
    free(v);
}

exccel_var* _setExcelVarTo(exccel_var *var, exccel_var_data data, exccel_var_data_type type) {
    var->data = data;
    var->type = type;
    return var;
}

exccel_var* createExccelIntVar(int value) {
    exccel_var_data data;
    exccel_var_data_type type;
    exccel_var* var = _allocate();
    
    type = VAR_INT;
    data.ival = value;
    
    return _setExcelVarTo(var,data,type);
}

exccel_var* createExccelDoubleVar(double value) {
    exccel_var_data data;
    exccel_var_data_type type;
    exccel_var* var = _allocate();
    
    type = VAR_DOUBLE;
    data.dval = value;
    
    return _setExcelVarTo(var,data,type);
}

exccel_var* createExccelStringVar(char *s) {
    exccel_var_data data;
    exccel_var_data_type type;
    exccel_var* var = _allocate();
    int n = strlen(s);
    int i = 0;
    
    type = VAR_STRING;
    data.sval.val = calloc(n+1,sizeof(char));
    
    while(i<n) {
        data.sval.val[i] = s[i];
        i++;
    }
    data.sval.val[i] = '\0';
    data.sval.len = n;
    
    return _setExcelVarTo(var,data,type);
}

exccel_var* createExccelVarFromString(char *s) {
    if (isInteger(s)) {
        return createExccelIntVar(atoi(s));
    } else if (isFoat(s)) {
        floatStringNormalize(s);
        return createExccelDoubleVar(atof(s));
    } else if (isRange(s) || isCell(s)) {
        return createExccelRangeVarFromString(s);
    } else {
        return createExccelStringVar(s);
    }
}

int _getColLinkNum(char *link,int n) {
    int col = 0, i;
    
    if (n == 1) {
        col = (link[0] - 'A') + 1; 
    } else {
        i = 0;
        while(i<n-1) {
            col += ('Z' - 'A') * ((link[i] - 'A') + 1);
            i++;
        }
        col += (link[i] - 'A') + 1;
    }
    
    return col;
}

void _getCellLinkColAndRow(char *link, int n, int *row, int *col) {
    int c,r,i,j;
    char *cs,*rs;
    
    cs = calloc(n,sizeof(char*));
    rs = calloc(n,sizeof(char*));
    
    i = 0;
    while(isalpha(link[i])) {
        cs[i] = link[i];
        i++;
    }
    
    c = _getColLinkNum(cs,i);
    
    j = 0;
    while(i<n) {
        rs[j] = link[i];
        j++;
        i++;
    }
    
    r = atoi(rs);
    
    *row = r;
    *col = c;
    
    free(cs);
    free(rs);
}

exccel_var* createExccelRangeVarFromString(char *s) {
    int i,j,n,startRow = 0,startCol = 0,endRow = 0,endCol = 0,y,nStart,nEnd;
    char *start, *end;
    
    n = strlen(s);
    
    start = calloc(n,sizeof(char*));
    end   = calloc(n,sizeof(char*));
    
    i = 0;
    j = 0;
    y = 0;
    while(i<n) {
        if (s[i] == ':') {
            y = 1;
            nStart = i;
        } else if (y == 0) {
            start[i] = s[i];
        } else {
            end[j++] = s[i];
        }
        i++;
    }
    
    if (y == 0) {
        start[i] = '\0';
        nStart = i;
        
        _getCellLinkColAndRow(start,nStart,&startRow,&startCol);
        
        free(start);
        free(end);
        
        return createExccelRangeVar(startRow,startCol,0,0);
    }
    
    start[i] = '\0';
    end[j] = '\0';
    nEnd   = j;
    
    _getCellLinkColAndRow(start,nStart,&startRow,&startCol);
    _getCellLinkColAndRow(end,  nEnd,  &endRow,  &endCol);
    
    free(start);
    free(end);
    
    return createExccelRangeVar(startRow,startCol,endRow,endCol);
}

exccel_var* createExccelRangeVar(int startRow, int startCol, int endRow, int endCol) {
    exccel_var_data data;
    exccel_var_data_type type;
    exccel_var* var = _allocate();
    
    type = VAR_RANGE;
    data.range.startRow = startRow;
    data.range.startCol = startCol;
    
    if (endRow == 0 && endCol == 0) {
        data.range.oneCell = 1;
        type = VAR_CELL;
    } else {
        data.range.oneCell = 0;
    }
    
    data.range.endRow   = endRow;
    data.range.endCol   = endCol;
    
    return _setExcelVarTo(var,data,type);
}

exccel_var_data_type getExccelVarDataType(exccel_var* var) {
    return var->type;
}

int getExccelVarIntValue(exccel_var *v) {
    int type;
    
    type = getExccelVarDataType(v);
    if (type == VAR_INT) {
        return v->data.ival;
    } else if (type == VAR_DOUBLE) {
        return (int)v->data.dval;
    } else if (type == VAR_RANGE) {
        return 0;
    } else if (isInteger(v->data.sval.val)) {
        return atoi(v->data.sval.val);
    } else {
        return 0;
    }
}

double getExccelVarDoubleValue(exccel_var *v) {
    int type;
    
    type = getExccelVarDataType(v);
    if (type == VAR_INT) {
        return (double)v->data.ival;
    } else if (type == VAR_DOUBLE) {
        return v->data.dval;
    } else if (type == VAR_RANGE) {
        return 0;
    } else if (isFoat(v->data.sval.val)) {
        return atof(v->data.sval.val);
    } else {
        return 0;
    }
}

char* convertDotToCommas(double d) {
    char *s;
    int n, i;
    
    s = calloc(512,sizeof(char));
    
    sprintf(s,"%3.2f\0",d);
    n = strlen(s);
    
    for (i = 0; i<n; i++) {
        if (s[i] == '.') {
            s[i] = ',';
        }
    }
    
    return s;
}

char* getExccelVarStringValue(exccel_var *v) {
    int type;
    char *buff;
    
    buff = (char*)calloc(512,sizeof(*buff));
    type = getExccelVarDataType(v);
    if (type == VAR_INT) {
        sprintf(buff,"%i\0",v->data.ival);
        return buff;
    } else if (type == VAR_DOUBLE) {
        buff = convertDotToCommas(v->data.dval);
        return buff;
    } else if (type == VAR_RANGE) {
        free(buff);
        return "";
    } else {
        free(buff);
        return v->data.sval.val;
    }
}

struct _exccel_var_data_range * getExccelVarRangeValue(exccel_var *v) {
    int type;
    
    type = getExccelVarDataType(v);
    if (type == VAR_RANGE || type == VAR_CELL) {
        return &(v->data.range);
    }

	return NULL;
}

#endif