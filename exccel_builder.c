#ifndef exccel_builder_source
#define exccel_builder_source

#include "exccel_builder.h"
#include "exccel_utils.h"
#include "exccel_function.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <ctype.h>

table* _processing;

void addProcess(cell_process **LIST, cell_process *n) {
    if (*LIST == NULL) {
        n->next = NULL;
        *LIST = n;
        return;
    }
    
    n->next = *LIST;
    *LIST = n;
}

void build(table* table) {
    int col = table->matrix->col;
    int row = table->matrix->row;
    int i,j;
    
    table_cell *cellTemp;
    _processing = table;
    
    for (i = 1; i<=row; i++) {
        for (j = 1; j<=col; j++) {
            cellTemp = getCell(table,i,j);
            if (cellTemp != NULL) {
                buildCell(cellTemp);
            }
        }
    }
}

void buildCell(table_cell *cell) {
    if (isFunction(cell)) {
        buildCellWithFunction(cell);
    }
}

void buildCellWithFunction(table_cell *cell) {
    cell_process *HEAD = NULL;
    buildCellProcessList(cell,&HEAD);
    cell->cp = HEAD;
}

void buildCellProcessList(table_cell *cell, cell_process **HEAD) {
    char *fnString;
    int  size;
    int i = 0;
    
    fnString = getCellStringValue(cell, &size);
    
    while(i<size) {
        fnString[i] = toupper(fnString[i]);
        i++;
    }
    
    if (!buildFunction(fnString,size,1,cell,HEAD,-1)) {
        setCellValue(cell,createExccelVarFromString("#NÉV?\0"));
    }
}

int buildFunction(char *fnString, int n, int offset, table_cell *cell, cell_process **LIST, int paramPos) {
    char *fnName, *fnParam;
    int  fnNameLength;
    int  *fnSig;
    int  fnSigN;
    int  fnSigI;
    int  sig;
    exccel_var *vtmp;
    exccel_function *ftmp;
    cell_process *ptmp;
    char **parameters;
    int  *fnIndexes;
    int paramsCount;
    int paramI;
    
    fnName = readFnName(fnString,n,offset,&fnNameLength);
    ftmp   = getExccelFunction(fnName);
    if (ftmp == NULL) {
        return 0;
    }
    free(fnName);
    
    ptmp = (cell_process*)malloc(sizeof(cell_process));
    ptmp->cell = cell;
    ptmp->func = ftmp;
    ptmp->paramPos = paramPos; 
    ptmp->t    = _processing;
    ptmp->childs = NULL;
    ptmp->proceed = 0;
    
    addProcess(LIST,ptmp);
    
    parameters = readFnParams(fnString,n,&paramsCount,&fnIndexes);
    
    paramI     = 0;
    fnSig  = ftmp->signature;
    fnSigN = fnSig[0];
    fnSigI = 1;
    
    if (paramsCount < fnSigN) {
        return 0;
    }
    
    while(fnSigI <= fnSigN) {
        sig = fnSig[fnSigI];
        if (sig == FN_SIG_RANGE) {
            fnParam = parameters[paramI];
            vtmp    = createExccelRangeVarFromString(fnParam);
            addParamToFunction(ftmp,vtmp,paramI);
            free(fnParam);
            paramI++;
        } else if (sig == FN_SIG_LITERAL) {
            
            fnParam = parameters[paramI];
            if (fnIndexes[paramI] == 1) {
                if (!buildFunction(fnParam,strlen(fnParam),0,cell,&((*LIST)->childs),paramI)) {
                    return 0;
                }
            } else {
                vtmp = createExccelVarFromString(fnParam);
                addParamToFunction(ftmp,vtmp,paramI);
                free(fnParam);
            }
            paramI++;
            
        } else if (sig == FN_SIG_LIST) {
            
            while(paramI<paramsCount) {
                fnParam = parameters[paramI];
                if (fnIndexes[paramI] == 1) {
                    if (!buildFunction(fnParam,strlen(fnParam),0,cell,&((*LIST)->childs),paramI)) {
                        return 0;
                    }
                } else {
                    vtmp = createExccelVarFromString(fnParam);
                    addParamToFunction(ftmp,vtmp,paramI);
                    free(fnParam);
                }
                paramI++;
            }
            
        } else {
            printf("Invalid signature %d\n",sig);
            exit(1);
        }
        fnSigI++;
    }
    
    
    free(parameters);
    free(fnIndexes);
    
    return 1;
}

