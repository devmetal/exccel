#ifndef exccel_utils_source
#define	exccel_utils_source

#include "exccel_function.h"
#include "exccel_table.h"
#include "exccel_processor.h"
#include "exccel_utils.h"
#include "exccel_errors.h"

#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>

int strpos(char *s1, char *s2) {
    char *p = strstr(s1, s2);
    if (p) {
        return p - s1;
    } else {
        return -1;
    }
}

char *strput(char *s1, char *s2, int p, int n) {
    char *tmp;
    int i = 0;
    
    tmp = calloc(strlen(s1),sizeof(char));
    tmp = strcpy(tmp,s1);
    tmp = realloc(tmp,sizeof(char) * (strlen(s1) + n));
    
    while(i<n) {
        tmp[i + p] = s2[i];
        i++;
    }
    
    return tmp;
}

char *getInputFileOutputName(char *ifname) {
    char *oname;
    int n;
    int p;
    
    n = strlen(ifname);
    p = strpos(ifname,".csv");
    
    if (p == -1) {
        exccel_fatal_error("Hibas bemeneti fajl\nnem talalhato a .csv kiterjesztes");
    }
    
    oname = strput(ifname,"_mod\0",p,5);
    oname = strput(oname,".csv\0",strlen(oname),5);
    
    return oname;
}

int isInteger(char *s) {
    int n = strlen(s);
    int i = 0;
    
	if (s[0] == '\0')
		return 0;

    for (;i<n;i++) {
        if (!isdigit(s[i]) && s[i] != '-') {
            return 0;
        }
    }
    
    return 1;
}

int isFoat(char *s) {
    int n = strlen(s);
    int i = 0;

	if (s[0] == '\0')
		return 0;
    
    for (;i<n;i++) {
        if (!isdigit(s[i]) && (s[i] != '.' && s[i] != ',' && s[i] != '-')) {
            return 0;
        }
    }
    
    return 1;
}

int _isCell(char *s, int n) {
    int i = 0, letters = 0, digits = 0;
    
    while(i<n) {
        if (isalpha(s[i])) {
            letters++;
        } else if (isdigit(s[i]) && letters > 0) {
            digits++;
        }  else {
            return 0;
        }
        i++;
    }
    
    return ((letters + digits) == n);
}

int isRange(char *s) {
    int n = strlen(s);
    int i = 0, j = 0, y = 0, flag = 0, letters = 0, digits = 0;
    
    char *start, *end;
    
    start = calloc(n,sizeof(char));
    end   = calloc(n,sizeof(char)); 
    
    for (;i<n;i++) {
        if (s[i] == ':') {
            flag = 1;
        } else if (flag == 0) {
            start[j++] = s[i];
        } else {
            end[y++] = s[i];
        }
    }
    
    if (flag == 0) {
        return 0;
    }
    
    if (!_isCell(start,j)) {
        return 0;
    }
    
    if (!_isCell(end,y)) {
        return 0;
    }
    
    free(start);
    free(end);
    
    return 1;
}

int isCell(char *s) {
    int n = strlen(s);
    return _isCell(s,n);
}

double **getDoubleRangeMatrix(struct _exccel_var_data_range *range, table *t, int *row, int *col) {
    int sr,sc,er,ec,n,i,j;
    double **tmp;
    table_cell_matrix *m;
    table_cell *tmpCell;
    
    sr = range->startRow;
    sc = range->startCol;
    er = range->endRow;
    ec = range->endCol;
    
    m = getCells(t,sr,er,sc,ec);
    
    if (m == NULL) {
        return NULL;
    }
    
    if (!rangeConflictResolve(m,t)) {
        return NULL;
    }
    
    n = m->col;
    
    tmp = (double**)calloc(n,sizeof(double));
    for (i = 0; i < n; i++) {
        tmp[i] = (double*)calloc(n,sizeof(double));
        for (j = 0; j < n; j++) {
            tmpCell = getCell(t,i+sr,j+sc);
            if (tmpCell == NULL) {
                return NULL;
            } else {
                tmp[i][j] = getExccelVarDoubleValue(tmpCell->data);
            }
        }
        free(m->cells[i]);
    }
    
    free(m->cells);
    free(m);
    
    *row = m->row;
    *col = m->col;
    
    return tmp;
}

double *getDoubleRangeVector(struct _exccel_var_data_range *range, table *t, int *row, int *col, int *vs) {
    int sr,sc,er,ec,n,i,j,y;
    double *tmp;
    table_cell_matrix *m;
    
    sr = range->startRow;
    sc = range->startCol;
    er = range->endRow;
    ec = range->endCol;
    
    m = getCells(t,sr,er,sc,ec);
    
    if (m == NULL) {
        return NULL;
    }
    
    if (!rangeConflictResolve(m,t)) {
        return NULL;
    }
    
    n = m->row * m->col;
    tmp = (double*)calloc(n,sizeof(double));
    y = 0;
    for (i = 0; i < m->row; i++) {
        for (j = 0; j < m->col; j++) {
            tmp[y++] = getExccelVarDoubleValue(getCell(t,i+sr,j+sc)->data);
        }
        free(m->cells[i]);
    }
    
    *row = m->row;
    *col = m->col;
    *vs   = y;
    
    free(m->cells);
    free(m);
    
    return tmp;
}

exccel_var *getCellLinkValue(struct _exccel_var_data_range *range, table *t) {
    int row, col;
    table_cell *cell;
    
    if (!range->oneCell) {
        return NULL;
    }
    
    row = range->startRow;
    col = range->startCol;
    
    cell = getCell(t,row,col);
    if (cell) {
        return cell->data;
    } else{
        return NULL;
    }
}

void floatStringNormalize(char *s) {
    int n, i;
    
    n = strlen(s);
    for (i = 0; i<n; i++) {
        if (s[i] == ',') {
            s[i] = '.';
        }
    }
}

int rangeConflictResolve(table_cell_matrix *m, table *t) {
    int row, col, i, j;
    table_cell *tmp;
    
    row = m->row;
    col = m->col;
    for (i = 0; i<row; i++) {
        for (j = 0; j<col; j++) {
            tmp = m->cells[i][j];
            if (tmp != NULL && tmp->cp != NULL) {
                if (tmp->cp->proceed == 0) {
                    process_cell(tmp,t);
                    return 1;
                } else if (tmp->cp->proceed == 1) {
                    return 0;
                }
            }
        }
    }
    
    return 1;
}

int cellIsInRange(struct _exccel_var_data_range *range, table_cell *cell) {
    if (range->oneCell) {
        return ((range->startRow == cell->row) && (range->startCol == cell->col));
    }
    
    if ( range->startRow <= cell->row && range->startCol <= cell->col &&
            range->endRow >= cell->row && range->endCol >= cell->col) {
        return 1;
    }
    
    return 0;
}

#endif	/* EXCCEL_UTILS_H */
