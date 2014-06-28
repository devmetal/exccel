#ifndef EXCCEL_TABLE_HEADER
#define EXCCEL_TABLE_HEADER

#include "exccel_types.h"


table* initTable(const char *name);

table* initTableWithRowsAndCols(const char *name, int row, int col);

table_cell* createCell();

int addCell(table *table, table_cell *cell);

table_cell* getCell(table *table, int row, int col);

table_cell_matrix* getCells(table *table, int stratRow, int endRow, int startCol, int endCol);

int setCellValue(table_cell *cell, exccel_var *value);

int isString(table_cell *cell);
int isInt(table_cell *cell);
int isDouble(table_cell *cell);
int isFunction(table_cell *cell);

int    getCellIntValue(table_cell *cell);
double getCellDoubleValue(table_cell *cell);
char*  getCellStringValue(table_cell *cell, int *n);

void   saveTable(table *t);
void   freeTable(table *t);
#endif