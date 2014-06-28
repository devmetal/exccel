#ifndef EXCCEL_TYPES_HEADER
#define	EXCCEL_TYPES_HEADER

#include<stdio.h>
#include<stdlib.h>

struct _table;
struct _table_cell;
struct _table_cell_matrix;
struct _cell_processing;
struct _exccel_function;
union  _exccel_var_data;
struct _exccel_var;
struct _exccel_var_data_range;

typedef struct _table             table;
typedef struct _table_cell        table_cell;
typedef struct _table_cell_matrix table_cell_matrix;
typedef struct _cell_processing   cell_process;
typedef struct _exccel_function   exccel_function;
typedef union  _exccel_var_data   exccel_var_data;
typedef struct _exccel_var        exccel_var;

#define FN_SIG_RANGE   32 //Cellatartomány vagy cella (A1:A2)
#define FN_SIG_LIST    16 //Lista (1,2,3,FN())
#define FN_SIG_LITERAL 8  //Egy érték (3||FN()||A1)

/**
 * Felsorolás amely jelzőként szolgál,
 * arra, hogy aktuálisan milyen típus
 * található egy exccel_var_data únióban
 */
typedef enum {
    VAR_STRING,
    VAR_INT,
    VAR_DOUBLE,
    VAR_RANGE,
    VAR_CELL,
} exccel_var_data_type;

/*
A táblázatot leíró adatszerkezet
*/
struct _table {
    table_cell_matrix *matrix;
    struct {
        const char * value;
        int len;	
    } name;
};

/**
 * Az exccel program egy adott celláját
 * képviseli a táblázatban.
 */
struct _table_cell {
    int col;
    int row;
    exccel_var   *data;
    cell_process *cp;
};

/**
 * Egy mátrixot leíró szerkezet,
 * amely a cellákat tárolja
 */
struct _table_cell_matrix {
    table_cell ***cells;
    int row;
    int col;
};

/**
 * A cella feldolgozási láncolt lista
 * alapstruktúrája. Ebben tárolom,
 * a végrehajtásra váró függvényeket.
 */
struct _cell_processing {
    table *t;
    table_cell *cell;
    exccel_function *func;
    
    int paramPos; 
    struct _cell_processing *next;
    struct _cell_processing *childs;
    
    int proceed;
};

/**
 * A cellatartomány típus struktúrája
 */
struct _exccel_var_data_range {
    int startRow;
    int startCol;
    int endRow;
    int endCol;
    int oneCell;
};

/**
 * Egy adott exccel függvényt leíró struktúra,
 * névvel, paraméterekkel és visszatérési értékekkel,
 * plusz egy függvénymutatóval, amely a konkrét
 * algoritmus implementációja 
 */
struct _exccel_function {
    const char *name;
    
    table *table;
    table_cell *cell;
    exccel_var *ret;
    exccel_var **par;
    
    int *paramIndexes;
    int  paramsCount;
    int *signature;
    
    void (*concreteFunction)(exccel_function *exccelFunction);
};

/**
 * Bármilyen dinamikus típusú elemnek,
 * ilyen únió hordozza az adatát.
 * 
 * Külön nem használható, csak az exccel_var
 * struktúrával együtt. 
 */
union _exccel_var_data {
    int ival;
    double dval;
    
    struct {
        char *val;
        int len;	
    } sval;
    
    struct _exccel_var_data_range range;
};

/**
 * Az exccel_var típus fogja össze
 * a data_type és var_data szerkezeteket.
 */
struct _exccel_var {
    exccel_var_data      data;
    exccel_var_data_type type;
};


exccel_var *_allocate();
void       freeExccelVar(exccel_var *v);

exccel_var* _setExcelVarTo(exccel_var *var, exccel_var_data data, exccel_var_data_type type);

exccel_var* createExccelVarFromString(char *s);
exccel_var* createExccelIntVar(int value);
exccel_var* createExccelDoubleVar(double value);
exccel_var* createExccelStringVar(char *s);
exccel_var* createExccelRangeVarFromString(char *s);
exccel_var* createExccelRangeVar(int startRow, int startCol, int endRow, int endCol);

int         getExccelVarIntValue(exccel_var *v);
double      getExccelVarDoubleValue(exccel_var *v);
char*       getExccelVarStringValue(exccel_var *v);

struct _exccel_var_data_range 
            *getExccelVarRangeValue(exccel_var *v);

exccel_var* getExccelVarCellLinkValue(exccel_var *v);

exccel_var_data_type getExccelVarDataType(exccel_var* var);

#endif