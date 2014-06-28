#ifndef EXCCEL_FUNCTION_SOURCE
#define EXCCEL_FUNCTION_SOURCE

#include <string.h>

#include "exccel_function.h"
#include "exccel_errors.h"

/*
 * Globális változó a létrehozott funkciók tárolásához
 */
exccel_function **__initializedFunctions;
int               __initializedFunctionsNum = 0;

exccel_function *initFunction(const char* name, void (*func)(exccel_function* fn), int *sig) {
    exccel_function *f;
    
    f = (exccel_function*)malloc(sizeof(exccel_function));
    f->name = name;
    f->concreteFunction = func;
    f->paramsCount = 0;
    f->signature = sig;
    
    
    if (__initializedFunctionsNum == 0) {
        __initializedFunctions = (exccel_function**)malloc(sizeof(exccel_function));
    } else {
        __initializedFunctions = (exccel_function**)realloc(__initializedFunctions,sizeof(exccel_function) * __initializedFunctionsNum);
    }
    
    *(__initializedFunctions + __initializedFunctionsNum++) = f;
    
    return f;
}

void destroyFunctions() {
    int i = 0;
    while(i<__initializedFunctionsNum) {
        free(__initializedFunctions[i]);
        i++;
    }
    free(__initializedFunctions);
}

void freeFunction(exccel_function *f) {
    int i = 0;
    while(i<f->paramsCount) {
        freeExccelVar(f->par[i]);
        i++;
    }
    free(f->par);
}

exccel_function **getFunctions() {
    if (__initializedFunctionsNum) {
        return __initializedFunctions;
    } else {
        return NULL;
    }
}

int getFunctionsCount() {
    return __initializedFunctionsNum;
}

exccel_function *getExccelFunction(const char *name) {
    int i = 0;
    int n = getFunctionsCount();
    exccel_function *copyed;
    const char *fnName;

    while(i<n) {
        fnName = __initializedFunctions[i]->name;
        if (strcmp(name,fnName) == 0) {
            copyed = (exccel_function*)malloc(sizeof(exccel_function));
            copyed->paramsCount = 0;
            copyed->par = NULL;
            copyed->ret = NULL;
            copyed->name =             __initializedFunctions[i]->name;
            copyed->signature =        __initializedFunctions[i]->signature;
            copyed->concreteFunction = __initializedFunctions[i]->concreteFunction;
            return copyed;
        }
        i++;
    }
    
    exccel_fatal_error(ERR_EXCCEL_FUNCTION_NOT_EXISTS);
    
    return NULL;
}

int isFunctionString(const char *s) {
    int i = 0;
    int n = getFunctionsCount();
    
    while(i<n) {
        if (strcmp(__initializedFunctions[i]->name,s) == 0) {
            return 1;
        }
        i++;
    }
    
    return 0;
}

int addParamToFunction(exccel_function* fn, exccel_var *param, int index) {
    int paramsCount = fn->paramsCount;
    
    if (paramsCount == 0) {
        fn->par = (exccel_var**)calloc(1,sizeof(exccel_var));
        fn->paramIndexes = (int*)calloc(1,sizeof(int));
    } else {
        fn->par = (exccel_var**)realloc(fn->par,sizeof(exccel_var) * (paramsCount + 1));
        fn->paramIndexes = (int*)realloc(fn->paramIndexes,sizeof(int) * (paramsCount + 1));
    }
    
    fn->par[paramsCount] = param;
    fn->paramIndexes[paramsCount] = index;
    
    paramsCount++;
    
    fn->paramsCount = paramsCount;
    return 1;
}

exccel_var *getFunctionParam(exccel_function *fn, int index) {
    int pc = fn->paramsCount;
    int i  = 0;
    
    while(i<pc) {
        if (fn->paramIndexes[i] == index) {
            return fn->par[i];
        }
        i++;
    }
    
    return NULL;
}

int assignReturnValueToFunction(exccel_function* fn, exccel_var *ret) {
    fn->ret = ret;
    return 1;
}

int runFunction(exccel_function* fn, table_cell *c, table *t) {
    int i = 0;
    
    fn->cell  = c; 
    fn->table = t;
    fn->concreteFunction(fn);
    
    for (; i<fn->paramsCount; i++) {
        freeExccelVar(fn->par[i]);
    }
    
    free(fn->par);
    
    return 1;
}


exccel_var *getReturnValue(exccel_function* fn) {
    return fn->ret;
}

#endif
