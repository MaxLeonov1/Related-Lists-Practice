#ifndef __REL_LIST__
#define __REL_LIST__

#include <stdlib.h>

#include "rellist_err.h"



typedef long List_Elem_t;
const List_Elem_t POISON_VALUE = 0xFEE1DEAD;

typedef struct {

    List_Elem_t* data;
    long head;
    long free;

    long* next;
    long* prev;

} List_t;

#define INIT_LIST( name ) List_t name = { \
    .data = nullptr,                      \
    .head = 0,                            \
    .free = 0,                            \
    .next = nullptr,                      \
    .prev = nullptr,                      \
};



List_Err_t ListCtor ( List_t* list ,size_t capacity );
List_Err_t ListDtor ( List_t* list );


#endif //__REL_LIST__