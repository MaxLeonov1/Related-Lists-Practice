#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "rellist.h"

/*=====================================================================================*/

List_Err_t ListVerify ( List_t* list ) {

    if ( list == nullptr ) return List_Err_t::LIST_T_NULL_ERR;
    if ( list->capacity == 0 ) return List_Err_t::ZERO_CAPACITY_ERR;
    if ( list->free == 0 || list->free > list->capacity ) return List_Err_t::INCOR_FREE_P_ERR;
    if ( list->size >= list->capacity ) return List_Err_t::SIZE_MORE_CAPACITY_ERR;

    if ( list->next == nullptr ) return List_Err_t::NEXT_NULL_ERR;
    if ( list->data == nullptr ) return List_Err_t::DATA_NULL_ERR;
    if ( list->prev == nullptr ) return List_Err_t::PREV_NULL_ERR;

    size_t idx = 0;
    size_t idx_test = 0;

    while(1) {

        if ( idx_test == 0 &&
           ( idx > 0 && idx < list->capacity ) ) break;

        if ( idx >= list->capacity ||
             idx_test >= list->capacity ) return List_Err_t::UNCYCLED_NEXT_ERR;

        idx_test = NEXT(idx_test);
        idx++;

    } 
    idx = 0;
    while(1) {

        if ( idx_test == 0 &&
           ( idx > 0 && idx < list->capacity ) ) break;

        if ( idx >= list->capacity ||
             idx_test >= list->capacity ) return List_Err_t::UNCYCLED_PREV_ERR;

        idx_test = PREV(idx_test);
        idx++;

    }

    return List_Err_t::LST_SUCCSESSFUL;

}

/*=====================================================================================*/

List_Err_t CheckCycle ( List_t* list, int check_t ) {

    size_t idx = 0;
    size_t idx_test = 0;

    while(1) {

        if ( idx_test == 0 &&
           ( idx > 0 && idx < list->capacity ) ) break;

        if ( idx >= list->capacity ||
             idx_test >= list->capacity ) return List_Err_t::UNCYCLED_PREV_ERR;

        idx_test = PREV(idx_test);
        idx++;

    }
}

/*=====================================================================================*/

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
        case List_Err_t::LIST_T_NULL_ERR:
            fprintf ( stderr, "[NULL POINTER AT LIST STRUCTURE]\n" );
            break;
        case List_Err_t::ZERO_CAPACITY_ERR:
            fprintf ( stderr, "[LIST HAS ZERO CAPACITY]\n" );
            break;
        case List_Err_t::INCOR_FREE_P_ERR:
            fprintf ( stderr, "[FREE POINTER HAS POSITION OUT OF LIST]\n" );
            break;
        case List_Err_t::DATA_NULL_ERR:
            fprintf ( stderr, "[DATA ARRAY IS NULL POINTER]\n" );
            break;
        case List_Err_t::NEXT_NULL_ERR:
            fprintf ( stderr, "[NEXT ARRAY IS NULL POINTER]\n" );
            break;
        case List_Err_t::PREV_NULL_ERR:
            fprintf ( stderr, "[PREV ARRAY IS NULL POINTER]\n" );
            break;
        case List_Err_t::SIZE_MORE_CAPACITY_ERR:
            fprintf ( stderr, "[SIZE IS GREATER THAN CAPACITY]\n" );
            break;
        case List_Err_t::UNCYCLED_NEXT_ERR:
            fprintf ( stderr, "[NEXT ARRAY IS NOT CYCLED]\n" );
            break;
        case List_Err_t::UNCYCLED_PREV_ERR:
            fprintf ( stderr, "[PREV ARRAY IS NOT CYCLED]\n" );
            break;


    }

}

/*=====================================================================================*/

void CreateLogDir ( char* dir_name, int call_num ) {
 
    char proj_path[MAX_STR_LEN_] = {0};
    time_t log_time_sec = time(NULL);
    struct tm* log_time = localtime( &log_time_sec );
    
    getcwd(proj_path, sizeof(proj_path));
    sprintf ( proj_path, "%s/logs", proj_path );
    mkdir ( proj_path, FILE_MODE_ ); 

    sprintf ( dir_name,
              "%s/log_%d.%d.%d_%d:%d:%d",
              proj_path,
              log_time->tm_mday,
              log_time->tm_mon,
              1900 + log_time->tm_year,
              log_time->tm_hour,
              log_time->tm_min,
              log_time->tm_sec );
    
    mkdir ( dir_name, FILE_MODE_ );

}

/*=====================================================================================*/

