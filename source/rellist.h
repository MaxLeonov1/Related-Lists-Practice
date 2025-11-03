#ifndef __REL_LIST__
#define __REL_LIST__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*=====================================================================================*/

#define FILE_MODE_ 0755
#define MAX_STR_LEN_ 500

#define NEXT(pos) list->next[pos]
#define PREV(pos) list->prev[pos]
#define DATA(pos) list->data[pos]
#define FREE_P list->free

#define EDGE_STD_SET_ "constraint = \"false\", penwidth = 2.0, arrowsize = 0.5"

/*=====================================================================================*/

#define NEXT_EDGE_COL_ "\"#2563eb\""
#define PREV_EDGE_COL_ "\"#ffb3b3\""
#define BOTH_EDGE_COL_ "\"#6b7280\""
#define FREE_EDGE_COL_ "\"#b3b3ff\""
#define BAD_EDGE_COL_  "\"#dc2626\""

/*=====================================================================================*/

typedef long List_Elem_t;
const List_Elem_t POISON_VALUE = 0xFEE1DEAD;

typedef struct {

    const char* name;
    int line;
    const char* file;
    const char* func;

} List_Info_t;

typedef struct {

    List_Info_t info;

    List_Elem_t* data;
    size_t capacity;
    size_t size;
    size_t free;

    size_t* next;
    size_t* prev;

} List_t;

#define INIT_LIST(_name) List_t _name = { \
    {#_name, __LINE__, __FILE__, __FUNCTION__},         \
    nullptr, 0, 0, 0, nullptr, nullptr    \
};

/*=====================================================================================*/

typedef enum {

    LST_SUCCSESSFUL = 0,
    MEM_ALLOC_ERR = 1,
    FILE_OPEN_ERR = 2,
    OUT_OF_BOUNDS_ERR = 3,
    DEL_FROM_NULLPTR_ERR = 4,
    ZERO_CAPACITY_ERR = 5,
    LIST_T_NULL_ERR = 6,
    DATA_NULL_ERR = 7,
    NEXT_NULL_ERR = 8,
    PREV_NULL_ERR = 9,
    INCOR_FREE_P_ERR = 10,
    SIZE_MORE_CAPACITY_ERR = 11,
    UNCYCLED_NEXT_ERR = 12,
    UNCYCLED_PREV_ERR = 13,
    INCOR_FREE_ORDER_ERR = 14,

} List_Err_t;

/*=====================================================================================*/

void       ListStatusHandler ( List_Err_t status );
List_Err_t ListVerify        ( List_t* list );
List_Err_t ListDump          ( List_t* list );
void       CreateLogDir      ( char* dir_name, int call_num );
void       PrintLogHeader    ( List_t* list, FILE* log_file );
List_Err_t CreateGraphImg    ( List_t* list, const char* graphname, const char* graph_dir );
void       PrintGraphNodes   ( List_t* list, FILE* graph_text );
void       PrintEdges        ( List_t* list, FILE* graph_text );

/*=====================================================================================*/

List_Err_t ListCtor    ( List_t* list ,size_t capacity );
List_Err_t ListDtor    ( List_t* list );
List_Err_t InsertAfter ( List_t* list, List_Elem_t elem, size_t pos );
List_Err_t Insert      ( List_t* list, List_Elem_t elem, size_t pos );
List_Err_t Delete      ( List_t* list, size_t pos );
List_Err_t AllocMem    ( List_t* list );

/*=====================================================================================*/

#define LST_STAT_CHECK if ( status != List_Err_t::LST_SUCCSESSFUL ) return status;
// #define VERIF_LIST( list ) ListStatusHandler( ListVerify( &list ) );

/*=====================================================================================*/

#endif //__REL_LIST__