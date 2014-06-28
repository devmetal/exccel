#ifndef EXCCEL_FUNCTION_HEADER
#define EXCCEL_FUNCTION_HEADER

#define INIT_FUNCTION(name,fn,sig) initFunction(#name"\0",fn,sig)

#include "exccel_types.h"

int getFunctionsCount();

exccel_function **getFunctions();
exccel_function *getExccelFunction(const char* name);
exccel_function *initFunction(const char* name, void (*func)(exccel_function* fn), int *sig);

void freeFunction(exccel_function *f);
void destroyFunctions();

int isFunctionString(const char *s);

int        addParamToFunction(exccel_function* fn, exccel_var *param, int index);
exccel_var *getFunctionParam(exccel_function *fn, int index);
exccel_var *getReturnValue(exccel_function* fn);

int assignReturnValueToFunction(exccel_function* fn, exccel_var *ret);
int runFunction(exccel_function* fn, table_cell *c, table *t);

#endif