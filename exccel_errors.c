#ifndef exccel_errors_source
#define	exccel_errors_source

#include "exccel_errors.h"
#include <stdlib.h>
#include <stdio.h>

void exccel_error(const char* msg) {
    printf("Hiba Tortent!\n");
    printf("%s\n",msg);
}

void exccel_fatal_error(const char *msg) {
    printf("Hiba Tortent!\n");
    printf("%s\n",msg);
    exit(EXIT_FAILURE);
}

#endif