List_Err_t ListDump ( List_t* list ) {

    List_Err_t status = List_Err_t::LST_SUCCSESSFUL;

    static int call_num = 1;
    char filename[MAX_STR_LEN_] = {0};
    char graphname[MAX_STR_LEN_] = {0};
    static char log_dir[MAX_STR_LEN_] = {0};

    if (call_num == 1) CreateLogDir ( log_dir, call_num );

    snprintf ( filename, sizeof(filename), "%s/list_log.htm", log_dir );
    snprintf ( graphname, sizeof(graphname), "graph_%d.svg", call_num );

    call_num++;

    FILE* log_file = fopen ( filename, "a" );
    if ( log_file == nullptr ) return List_Err_t::FILE_OPEN_ERR;

    fprintf ( log_file,
              "\n<div style=\"height:4px;background:#000\"/>\n" 
              "<pre>\n"
              "<body style=\"background-color: white;\">\n" );

    PrintLogHeader ( list, log_file );
    
    fprintf ( log_file, "<h3>[IMG]:</h3>\n" );

    status =  CreateGraphImg ( list, graphname, log_dir );

    fprintf ( log_file, "<img "
                        "src = \"%s\" "
                        "style=\"width: 80vw; height: auto; max-width: 100%%;\" >",
                        graphname );

    fclose ( log_file );

    return List_Err_t::LST_SUCCSESSFUL;

}

/*=====================================================================================*/

void PrintLogHeader ( List_t* list, FILE* log_file ) {

    fprintf ( log_file,
        "<h3>[LIST INFO]:</h3>\n"
        "name: %s\n"
        "location: %s::%d, %s()\n"
        "<h3>[LIST DATA]:</h3>\n"
        "capacity: %lu\n"
        "size: %lu\n",
        list->info.name,
        list->info.file,
        list->info.line,
        list->info.func,
        list->capacity,
        list->size );

    fprintf ( log_file ,"data: " );
    for ( size_t i = 0; i < list->capacity; i++ ) {

        if ( DATA(i) != POISON_VALUE )
            fprintf ( log_file, "%8ld|", DATA(i) );
        else
            fprintf ( log_file, "  POISON|" );
    }
    fprintf ( log_file ,"\n" );


    fprintf ( log_file ,"next: " );
    for ( size_t i = 0; i < list->capacity; i++ ) {

        if ( NEXT(i) != POISON_VALUE )
            fprintf ( log_file, "%8ld|", NEXT(i) );
        else
            fprintf ( log_file, "  POISON|" );

    }
    fprintf ( log_file ,"\n" );


    fprintf ( log_file ,"prev: " );
    for ( size_t i = 0; i < list->capacity; i++ ) {

        if ( PREV(i) != POISON_VALUE )
            fprintf ( log_file, "%8lu|", PREV(i) );
        else
            fprintf ( log_file, "  POISON|" );

    }
    fprintf ( log_file ,"\n" );

}

/*=====================================================================================*/

List_Err_t CreateGraphImg ( List_t* list, const char* graphname, const char* graph_dir ) {

    char graph_txt_path[MAX_STR_LEN_] = {0};
    char graph_svg_path[MAX_STR_LEN_] = {0};
    snprintf ( graph_svg_path, sizeof(graph_svg_path), "%s/%s", graph_dir, graphname );
    snprintf ( graph_txt_path, sizeof(graph_txt_path), "%s/graph.txt", graph_dir );

    FILE* graph_text = fopen ( graph_txt_path, "w" );
    if ( graph_text == nullptr ) return List_Err_t::FILE_OPEN_ERR;

    fprintf ( graph_text, 
              "digraph structs {\n"
              "   rankdir = LR;\n"
              "   bgcolor = \"white\""
              "   overlap = \"scale\";\n"
              /*"   splines = \"ortho\";\n"*/
              "   node [fontname=\"Helvetica-BoldOblique\", fontsize=\"11\","
              " shape=\"box\", style=\"filled,bold\", fillcolor = \"#ffcccc\", color = \"#ffb3b3\"];\n"
              "   edge [color = \"#00000000\"]\n"
              "   node_0 [shape = Mrecord, style = \"filled,bold\", fillcolor = \"#ffcccc\", color = \"#ffb3b3\","
              "label = \" ROOT | idx: 0 |{ head: %lu | tail: %lu }\"]\n",
              list->next[0], list->prev[0] );
    
    PrintGraphNodes ( list, graph_text );

    fprintf ( graph_text,
              "   edge [color = \"#ffe680\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n"
              "   node_0 -> node_%lu;\n"
              "   edge [color = \"#ffb3b3\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n"
              "   node_0 -> node_%lu;\n"
              "   edge [color = \"#b3b3ff\", constraint = \"false\", penwidth = 2.0, arrowsize = 0.5]\n"
              "   free [shape = Mrecord , color = \"blue\", lable = \"free: %lu\","
              " style = \"filled,bold\", fillcolor = \"#ccccff\", color = \"#b3b3ff\"]\n"
              "   free -> node_%lu;\n",
              list->next[0], list->prev[0], list->free, list->free );

    // PrintEdgesForNext ( list, graph_text );
    // PrintEdgesForPrev ( list, graph_text );
    PrintEdges ( list, graph_text );
    // PrintEdgesForFree ( list, graph_text );

    fprintf ( graph_text, "}" );
    fclose ( graph_text );

    char cmd_line[MAX_STR_LEN_] = {0};
    snprintf ( cmd_line, sizeof(cmd_line), "dot -Tsvg %s -o %s", graph_txt_path, graph_svg_path );
    system ( cmd_line );

    return List_Err_t::LST_SUCCSESSFUL;

}

