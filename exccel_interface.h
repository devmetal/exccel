#ifndef exccel_interface_header
#define	exccel_interface_header

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

char *getCsvParam(int argc, char **argv);
char *readNextCsv();
int  fileExists(const char *fname);

void printRes(const char *file);
void printBorder(int n);
void printBordered(const char *s, int top, int bottom);
void about();
void help();



void about() {
    printBorder(13);printf("\n");
    printf("|Authors      |\n");
    printf("|Balint Gyozo |\n");
    printf("|Varga Laszlo |\n");
    printBorder(13);printf("\n");
    
    printBordered("2014 - ExcCel",1,1);
    printf("\n");
}

void help() {
    printBordered("HELP",1,1);
    printf("\n");
    printRes("manual.txt");
    printf("\n");
}

void printBordered(const char *s, int top, int bottom) {
    int n = strlen(s);
    if (top) {printBorder(n);printf("\n");}
    printf("|%s|",s);
    if (bottom) {printf("\n");printBorder(n);}
}

void printBorder(int n) {
    int i = 0;
    printf("+");
    while(i<n) {
        printf("-");
        i++;
    }
    printf("+");
}

void printRes(const char *file) {
    FILE *f;
    char c;
    
    f = fopen(file,"r");
    if (f == NULL) {
        return;
    }
    
    while((c = fgetc(f)) != EOF) {
        printf("%c",c);
    }
    fclose(f);
}

int fileExists(const char* fname) {
    FILE *f;
    if ((f = fopen(fname,"r")) != NULL) {
        fclose(f);
        return 1;
    }
    return 0;
}

char *getCsvParam(int argc, char **argv) {
    char *fname;
    
    if (argc != 2) {
        help();
        return NULL;
    }
    
    fname = argv[1];
    if (!fileExists(fname)) {
        printf("%s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    return fname;
}

char *_getLine(int *s) {
    char *tmp;
    char c;
    int n,buff;
    
    n    = 0;
    buff = 32;
    tmp = calloc(buff,sizeof(char));
    
    while((c = getchar()) != '\n' && c != EOF) {
        tmp[n++] = c;
        if (n>=buff) {
            buff += 32;
            tmp = realloc(tmp,sizeof(char) * buff);
        }
    }
    
    tmp[n] = '\0';
    
    *s = n;
    
    return tmp;
}

char *readNextCsv() {
    char *tmp;
    int  s,r;
    
    r = 0;
    
    printf("\n");
    while(!r) {
        printf("exccel >");
        tmp = _getLine(&s);

        if (s == 0) {
            return NULL;
        } else if (!fileExists(tmp)) {
            printf("%s\n",strerror(errno));
            printf("Probalja meg ujra\n");
        } else {
            r = 1;
        }
    }
    
    return tmp;
}

#endif	/* EXCCEL_INTERFACE_H */

