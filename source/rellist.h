#ifndef __REL_LIST__
#define __REL_LIST__

#include <stdlib.h>

/*=====================================================================================*/

typedef long List_Elem_t;
const List_Elem_t POISON_VALUE = 0xFEE1DEAD;

typedef struct {

    const char* name;
    int line;
    const char* file;

} List_Info_t;

typedef struct {

    List_Info_t info;

    List_Elem_t* data;
    long capacity;
    long head;
    long free;
    long tail;

    long* next;
    long* prev;

} List_t;

#define INIT_LIST(_name) List_t _name = { \
    {#_name, __LINE__, __FILE__},         \
    nullptr, 0, 0, 0, 0, nullptr, nullptr    \
};

/*=====================================================================================*/

typedef enum {

    LST_SUCCSESSFUL = 0,
    MEM_ALLOC_ERR = 1,
    FILE_OPEN_ERR = 2,

} List_Err_t;

/*=====================================================================================*/

int  ListDump       ( List_t* list );
void CreateGraphImg ( List_t* list );

List_Err_t ListCtor ( List_t* list ,size_t capacity );
List_Err_t ListDtor ( List_t* list );


#endif //__REL_LIST__