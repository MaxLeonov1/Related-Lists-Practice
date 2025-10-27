#include <stdlib.h>
#include <stdio.h>

#include "rellist.h"



List_Err_t ListCtor ( List_t* list ,size_t capacity ) {

    list->data = (List_Elem_t*) calloc ( capacity, sizeof(List_Elem_t) );
    if ( list->data == nullptr ) return List_Err_t::MEM_ALLOC_ERR;
    for ( size_t i = 1; i < capacity; i++ )
        list->data[i] = POISON_VALUE;

    list->capacity = capacity;
    list->free = 1;

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

    list->free = 0;

    free ( list->data );
    free ( list->next );
    free ( list->prev );

    return List_Err_t::LST_SUCCSESSFUL;

}



List_Err_t AddToScPos ( List_t* list, List_Elem_t elem, size_t pos ) {

    if ( pos >= list->capacity - 2 )
        AllocMem ( list );

    size_t elem_pos = list->free;
    list->data[elem_pos] = elem;
    list->free = list->next[list->free];

    if ( list->prev[0] == list->next[0] && list->prev[0] == 0 ) {

        list->prev[0] = elem_pos;
        list->next[0] = elem_pos;

        list->next[elem_pos] = 0;
        list->prev[elem_pos] = 0;

        return List_Err_t::LST_SUCCSESSFUL;

    } else if ( list->prev[0] == pos ) {

        list->prev[0] = elem_pos;

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

    if ( pos >= list->capacity - 1 )
        AllocMem ( list );

    size_t elem_pos = list->free;
    list->data[elem_pos] = elem;
    list->free = list->next[list->free];

    if ( list->prev[0] == list->next[0] && list->prev[0] == 0 ) {

        list->prev[0] = elem_pos;
        list->next[0] = elem_pos;

        list->next[elem_pos] = 0;
        list->prev[elem_pos] = 0;

        return List_Err_t::LST_SUCCSESSFUL;

    } else if ( list->next[0] == pos ) {

        list->next[0] = elem_pos;

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



List_Err_t DelFromPos ( List_t* list, size_t pos ) {

    if ( list->prev[0] == list->next[0] ) {

        if ( list->prev[0] == 0 ) return List_Err_t::EMPTY_LIST_DEL_ERR;

        list->next[0] = 0;
        list->prev[0] = 0;

        RESET_POS ( list, pos )
        return List_Err_t::LST_SUCCSESSFUL;

    } else if ( list->next[0] == pos ) {

        list->next[0] = list->next[pos];
        list->prev[list->next[pos]] = 0;

        RESET_POS ( list, pos )
        return List_Err_t::LST_SUCCSESSFUL;
    
    } else if ( list->prev[0] == pos ) {

        list->prev[0] = list->prev[pos];
        list->next[list->prev[pos]] = 0;

        RESET_POS ( list, pos )
        return List_Err_t::LST_SUCCSESSFUL;
        
    } else if ( list->data[pos] != POISON_VALUE ) {

        list->next[list->prev[pos]] = list->next[pos];
        list->prev[list->next[pos]] = list->prev[pos];

        RESET_POS ( list, pos )
        return List_Err_t::LST_SUCCSESSFUL;

    } else return List_Err_t::OUT_OF_BOUNDS_ERR; 


}



List_Err_t AllocMem ( List_t* list ) {

    list->data = (List_Elem_t*) realloc ( list->data, (list->capacity*2) * sizeof(List_Elem_t) );
    if ( list->data == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    list->next = (size_t*) realloc ( list->next, (list->capacity*2) * sizeof(size_t) );
    if ( list->next == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    list->prev = (size_t*) realloc ( list->prev, (list->capacity*2) * sizeof(size_t) );
    if ( list->prev == nullptr ) return List_Err_t::MEM_ALLOC_ERR;

    for ( size_t i = list->capacity; i < list->capacity*2; i++ ) {

        list->data[i] = POISON_VALUE;
        list->prev[i] = POISON_VALUE;
        list->next[i] = i + 1;

    }

    list->capacity = list->capacity*2;

    return List_Err_t::LST_SUCCSESSFUL;

}