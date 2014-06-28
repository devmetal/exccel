#ifndef exccel_errors_header
#define	exccel_errors_header

#define ERR_CSV_READER_FILE_NOT_FOUND "A csv fajl nem talalhato\0"
#define ERR_CSV_READER_STATE_ERROR "A csv olvaso inicializalasa nem sikerult\0"
#define ERR_CSV_WRITER "A kimeneti fajl nem irahto\0"
#define ERR_EXCCEL_FUNCTION_NOT_EXISTS "A kivant exccel fuggveny nem talalhato\0"
#define ERR_EXCCEL_FUNCTION_INVALID_PARAM "The current exccel function parameter is invalid!\0"
#define ERR_EXCCEL_RANGE_ERROR "Invalid cell range!\0"
#define ERR_EXCCEL_CELL_ERROR "Invalid cell!\0"

void exccel_error(const char *msg);
void exccel_fatal_error(const char *msg);
#endif	/* EXCCEL_ERRORS_H */