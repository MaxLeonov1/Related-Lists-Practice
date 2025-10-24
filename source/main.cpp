#include <stdio.h>
#include <stdlib.h>

#include "rellist.h"



int main() {

    INIT_LIST( list_1 )
    size_t capacity = 5;

    ListCtor ( &list_1, capacity );

    ListDtor ( &list_1 );

    return 0;
}
