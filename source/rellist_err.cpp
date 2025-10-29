#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "rellist.h"



void ListStatusHandler ( List_Err_t status ) {

    switch (status) {

        case List_Err_t::LST_SUCCSESSFUL:
            break;
        case List_Err_t::FILE_OPEN_ERR:
            fprintf ( stderr, "[UNABLE TO OPEN FILE]\n" );
            break;
        case List_Err_t::DEL_FROM_NULLPTR_ERR:
            fprintf ( stderr, "[ATTEMPT TO DELETE ROOT ELEMENT]\n" );
            break;
        case List_Err_t::MEM_ALLOC_ERR:
            fprintf ( stderr, "[UNABLE TO ALLOCATE MEMORY]\n" );
            break;
        case List_Err_t::OUT_OF_BOUNDS_ERR:
            fprintf ( stderr, "[OPERATION WITH UNEXISTING LIST ELEMENT]\n" );
            break;

    }

}



void CreateLogDir ( char* dir_name, int call_num ) {
 
    char proj_path[MAX_STR_LEN] = {0};
    time_t log_time_sec = time(NULL);
    struct tm* log_time = localtime( &log_time_sec );
    
    getcwd(proj_path, sizeof(proj_path));
    sprintf ( proj_path, "%s/logs", proj_path );
    mkdir ( proj_path, 0755 ); 

    sprintf ( dir_name,
              "%s/log_%d.%d.%d_%d:%d:%d",
              proj_path,
              log_time->tm_mday,
              log_time->tm_mon,
              1900 + log_time->tm_year,
              log_time->tm_hour,
              log_time->tm_min,
              log_time->tm_sec );
    
    mkdir ( dir_name, 0755 );

}



List_Err_t ListDump ( List_t* list ) {

    List_Err_t status = List_Err_t::LST_SUCCSESSFUL;

    static int call_num = 1;
    char filename[MAX_STR_LEN] = {0};
    char graphname[MAX_STR_LEN] = {0};
    static char log_dir[MAX_STR_LEN] = {0};

    if (call_num == 1) CreateLogDir ( log_dir, call_num );

    snprintf ( filename, sizeof(filename), "%s/list_log.htm", log_dir );
    snprintf ( graphname, sizeof(graphname), "graph_%d.svg", call_num );

    call_num++;

    FILE* log_file = fopen ( filename, "a" );
    if ( log_file == nullptr ) return List_Err_t::FILE_OPEN_ERR;

    fprintf ( log_file,
              "\n<div style=\"height:4px;background:#000\"/>\n" 
              "<pre>\n"
              "<body style=\"background-color: lightblue;\">\n" );

    PrintLogHeader ( list, log_file );
    
    fprintf ( log_file, "<h3>[IMG]:</h3>\n" );

    status =  CreateGraphImg ( list, graphname, log_dir );
    LST_STAT_CHECK

    fprintf ( log_file, "<img "
                        "src = \"%s\" "
                        "style=\"width: 80vw; height: auto; max-width: 100%%;\" >",
                        graphname );

    fclose ( log_file );

    return List_Err_t::LST_SUCCSESSFUL;

}



void PrintLogHeader ( List_t* list, FILE* log_file ) {

    fprintf ( log_file,
        "<h3>[LIST INFO]:</h3>\n"
        "name: %s\n"
        "location: %s::%d, %s()\n"
        "<h3>[LIST DATA]:</h3>\n"
        "capacity: %lu\n",
        list->info.name,
        list->info.file,
        list->info.line,
        list->info.func,
        list->capacity );

    fprintf ( log_file ,"data: " );
    for ( size_t i = 0; i < list->capacity; i++ ) {

        if ( list->data[i] != POISON_VALUE )
            fprintf ( log_file, "%8ld|", list->data[i] );
        else
            fprintf ( log_file, "  POISON|" );
    }
    fprintf ( log_file ,"\n" );


    fprintf ( log_file ,"next: " );
    for ( size_t i = 0; i < list->capacity; i++ )
        fprintf ( log_file, "%8lu|", list->next[i] );
    fprintf ( log_file ,"\n" );


    fprintf ( log_file ,"prev: " );
    for ( size_t i = 0; i < list->capacity; i++ ) {

        if ( list->data[i] != POISON_VALUE )
            fprintf ( log_file, "%8lu|", list->prev[i] );
        else
            fprintf ( log_file, "  POISON|" );

    }
    fprintf ( log_file ,"\n" );

}



