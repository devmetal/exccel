#ifndef EXCCEL_TABLE_SOURCE
#define EXCCEL_TABLE_SOURCE

#include <stdlib.h>
#include <string.h>

#include "exccel_table.h"
#include "exccel_function.h"
#include "exccel_csv_writer.h"

table* initTable(const char *name) {
    table *t;

    t = (table*)calloc(1,sizeof(table));
    t->matrix = (table_cell_matrix*)calloc(1,sizeof(table_cell_matrix));
    t->matrix->cells = (table_cell***)calloc(1,sizeof(table_cell));
    t->matrix->cells[0] = (table_cell**)calloc(1,sizeof(table_cell));
    t->matrix->col = 1;
    t->matrix->row = 1;
    t->name.value = name;
    t->name.len = strlen(name);

    return t;
}

table* initTableWithRowsAndCols(const char *name, int row, int col) {
    table *t;
    int i = 0;
    
    t = (table*)calloc(1,sizeof(table));
    t->matrix = (table_cell_matrix*)calloc(1,sizeof(table_cell_matrix));
    
    t->matrix->cells = (table_cell***)calloc(row,sizeof(table_cell));
    while(i<row) {
        t->matrix->cells[i] = (table_cell**)calloc(col, sizeof(table_cell));
        i++;
    }
    
    t->matrix->col = col;
    t->matrix->row = row;
    t->name.value = name;
    t->name.len = strlen(name);

    return t;
}

void freeTable(table *t) {
    int i = 0;
    int j = 0;
    for (;i<t->matrix->row; i++) {
        for (j = 0;j<t->matrix->col; j++) {
            if (t->matrix->cells[i][j] != NULL) {
                freeExccelVar(t->matrix->cells[i][j]->data);
                free(t->matrix->cells[i][j]);
            }
        }
        free(t->matrix->cells[i]);
    }
    free(t->matrix->cells);
    free(t->matrix);
    free(t);
}

/* Hozzáad egy cellát a táblázathoz */
int addCell(table *table, table_cell *cell) {
    int cRow = cell->row;
    int cCol = cell->col;
    int i = 0,j = 0;
    table_cell_matrix *m = table->matrix;
    
    if (m->row < cRow) {
        m->cells = (table_cell***)realloc(m->cells,sizeof(table_cell) * cRow);
        
        for (i = m->row; i<cRow; i++){
            m->cells[i] = (table_cell**)calloc(m->col,sizeof(table_cell));
			for (j = 0; j<m->col; j++) {
				m->cells[i][j] = NULL;
			}
        }
        
        m->row = cRow;
    }
    
    if (m->col < cCol) {
        for (i = 0; i < m->row; i++) {
            m->cells[i] = (table_cell**)realloc(m->cells[i],sizeof(table_cell) * cCol);
			for (j = m->col; j<cCol; j++) {
				m->cells[i][j] = NULL;
			}
        }
        
        m->col = cCol;
    }
    
    m->cells[cRow-1][cCol-1] = cell;

	return 1;
}

table_cell* createCell() {
    table_cell *tmp = NULL; 
    tmp = (table_cell*)calloc(1,sizeof(table_cell));
    tmp->cp = NULL;
    return tmp;
}

/* Lekérdez egy cellát a táblázatból */
table_cell* getCell(table *table, int row, int col) {
    int maxCol = table->matrix->col;
    int maxRow = table->matrix->row;
    
    table_cell *tmp;
    
    if (row > maxRow) {
        return NULL;
    } else if (col > maxCol) {
        return NULL;
    } else if (row < 0 || col < 0) {
        return NULL;
    } else {
        tmp = table->matrix->cells[row-1][col-1];
        if (tmp == NULL) {
            return NULL;
        } else {
            return tmp;
        }
    }
}

table_cell_matrix* getCells(table *table, int startRow, int endRow, int startCol, int endCol) {
    int rows = table->matrix->row,
        cols = table->matrix->col,
        outRowCount = 0,
        outColCount = 0,
        i = 0,
        j = 0,
        y = 0,
        z = 0; 
    
    table_cell_matrix *matrix;
    table_cell ***cells;
    
    if (startRow > rows || 
            startRow < 0 || 
            endRow > rows || 
            endRow < 0 || 
            startRow > endRow) {
        
        return NULL;
    
    }
    
    if (startCol > cols || 
            startCol < 0 || 
            endCol > cols || 
            endCol < 0 || 
            startCol > endCol) {
        
        return NULL;
    }
    
    outRowCount = (endRow - startRow) + 1;
    outColCount = (endCol - startCol) + 1;
    
    cells = (table_cell***)malloc(sizeof(table_cell) * (outRowCount));
    for (i = 0; i<outRowCount; i++) {
        cells[i] = (table_cell**)malloc(sizeof(table_cell) * (outColCount));
    }
    
    for (i = startRow-1; i<endRow; i++) {
        z = 0;
        for (j = startCol-1; j<endCol; j++) {
            cells[y][z++] = table->matrix->cells[i][j];
        }
        y++;
    }
    
    matrix = (table_cell_matrix*)malloc(sizeof(table_cell_matrix));
    matrix->cells = cells;
    matrix->col = outColCount;
    matrix->row = outRowCount;
    
    return matrix;
}

int setCellValue(table_cell *cell, exccel_var *value) {
    cell->data = value;
    return 1;
}

int isString(table_cell *cell) {
    return (cell->data->type == VAR_STRING);
}

int isInt(table_cell *cell) {
    return (cell->data->type == VAR_INT);
}

int isDouble(table_cell *cell) {
    return (cell->data->type == VAR_DOUBLE);
}

int isFunction(table_cell *cell) {
    char *tmp;
    if (isString(cell)) {
        tmp = getCellStringValue(cell,NULL);
        return (tmp[0] == '=');
    } else {
        return 0;
    }
}

int getCellIntValue(table_cell *cell) {
    return getExccelVarIntValue(cell->data);
}

double getCellDoubleValue(table_cell *cell) {
    return getExccelVarDoubleValue(cell->data);
}

char* getCellStringValue(table_cell *cell, int *n) {
    char *s;
    if (isString(cell)) {
        if (n != NULL)
            *n = cell->data->data.sval.len;

        return cell->data->data.sval.val;
    } else {
        s = getExccelVarStringValue(cell->data);
        if (n != NULL) {
            *n = strlen(s);
        }
        
        return s;
    }
}

void saveTable(table *t) {
    int i,j;
    table_cell *cell;
    csv_writer *w;
    char *tmp;
    
    w = openCsvWriter(t->name.value);
    
    for (i = 1; i<=t->matrix->row; i++) {
        for (j = 1; j<=t->matrix->col; j++) {
            cell = getCell(t,i,j);
            if (cell != NULL) {
                tmp = getCellStringValue(cell,NULL);
                addCol(w,tmp);
                free(tmp);
            }
        }
        newLine(w);
    }
    
    closeWriter(w);
}

#endif