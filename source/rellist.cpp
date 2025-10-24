#include <stdlib.h>
#include <stdio.h>

#include "rellist.h"



List_Err_t ListCtor ( List_t* list ,size_t capacity ) {

    list->data = (List_Elem_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->data == nullptr ) return List_Err_t::MEM_ALLOC_ERR;
    for ( size_t i = 1; i < capacity; i++ )
        list->data[i] = POISON_VALUE;

    list->head = 0;
    list->free = 1;

    list->next = (List_Elem_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->next == nullptr ) return List_Err_t::MEM_ALLOC_ERR;
    for ( size_t i = 1; i < capacity; i++ )
        list->next[i] = (long)(i + 1);

    list->prev = (List_Elem_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->prev == nullptr ) return List_Err_t::MEM_ALLOC_ERR;
    for ( size_t i = 1; i < capacity; i++ )
        list->prev[i] = POISON_VALUE;

    return List_Err_t::LST_SUCCSESSFUL;

};



List_Err_t ListDtor ( List_t* list ) {

    list->head = 0;
    list->free = 0;

    free ( list->data );
    free ( list->next );
    free ( list->prev );

    return List_Err_t::LST_SUCCSESSFUL;

}