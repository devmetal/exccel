#include <stdio.h>
#include <stdlib.h>

#include "exccel.h"

table* readTable(const char *fname, const char *oname) {
    table *table;
    table_cell *tmp;
    exccel_var *var;
    csv_file *file;
    char *col;
    int colNum;
    int rowNum;
    
    table = initTable(oname);
    
    file = csvFopen(fname);
    
    while(hasNextRow(file)) {
        nextRow(file);
        rowNum = getRowNum(file);
        
        while(hasNextCol(file)) {
            col = nextCol(file);
            colNum = getColNum(file);
            
            tmp = createCell();
            tmp->col = colNum;
            tmp->row = rowNum;
            
            if (isInteger(col)) {
                var = createExccelIntVar(atoi(col));
            } else if (isFoat(col)) {
                floatStringNormalize(col);
                var = createExccelDoubleVar(atof(col));
            } else {
                var = createExccelStringVar(col);
            }
            
            free(col);
            setCellValue(tmp,var);
            addCell(table,tmp);
        }
    }
    close(file);
    
    return table;
}

void processCsvFile(char *fname) {
    table *t = NULL;
    char *oname = getInputFileOutputName(fname);
    
    t = readTable(fname,oname);
    
    printf("Table readed from: %s\n",fname);
    
    build(t);
    process(t);
    saveTable(t);
    freeTable(t);
    
    printf("New file created: %s\n",oname);
}


int main(int argc, char** argv) {
    char *cfn;
    
    INIT_FUNCTION(NAGY,nagy,nagySig);
    INIT_FUNCTION(PARATLAN,paratlan,paratlanSig);
    INIT_FUNCTION(SZORZAT,szorzat,szorzatSig);
    INIT_FUNCTION(MDETERM,mdeterm,mdetermSig);
    
    about();
    
    cfn = getCsvParam(argc,argv);
    if (cfn == NULL) {
        while((cfn = readNextCsv())) {
            processCsvFile(cfn);
        }
    } else {
        processCsvFile(cfn);
    }
    
    destroyFunctions();
    
    printf("Exit now\n");
    return (EXIT_SUCCESS);
}

