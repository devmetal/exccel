#ifndef EXCCEL_CSV_PARSER_SOURCE
#define	EXCCEL_CSV_PARSER_SOURCE

#include "exccel_csv_parser.h"
#include "exccel_errors.h"
#include <stdio.h>
#include <stdlib.h>

int _hasStateOpen(csv_file *f) {
    return f != NULL && f->state == 1;
}

csv_file *csvFopen(const char *name) {
    FILE *f;
    csv_file *csv;
    
    f = fopen(name, "r");
    if (f == NULL) {
        exccel_fatal_error(ERR_CSV_READER_FILE_NOT_FOUND);
    }
    
    csv = (csv_file*)malloc(sizeof(csv_file));
    csv->col = 0;
    csv->row = 0;
    csv->f = f;
    csv->currentCol = NULL;
    csv->currentRow = NULL;
    csv->state = 1;
    csv->sequenced = 0;
    
    setSeparator(csv,";\0");
    setStringSequencer(csv,"\"\0");
    
    return csv;
}

int setSeparator(csv_file *csv, char *s) {
    int n = 0;
    
    if (!_hasStateOpen(csv)) {
        return 0;
    }
    
    while(s[n] != '\0') {
        n++;
    }
    
    csv->sep.value = s;
    csv->sep.n = n;
    
    return 1;
}

int setStringSequencer(csv_file *csv, char *s) {
    int n = 0;
    
    if (!_hasStateOpen(csv)) {
        return 0;
    }
    
    while(s[n] != '\0') {
        n++;
    }
    
    csv->strSeq.value = s;
    csv->strSeq.n     = n;
    return 1;
}

int hasNextCol(csv_file *csv) {
    if (!_hasStateOpen(csv)) {
        return 0;
    }
    
    return (csv->currentColPos <= csv->curentRowLength);
}

int hasNextRow(csv_file *csv) {
    if (!_hasStateOpen(csv)) {
        return 0;
    }
    
    return (!feof(csv->f));
}

void nextRow(csv_file *csv) {
    char *row;
    char c;
    FILE *f;
    int n;
    int s;
    
    if (!_hasStateOpen(csv)) {
        return;
    }
    
    if (csv->currentRow != NULL) {
        free(csv->currentRow);
    }
    
    n = 0;
    s = 128;
    row = (char*)calloc(s, sizeof(char*));
    
    f = csv->f;
    while((c = fgetc(f)) != '\n' && c != EOF) {
        *(row + n) = c;
        n++;
        if (n>=s) {
            s += 128;
            row = (char*)realloc(row,sizeof(char*) * s);
        }
    }
    *(row + n) = '\0';
    
    csv->currentRow = row;
    csv->curentRowLength = n;
    csv->row++;
    
    csv->col = 0;
    csv->c = 0;
    csv->currentColPos = 0;
    
    return;
}

char* nextCol(csv_file *csv) {
    char *row = csv->currentRow;
    int  crl  = csv->curentRowLength;
    int  cps  = csv->currentColPos;
    
    char *strSep = csv->sep.value;
    char *strSeq = csv->strSeq.value;
    int  strSeqN = csv->strSeq.n;
    int  strSepN = csv->sep.n;
    int  sequenced = 0;
    int  separated = 0;
    
    int bufferSize = 8;
    int n = 0;
    char *col;
    char c;
    
    if (!_hasStateOpen(csv)) {
        return NULL;
    }

    col = (char *)malloc(bufferSize);
    
    while ((c = *(row + cps)) != '\0' && (separated != strSepN || csv->sequenced) && cps < crl && c != '\n') {
        if (c == strSeq[sequenced]) {
            sequenced++;
            
            if (sequenced == strSeqN) {
                csv->sequenced = !csv->sequenced;
            }
            
            cps++;
        } else {
            sequenced = 0;
            
            if (c == strSep[separated] && !csv->sequenced) {
                separated++;
            } else {
                separated = 0;
                
                *(col + n++) = c;

                if (n >= bufferSize) {
                    bufferSize += 8;
                    col = (char *)realloc(col,bufferSize);
                }
                
                cps++;
            }
        }
        
        
    }
    
    csv->c = *(row + cps++);
    
    *(col + n) = '\0';
    
    csv->currentColPos = cps;
    csv->currentCol = col;
    csv->currentColLength = n;
    csv->col++;
    
    return col;
}

int getColNum(csv_file *csv) {
    if (!_hasStateOpen(csv)) {
        return 0;
    }
    return csv->col;
}

int getRowNum(csv_file *csv) {
    if (!_hasStateOpen(csv)) {
        return 0;
    }
    return csv->row;
}

int close(csv_file *csv) {
    if (!_hasStateOpen(csv)) {
        return 0;
    }
    
    fclose(csv->f);
        
    if (csv->currentRow != NULL) {
        free(csv->currentRow);
    }

    free(csv);
    return 1;
}

void getRowsAndCols(const char *fname, int *row, int *col) {    
    csv_file *csv = csvFopen(fname);
    char *tmp;
    int c = 0, r = 0;
    
    if (hasNextRow(csv)) {
        r++;
        nextRow(csv);
        while(hasNextCol(csv)) {
            tmp = nextCol(csv);
            c++;
            free(tmp);
        }
    }
    
    while(hasNextRow(csv)) {
        nextRow(csv);
        r++;
    }
    
    close(csv);
    
    *row = r;
    *col = c;
}

#endif