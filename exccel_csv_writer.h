#ifndef exccel_csv_writer_header
#define	exccel_csv_writer_header

#include <stdio.h>
#include <stdlib.h>

struct _csv_writer;

typedef struct _csv_writer csv_writer;

struct _csv_writer {
    FILE *f;
    
    char sep;
    int fcol; //A sor első oszlopának jelzője
    
    int line; //Aktuális sor
    int col; //Aktuális oszlop
};

csv_writer *openCsvWriter(const char *fname);

void setWriterSeparator(csv_writer *w, char sep);
int newLine(csv_writer *w);
int addCol(csv_writer *w, char *s);
int closeWriter(csv_writer *w);

#endif	/* EXCCEL_CSV_WRITER_H */