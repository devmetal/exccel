#ifndef exccel_processor_source
#define exccel_processor_source

#include "exccel_function.h"
#include "exccel_processor.h"

void _printProcesses(cell_process *LIST, int tab) {
    cell_process *tmp = NULL;
    int i = 0;
    tmp = LIST;
    while(tmp != NULL) {
        i = 0;
        while(i++<tab) printf(" ");
        printf("%s, %d, paramPos: %i\n",tmp->func->name,tmp->func->paramsCount,tmp->paramPos);
        if (tmp->childs != NULL) {
            i = 0;
            while(i++<tab + 3) printf(" ");
            printf("Childs\n");
            _printProcesses(tmp->childs, tab + 3);
        }
        tmp = tmp->next;
    }
}

void _free_functions(cell_process *cp) {
    cell_process *tmp;
    cell_process *prev;
    tmp = cp;
    while(tmp != NULL) {
        if (tmp->childs != NULL) {
            _free_functions(tmp->childs);
        }
        
        free(tmp->func->paramIndexes);
        free(tmp->func);
        
        prev = tmp;
        tmp = tmp->next;
        
        free(prev);
    }
}

void _process_cell_process(cell_process *PARENT,cell_process *CURRENT,table *t) {
    cell_process *START, *TMP;
    
    if (CURRENT == NULL) {
        START = PARENT;
        if (START->childs != NULL) {
            _process_cell_process(START,START->childs,t);
        }
        runFunction(START->func,START->cell,t);
    } else {
        START = CURRENT;
        TMP   = CURRENT;
        while(TMP != NULL) {
            
            if (TMP->childs != NULL) {
                _process_cell_process(TMP,TMP->childs,t);
            }
            
            runFunction(TMP->func,TMP->cell,t);
            
            addParamToFunction(PARENT->func,getReturnValue(TMP->func),TMP->paramPos);
            TMP = TMP->next;
        }
    }
}

void process_cell(table_cell *c, table *t) {
    cell_process *HEAD = c->cp;
    exccel_var *result;
    
    //_printProcesses(HEAD,0);
    
    HEAD->proceed = 1;
    _process_cell_process(HEAD,NULL,t);
    HEAD->proceed = 2;
    result = getReturnValue(HEAD->func);
    
    freeExccelVar(c->data);
    setCellValue(c,result);
    
    _free_functions(HEAD);
}

void process(table *t) {
    int col = t->matrix->col;
    int row = t->matrix->row;
    int i,j;
    
    table_cell *cellTemp;
    
    for (i = 1; i<=row; i++) {
        for (j = 1; j<=col; j++) {
            cellTemp = getCell(t,i,j);
            if (cellTemp != NULL && cellTemp->cp != NULL && cellTemp->cp->proceed == 0) {
                process_cell(cellTemp,t);
            }
        }
    }
}

#endif