#include <stdio.h>
#include <stdlib.h>

#include "rellist.h"



int main() {

    INIT_LIST( list_1 )
    size_t capacity = 5;
    const char* log_dir = "/mnt/c/Users/Max/C_Projects/Related-Lists-Practice/logs";

    ListStatusHandler ( ListCtor ( &list_1, capacity ) );

    ListStatusHandler ( AddToPos ( &list_1, 10, 1 ) );
    ListStatusHandler ( AddToScPos ( &list_1, 20, 1 ) );
    ListStatusHandler ( AddToScPos ( &list_1, 30, 2 ) );
    ListStatusHandler ( AddToScPos ( &list_1, 40, 3 ) );
    ListStatusHandler ( AddToPos ( &list_1, 35, 3 ) );
    ListStatusHandler ( ListDump ( &list_1, log_dir ) );

    ListStatusHandler ( DelFromPos ( &list_1, 3 ) );
    ListStatusHandler ( ListDump ( &list_1, log_dir ) );

    ListDtor ( &list_1 );

    return 0;
}
