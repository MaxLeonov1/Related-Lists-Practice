#include <stdio.h>
#include <stdlib.h>

#include "rellist.h"



int main() {

    INIT_LIST( list_1 )
    size_t capacity = 5;

    ListStatusHandler ( ListCtor ( &list_1, capacity ) );

    ListStatusHandler ( InsertAfter( &list_1, 10, 0 ) );
    ListStatusHandler ( InsertAfter( &list_1, 20, 1 ) );
    ListStatusHandler ( Insert( &list_1, 5, 1 ) );
    ListDump( &list_1 );

    ListStatusHandler ( Delete( &list_1, 2 ) );
    ListDump( &list_1 );

    ListStatusHandler ( InsertAfter( &list_1, 9, 3 ) );
    ListStatusHandler ( InsertAfter( &list_1, 8, 3 ) );
    ListDump( &list_1 );

    
    ListDtor ( &list_1 );

    return 0;
}