/*=====================================================================================*/

void PrintGraphNodes ( List_t* list, FILE* graph_text ) {

    for ( size_t i = 1; i < list->capacity; i++ ) {

        if ( PREV(i) != POISON_VALUE )
            fprintf ( graph_text, 
                      "   node_%lu [shape = Mrecord , label = \" data: %ld | idx: %lu |{ next: %lu | prev: %lu }\","
                      " style = \"filled,bold\", fillcolor = \"#fff0b3\", color = \"#ffe680\"]\n",
                      i, list->data[i], i, list->next[i], list->prev[i]);

        else if ( NEXT(i) == POISON_VALUE )
            fprintf ( graph_text, 
                      "   node_%lu [shape = Mrecord , label = \" data: POISON | idx: %lu |{ next: POISON | prev: POISON }\","
                      " style = \"filled,bold\", fillcolor = \"#ccccff\", color = \"#b3b3ff\"]\n",
                      i, i, list->next[i]);
        else 
            fprintf ( graph_text, 
                      "   node_%lu [shape = Mrecord , label = \" data: POISON | idx: %lu |{ next: %lu | prev: POISON }\","
                      " style = \"filled,bold\", fillcolor = \"#ccccff\", color = \"#b3b3ff\"]\n",
                      i, i, list->next[i]);

    }

    for ( size_t i = 0; i <list->capacity; i++ ) {

        if ( i == list->capacity - 1 )
            fprintf ( graph_text, " node_%lu;\n", i );
        else
            fprintf ( graph_text, " node_%lu ->", i );

    }

}

/*=====================================================================================*/

void PrintEdges ( List_t* list, FILE* graph_text ) {

    int8_t* bothdir_next = (int8_t*) calloc ( list->capacity, sizeof(int8_t) );
    int8_t* bothdir_prev = (int8_t*) calloc ( list->capacity, sizeof(int8_t) );

    for ( size_t idx = 1; idx < list->capacity; idx++ ) {

        if ( NEXT(idx) == POISON_VALUE ) continue;

        if ( PREV(idx) == POISON_VALUE ) {
            fprintf ( graph_text, 
                      "node_%lu-> node_%lu"
                      "[color = "FREE_EDGE_COL_", "EDGE_STD_SET_" ];\n",
                      idx, NEXT(idx) );
            continue;
        }

        if ( PREV(idx) < list->capacity ) {

            if ( NEXT(PREV(idx)) == idx ) {

                if ( !bothdir_prev[idx] && PREV(idx) != 0 ) {

                    fprintf ( graph_text,
                            "node_%lu-> node_%lu"
                            "[color = "BOTH_EDGE_COL_", dir = both, "EDGE_STD_SET_"];\n",
                            idx, PREV(idx) );
                    bothdir_next[PREV(idx)] = 1;
                }
            }
            else {

                fprintf ( graph_text,
                        "node_%lu-> node_%lu"
                        "[color = "PREV_EDGE_COL_", "EDGE_STD_SET_"];\n",
                        PREV(idx), idx );
            }
        }
        else {

            fprintf ( graph_text,
                      "node_%lu-> node_%lu"
                      "[color = "BAD_EDGE_COL_", "EDGE_STD_SET_"];\n",
                      PREV(idx), idx );
        }

        // if ( PREV(NEXT(idx)) == idx ) {

        //     if ( !bothdir_next[idx] && NEXT(idx) != 0 ) {

        //         fprintf ( graph_text,
        //                   "node_%lu-> node_%lu"
        //                   "[color = "BOTH_EDGE_COL_", dir = both, "EDGE_STD_SET_"];\n",
        //                   idx, NEXT(idx) );
        //         bothdir_prev[NEXT(idx)] = 1;
        //     }
        // }
        // else {

        //     fprintf ( graph_text,
        //               "node_%lu-> node_%lu"
        //               "[color = "NEXT_EDGE_COL_", "EDGE_STD_SET_"];\n",
        //               idx, NEXT(idx) );
        // }
    
    }

    free(bothdir_next);
    free(bothdir_prev);

} 

/*=====================================================================================*/