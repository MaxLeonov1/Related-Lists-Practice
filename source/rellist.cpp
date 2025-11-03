#include <stdlib.h>
#include <stdio.h>

#include "rellist.h"



List_Err_t ListCtor ( List_t* list ,size_t capacity ) {

    if ( list->capacity == 0 ) List_Err_t::ZERO_CAPACITY_ERR;

    list->capacity = capacity;
    list->free = 1;

    list->data = (List_Elem_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->data == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    for ( size_t i = 1; i < capacity; i++ )
        DATA(i) = POISON_VALUE;

    list->next = (size_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->next == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    for ( size_t i = 1; i < capacity; i++ ) {
        if ( i == list->capacity - 1 ) NEXT(i) = POISON_VALUE;
        else NEXT(i) = i + 1;
    }

    list->prev = (size_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->prev == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    for ( size_t i = 1; i < capacity; i++ )
        PREV(i) = POISON_VALUE;

    return List_Err_t::LST_SUCCSESSFUL;

};



List_Err_t ListDtor ( List_t* list ) {

    list->free = 0;
    list->capacity = 0;
    list->size = 0;

    free ( list->data );
    free ( list->next );
    free ( list->prev );

    return List_Err_t::LST_SUCCSESSFUL;

}



List_Err_t InsertAfter ( List_t* list, List_Elem_t elem, size_t pos ) {

    if ( pos >= list->capacity - 2 )
        AllocMem ( list );

    if ( DATA(pos) != POISON_VALUE ) {

        size_t elem_pos = FREE_P;
        DATA(elem_pos) = elem;
        FREE_P = NEXT(FREE_P);

        NEXT(elem_pos) = NEXT(pos);
        PREV(NEXT(pos)) = elem_pos;

        NEXT(pos) = elem_pos;
        PREV(elem_pos) = pos;

        list->size++;

        return List_Err_t::LST_SUCCSESSFUL;

    } else return List_Err_t::OUT_OF_BOUNDS_ERR;

}



List_Err_t Insert ( List_t* list, List_Elem_t elem, size_t pos ) {

    if ( pos >= list->capacity - 1 )
        AllocMem ( list );

    if ( DATA(pos) != POISON_VALUE ) {

        size_t elem_pos = FREE_P;
        DATA(elem_pos) = elem;
        FREE_P = NEXT(FREE_P);

        NEXT(elem_pos) = pos;
        NEXT(PREV(pos)) = elem_pos;

        PREV(elem_pos) = PREV(pos);
        PREV(pos) = elem_pos;

        list->size++;

        return List_Err_t::LST_SUCCSESSFUL;

    } else return List_Err_t::OUT_OF_BOUNDS_ERR;

}



List_Err_t Delete ( List_t* list, size_t pos ) {

    if ( pos == 0 ) return List_Err_t::DEL_FROM_NULLPTR_ERR;

    if ( list->data[pos] != POISON_VALUE ) {

        size_t prev_pos = PREV(pos);
        size_t next_pos = NEXT(pos);

        NEXT(prev_pos) = next_pos;
        PREV(next_pos) = prev_pos;

        NEXT(pos) = FREE_P;
        FREE_P = pos;
    
        DATA(pos) = POISON_VALUE;
        PREV(pos) = POISON_VALUE;

        list->size--;

        return List_Err_t::LST_SUCCSESSFUL;

    } else return List_Err_t::OUT_OF_BOUNDS_ERR; 


}



List_Err_t AllocMem ( List_t* list ) {

    NEXT(list->capacity - 1) = list->capacity;

    list->data = (List_Elem_t*) realloc ( list->data, (list->capacity*2) * sizeof(List_Elem_t) );
    if ( list->data == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    list->next = (size_t*) realloc ( list->next, (list->capacity*2) * sizeof(size_t) );
    if ( list->next == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    list->prev = (size_t*) realloc ( list->prev, (list->capacity*2) * sizeof(size_t) );
    if ( list->prev == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    for ( size_t i = list->capacity; i < list->capacity*2; i++ ) {

        DATA(i) = POISON_VALUE;
        PREV(i) = POISON_VALUE;
        if ( i == list->capacity*2 - 1 ) NEXT(i) = POISON_VALUE;
        else NEXT(i) = i + 1;

    }

    list->capacity = list->capacity*2;

    return List_Err_t::LST_SUCCSESSFUL;

}