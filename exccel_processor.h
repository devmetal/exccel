#ifndef exccel_processor_header
#define	exccel_processor_header

#include "exccel_table.h"

void process(table *t);
void process_cell(table_cell *c, table *t);

void freeProcess(cell_process *p);

#endif	/* EXCCEL_PROCESSOR_H */

