#ifndef exccel_builder_header
#define	exccel_builder_header

#include "exccel_table.h"

void addProcess(cell_process **LIST, cell_process *n);

void build(table* table);
void buildCell(table_cell *cell);
void buildCellWithFunction(table_cell *cell);
void buildCellProcessList(table_cell *cell, cell_process **HEAD);

int  buildFunction   (char *fnString, int n, int offset, table_cell *cell, cell_process **LIST, int paramPos);
char *readFnName     (char *fnString, int n, int offset, int *size);
char **readFnParams  (char *fnString, int n, int *count, int **func);
int  getParamsCount  (char *fnString, int n);
int  seekToFirstParam(char *fnString, int n);
int  seekToNextParam (char *fnString, int n, int offset);
char *readNextParam  (char *fnString, int n, int offset, int *size);
char *readNextFunctionParam(char *fnString, int n, int offset, int *size);

#endif