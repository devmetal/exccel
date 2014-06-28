#ifndef EXCCEL_CSV_PARSER_HEADER
#define	EXCCEL_CSV_PARSER_HEADER

#include<stdio.h>
#include<stdlib.h>

struct _csv_file;

typedef struct _csv_file csv_file;

struct _csv_file {
    FILE *f; //Belső file pointer
    int state; //0 vagy 1 vagy -1
    
    int col; //Aktuális oszlopszám
    int row; //Aktuális sorszám
    
    int curentRowLength; //Aktuális sor hossza
    char *currentRow; //Aktuális sor
    
    int currentColLength; //Aktuális oszlop hossza (elválasztó nélkül)
    char *currentCol; //Aktuális oszlop
    
    struct {
        char *value;
        int n;
    } strSeq; //Karakterlánc elválasztó
    
    int sequenced;
    
    struct {
        char *value;
        int n;
    } sep; //elválasztó
    
    char c; //Legutoljára beolvasott karakter
    int currentColPos;
};

csv_file *csvFopen(const char *name);
int setSeparator(csv_file *csv, char *s);
int setStringSequencer(csv_file *csv, char *s);

int hasNextCol(csv_file *csv);
int hasNextRow(csv_file *csv);

void nextRow(csv_file *csv);
char* nextCol(csv_file *csv);

int getColNum(csv_file *csv);
int getRowNum(csv_file *csv);

void getRowsAndCols(const char *fname, int *row, int *col);

int close(csv_file *csv);

#endif	/* EXCCEL_CSV_PARSER_H */

