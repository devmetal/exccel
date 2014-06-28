#ifndef exccel_utils_header
#define	exccel_utils_header

#include "exccel_types.h"

void floatStringNormalize(char *s);

int isInteger(char *s);
int isFoat(char *s);
int isRange(char *s);
int isCell(char *s);

double **getDoubleRangeMatrix(struct _exccel_var_data_range *range, table *t, int *row, int *col);
double *getDoubleRangeVector (struct _exccel_var_data_range *range, table *t, int *row, int *col, int *n);
exccel_var *getCellLinkValue(struct _exccel_var_data_range *range, table *t);

int rangeConflictResolve(table_cell_matrix *m, table *t);

int cellIsInRange(struct _exccel_var_data_range *range, table_cell *cell);

char *getInputFileOutputName(char *ifname);

#endif	/* EXCCEL_UTILS_H */