char *readFnName(char *fnString, int n, int offset, int *size) {
    char *fnName;
    int nameBuffer, i, j;
    
    i = offset;
    j = 0;
    
    nameBuffer = 8;
    fnName = (char *)calloc(nameBuffer,sizeof(char));    
    while(*(fnString + i) != '(' && i<n) {
        *(fnName + j++) = *(fnString + i++);
        if (j>=nameBuffer) {
            nameBuffer += 8;
            fnName = (char *)realloc(fnName, nameBuffer);
        }
    }
    
    *(fnName + j++) = '\0';
    *size = j;
    
    return fnName;
}

char **readFnParams(char *fnString, int n, int *count, int **func) {
    char **tmp;
    int  *fns = NULL;
    int c,i = 0,offset,s;
    
    c = getParamsCount(fnString,n);
    if (!c) {
        return NULL;
    }
    
    tmp = calloc(c,sizeof(char*));
    fns = (int*)calloc(c,sizeof(int*));
    
    offset = seekToFirstParam(fnString,n); 
    while(i<c) {
        tmp[i] = readNextParam(fnString,n,offset,&s);
        
        if (isFunctionString(tmp[i])) {
            tmp[i]  = readNextFunctionParam(fnString,n,offset,&s);
            offset = seekToNextParam(fnString,n,offset + s - 1);
            fns[i] = 1;
        } else {
            fns[i] = 0;
            offset = seekToNextParam(fnString,n,offset);
        }

        i++;
    }
    *func = fns;
    *count = c;
    
    return tmp;
}

int getParamsCount(char *fnString, int n) {
    int i = 0, c = 0, jump = 0;
    while(i<n) {
        
        if (fnString[i] == '(') {
            jump++;
        } else if (fnString[i] == ';') {
            if (jump == 1) c++;
        } else if (fnString[i] == ')') {
            jump--;
        }
        
        i++;
    }
    
    if (c > 0) {
        return c + 1;
    } else {
        return 1;
    }
}

int seekToFirstParam(char *fnString, int n) {
    int i = 0;
    while(fnString[i++] != '(' && i<n);
    
    return i;
}

int seekToNextParam(char *fnString, int n, int offset) {
    int i = offset;
    
    while(fnString[i++] != ';' && i<n);
    
    return i;
}

char *readNextParam(char *fnString, int n, int offset, int *size) {
    char *params, c;
    int paramBuffer, i, j;
    
    i = offset;
    j = 0;
    paramBuffer = 32;
    params = (char*)calloc(paramBuffer,sizeof(char));
    
    while((c = fnString[i++]) != ';' && c != ')' && c != '(' && i<n) {
        params[j++] = c;
        if (j >= paramBuffer) {
            paramBuffer += 32;
            params = (char*)realloc(params,paramBuffer);
        }
    }
    
    params[j] = '\0';
    *size = j;
    
    return params;
}


char *readNextFunctionParam(char *fnString, int n, int offset, int *size) {
    char *fn, c;
    int  fnBf, i, j, fnStarted = 0, fnClosed = 0;
    
    i    = offset;
    j    = 0;
    fnBf = 32;
    
    fn = (char*)calloc(fnBf, sizeof(char));
    while((fnStarted != fnClosed || fnStarted == 0) && i<n) {
        c = *(fnString + i++);
        
        if (c == '(')
            fnStarted++;
        else if (c == ')')
            fnClosed++;
        
        *(fn + j++) = c;
        if (j >= fnBf) {
            fnBf += 32;
            fn = (char*)realloc(fn, sizeof(char) * fnBf);
        }
        
    }
    
    *(fn + j++) = '\0';
    *size = j;
    
    return fn;
}

#endif