#include <stdio.h>
#include <stdlib.h>

#include "rellist.h"



int ListDump ( List_t* list ) {

    FILE* log_file = fopen ( "list_log.htm", "w" );

    fprintf ( log_file,
        "<pre>\n" 
        "<h3>[LIST INFO]:</h3>\n"
        "name: %s\n"
        "location: %s::%d\n"
        "<h3>[LIST DATA]:</h3>\n"
        "capacity:\n",
        list->info.name,
        list->info.file,
        list->info.line );

    fprintf ( log_file ,"data: " );
    for ( size_t i = 0; i < list->capacity; i++ )
        fprintf ( log_file, "%ld, ", list->data[i] );
    fprintf ( log_file ,"\n" );

    fprintf ( log_file ,"next: " );
    for ( size_t i = 0; i < list->capacity; i++ )
        fprintf ( log_file, "%ld, ", list->next[i] );
    fprintf ( log_file ,"\n" );

    fprintf ( log_file ,"prev: " );
    for ( size_t i = 0; i < list->capacity; i++ )
        fprintf ( log_file, "%ld, ", list->prev[i] );
    fprintf ( log_file ,"\n" );

    fprintf ( log_file, "<h3>[IMG]:</h3>" );


    fclose ( log_file );

    return 0;

}



void CreateGraphImg ( List_t* list ) {



}