List_Err_t CreateGraphImg ( List_t* list, const char* graphname, const char* graph_dir ) {

    char graph_txt_path[MAX_STR_LEN] = {0};
    char graph_svg_path[MAX_STR_LEN] = {0};
    snprintf ( graph_svg_path, sizeof(graph_svg_path), "%s/%s", graph_dir, graphname );
    snprintf ( graph_txt_path, sizeof(graph_txt_path), "%s/graph.txt", graph_dir );

    FILE* graph_text = fopen ( graph_txt_path, "w" );
    if ( graph_text == nullptr ) return List_Err_t::FILE_OPEN_ERR;

    fprintf ( graph_text, 
              "digraph structs {\n"
              "   rankdir = LR;\n"
              "   bgcolor = \"lightblue\""
              "   overlap = \"scale\";\n"
              "   splines = \"ortho\";\n"
              "   node [fontname=\"Helvetica-BoldOblique\", fontsize=\"11\"];\n"
              "   edge [color = \"#00000000\"]\n"
              "   node_0 [shape = Mrecord, style = \"filled,bold\", fillcolor = \"#ff4040\", color = \"#ff8080\","
              "label = \" ROOT | idx: 0 |{ head: %lu | tail: %lu }\"]\n",
              list->next[0], list->prev[0] );
    
    PrintGraphNodes ( list, graph_text );

    fprintf ( graph_text,
              "   edge [color = \"#80ff80\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n"
              "   node_0 -> node_%lu;\n"
              "   edge [color = \"#ff8080\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n"
              "   node_0 -> node_%lu;\n"
              "   edge [color = \"#8080ff\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n"
              "   free [shape = Mrecord , color = \"blue\", lable = \"free: %lu\","
              " style = \"filled,bold\", fillcolor = \"#5050ff\", color = \"#8080ff\"]\n"
              "   free -> node_%lu;\n",
              list->next[0], list->prev[0], list->free, list->free );

    PrintEdgesForNext ( list, graph_text );
    PrintEdgesForPrev ( list, graph_text );
    PrintEdgesForFree ( list, graph_text );

    fprintf ( graph_text, "}" );
    fclose ( graph_text );

    char cmd_line[MAX_STR_LEN] = {0};
    snprintf ( cmd_line, sizeof(cmd_line), "dot -Tsvg %s -o %s", graph_txt_path, graph_svg_path );
    system ( cmd_line );

    return List_Err_t::LST_SUCCSESSFUL;

}



void PrintGraphNodes ( List_t* list, FILE* graph_text ) {

    for ( size_t i = 1; i < list->capacity; i++ ) {

        if ( list->data[i] != POISON_VALUE )
            fprintf ( graph_text, 
                      "   node_%lu [shape = Mrecord , label = \" data: %ld | idx: %lu |{ next: %lu | prev: %lu }\","
                      " style = \"filled,bold\", fillcolor = \"#40ff40\", color = \"#80ff80\"]\n",
                      i, list->data[i], i, list->next[i], list->prev[i]);
        else
            fprintf ( graph_text, 
                      "   node_%lu [shape = Mrecord , label = \" data: POISON | idx: %lu |{ next: %lu | prev: POISON }\","
                      " style = \"filled,bold\", fillcolor = \"#5050ff\", color = \"#8080ff\"]\n",
                      i, i, list->next[i]);

    }

    for ( size_t i = 0; i <list->capacity; i++ ) {

        if ( i == list->capacity - 1 )
            fprintf ( graph_text, " node_%lu;\n", i );
        else
            fprintf ( graph_text, " node_%lu ->", i );

    }

}



void PrintEdgesForNext ( List_t* list, FILE* graph_text ) {

    fprintf ( graph_text, 
              "\n/*Arrows for next*/\n\n"
              "   edge [color = \"#80ff80\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n" );

    for ( size_t i = 1; i <= list->capacity - 1; i++ ) {

        if ( list->next[i] >= list->capacity )
            break;

        else if ( list->data[i] == POISON_VALUE )
            continue;

        else
            fprintf ( graph_text, 
                      "   node_%lu -> node_%lu;\n",
                      i, list->next[i] );

    }

}



void PrintEdgesForPrev ( List_t* list, FILE* graph_text ) {

    fprintf ( graph_text, 
              "\n/*Arrows for prev*/\n\n"
              "   edge [color = \"#ff8080\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n" );

    for ( size_t i = 1; i <= list->capacity - 1; i++ ) {

        if ( list->prev[i] == POISON_VALUE ) continue;
        fprintf ( graph_text, "   node_%lu-> node_%lu;\n", i, list->prev[i] );

    }

}



void PrintEdgesForFree ( List_t* list, FILE* graph_text ) {

    fprintf ( graph_text, 
              "\n/*Arrows for free*/\n\n"
              "   edge [color = \"#8080ff\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n" );

    size_t free_ind = list->free;

    for ( size_t i = 1; i <= list->capacity - 1; i++ ) {

        if ( list->next[free_ind] == list->capacity ) break;
        fprintf ( graph_text, "   node_%lu-> node_%lu;\n", free_ind, list->next[free_ind] );
        free_ind = list->next[free_ind];

    }


}