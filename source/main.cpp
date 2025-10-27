#include <stdio.h>
#include <stdlib.h>

#include "rellist.h"



int main() {

    INIT_LIST( list_1 )
    size_t capacity = 5;

    ListCtor ( &list_1, capacity );

    AddToPos ( &list_1, 10, 1 );
    AddToScPos ( &list_1, 20, 1 );
    AddToScPos ( &list_1, 30, 2 );
    AddToScPos ( &list_1, 40, 3 );
    AddToPos ( &list_1, 35, 3 );
    ListDump ( &list_1 );

    DelFromPos ( &list_1, 3 );
    ListDump ( &list_1 );

    ListDtor ( &list_1 );

    return 0;
}
