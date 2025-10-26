#include <stdlib.h>
#include <stdio.h>

#include "rellist.h"



List_Err_t ListCtor ( List_t* list ,size_t capacity ) {

    list->data = (List_Elem_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->data == nullptr ) return List_Err_t::MEM_ALLOC_ERR;
    for ( size_t i = 1; i < capacity; i++ )
        list->data[i] = POISON_VALUE;

    list->capacity = capacity;
    list->head = 0;
    list->free = 1;
    list->tail = 0;

    list->next = (size_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->next == nullptr ) return List_Err_t::MEM_ALLOC_ERR;
    for ( size_t i = 1; i < capacity; i++ )
        list->next[i] = i + 1;

    list->prev = (size_t*) calloc ( capacity, sizeof(List_Elem_t) );
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



List_Err_t AddToScPos ( List_t* list, List_Elem_t elem, size_t pos ) {

    size_t elem_pos = list->free;
    list->data[elem_pos] = elem;
    list->free = list->next[list->free];

    if ( list->tail == list->head && list->tail == 0 ) {

        list->tail = elem_pos;
        list->head = elem_pos;

        list->next[elem_pos] = 0;
        list->prev[elem_pos] = 0;

        return List_Err_t::LST_SUCCSESSFUL;

    } else if ( list->tail == pos ) {

        list->tail = elem_pos;

        list->next[elem_pos] = 0;
        list->prev[elem_pos] = pos;

        list->next[pos] = elem_pos;

        return List_Err_t::LST_SUCCSESSFUL;

    } else if ( list->data[pos] != POISON_VALUE ) {

        list->next[elem_pos] = list->next[pos];
        list->prev[elem_pos] = pos;

        list->next[pos] = elem_pos;
        list->prev[list->next[pos]] = elem_pos;

        return List_Err_t::LST_SUCCSESSFUL;

    } else return List_Err_t::OUT_OF_BOUNDS_ERR;

}



List_Err_t AddToPos ( List_t* list, List_Elem_t elem, size_t pos ) {

    size_t elem_pos = list->free;
    list->data[elem_pos] = elem;
    list->free = list->next[list->free];

    if ( list->tail == list->head && list->tail == 0 ) {

        list->tail = elem_pos;
        list->head = elem_pos;

        list->next[elem_pos] = 0;
        list->prev[elem_pos] = 0;

        return List_Err_t::LST_SUCCSESSFUL;

    } else if ( list->head == pos ) {

        list->head = elem_pos;

        list->next[elem_pos] = pos;
        list->prev[elem_pos] = 0;

        list->prev[pos] = elem_pos;

        return List_Err_t::LST_SUCCSESSFUL;

    } else if ( list->data[pos] != POISON_VALUE ) {

        list->next[elem_pos] = pos;
        list->prev[elem_pos] = list->prev[pos];

        list->next[list->prev[pos]] = elem_pos;
        list->prev[pos] = elem_pos;

        return List_Err_t::LST_SUCCSESSFUL;

    } else return List_Err_t::OUT_OF_BOUNDS_ERR;

}