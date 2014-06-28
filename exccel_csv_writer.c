#ifndef exccel_csv_writer_source
#define exccel_csv_writer_source

#include "exccel_csv_writer.h"
#include "exccel_errors.h"
#include <string.h>


csv_writer *openCsvWriter(const char *fname) {
    csv_writer *w = NULL;
    FILE *f = NULL;
    
    if ((f = fopen(fname,"w"))) {
        w = (csv_writer*)calloc(1,sizeof(csv_writer));
        w->f = f;
        w->sep = ';';
        w->col = 0;
        w->line = (int)'\n';
        w->fcol = 1;
        
        return w;
    } else {
        exccel_fatal_error(ERR_CSV_WRITER);
		return NULL;
    }
}

void setWriterSeparator(csv_writer *w, char sep) {
    w->sep = sep;
}

int newLine(csv_writer *w) {
    int ret = 0;
    
    ret = fprintf(w->f,"\n");
    w->fcol = 1;
    w->line++;
    
    return ret;
}

int addCol(csv_writer *w, char *s) {
    int ret = 0;
    if (!w->fcol) {
        if (!fprintf(w->f,"%c",w->sep)) {
            return 0;
        }
    }

    w->fcol = 0;
    w->col++;

	if (s[0] == '\0')
		return ret;
    
    ret = fprintf(w->f,"%s",s);
    
    return ret;
}

int closeWriter(csv_writer *w) {
    fclose(w->f);
    free(w);
    return 1;
}

#endif