//
//  qtl_string.h
//  qtlcart2
//
//  Created by Christopher Basten on 1/8/24.
//

#ifndef qtl_string_h
#define qtl_string_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qtl_mem.h"

typedef struct input_line {   /* structure to hold a line of input */
    char *input;              /* full input line, fields will point to places in this string  */
    char **fields;            /* pointers to the fields */
    int columns;              /* number of columns   */
    long row;                  /*  which row of the  file */
    struct input_line *prev;
    struct input_line *next;
}  the_line;
void PrintInputLine( struct input_line *thisline, int ntoks, char thedelim, FILE *OUT );
struct input_line *AllocInputLine(char *line, int num_fields) ;
struct input_line *DeAllocInputLine(struct input_line *inp ) ;
int CountFields(char *line, char the_delim);
int ParseThisLine(  struct input_line *line_store, long int lineno, char the_delim, int *ntoks);
struct input_line  *ParseALine2(struct input_line *prev_line, int before, char *line, long lineno, char the_delim );

#endif /* qtl_string_h */
