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
        fprintf ( log_file, "%lu, ", list->next[i] );
    fprintf ( log_file ,"\n" );

    fprintf ( log_file ,"prev: " );
    for ( size_t i = 0; i < list->capacity; i++ )
        fprintf ( log_file, "%lu, ", list->prev[i] );
    fprintf ( log_file ,"\n" );

    fprintf ( log_file, "<h3>[IMG]:</h3>" );

    CreateGraphImg ( list );

    fprintf ( log_file, "<img "
                        "src = \"graph.jpg\""
                        "style=\"width: 80vw; height: auto; max-width: 100%%;\" >" );

    fclose ( log_file );

    return 0;

}



void CreateGraphImg ( List_t* list ) {

    FILE* graph_text = fopen ( "graph.txt", "w" );

    fprintf ( graph_text, 
              "digraph structs {\n"
              "   rankdir = LR;\n"
              "   edge [weight = 100, color = \"white\"]\n"
              "   first [shape = Mrecord , label = \" root | <head> head: %lu | <tail> tail: %lu \"]\n",
              list->head, list->tail );
    
    for ( size_t i = 0; i < list->capacity; i++ ) {
        fprintf ( graph_text, 
                  "   node_%lu [shape = Mrecord , label = \" data: %li | <next> next: %lu | <prev> prev: %lu \"]\n",
                  i, list->data[i], list->next[i], list->prev[i]);
    }

    fprintf ( graph_text, "   first ->" );

    for ( size_t i = 0; i <list->capacity; i++ ) {

        fprintf ( graph_text, " node_%lu ->", i );

        if ( i == list->capacity - 1 )
            fprintf ( graph_text, " node_%lu;\n", i );
    }

    fprintf ( graph_text,
              "   edge [weight = 10, color = \"green\"]\n"
              "   first: <head> -> node_%lu;\n"
              "   edge [weight = 10, color = \"red\"]\n"
              "   first: <tail> -> node_%lu;\n"
              "   edge [weight = 1, color = \"blue\"]\n"
              "   free_p [shape = Mrecord , color = \"blue\", lable = \"free: %lu\"]\n"
              "   free_p -> node_%lu: <next>;\n",
              list->head, list->tail, list->free, list->free );

    /*Arrows for next*/

    fprintf ( graph_text, 
              "\n/*Arrows for next*/\n\n"
              "   edge [weight = 10, color = \"blue\"]\n" );

    for ( size_t i = 1; i < list->capacity; i++ ) {

        if ( i >= list->capacity - 1 ) break;
        fprintf ( graph_text, "   node_%lu: <next> -> node_%lu\n: <next>", i, list->next[i] );

    }

    /*Arrows for prev*/

    fprintf ( graph_text, 
              "\n/*Arrows for prev*/\n\n"
              "   edge [weight = 10, color = \"red\"]\n" );

    for ( size_t i = 1; i < list->capacity; i++ ) {

        if ( list->prev[i] == POISON_VALUE ) continue;
        if ( i >= list->capacity - 1 ) break;
        fprintf ( graph_text, "   node_%lu: <prev> -> node_%lu\n: <prev>", i, list->prev[i] );

    }

    fprintf ( graph_text, "}" );
    fclose ( graph_text );
    system ( "dot -Tjpg -Gsplines=polyline graph.txt -o graph.jpg" );

}