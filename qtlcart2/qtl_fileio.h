//
//  qtl_fileio.h
//  qtlcart2
//
//  Created by Christopher Basten on 1/4/24.
//

#ifndef qtl_fileio_h
#define qtl_fileio_h
#include "qtl_string.h"
#include <stdio.h>
void fileclose(char *name, FILE *fp);
FILE *fileopen(char *name, char *mode);
int isfile( char *filename );
int GuessInputType(char *infile,  int *delimiter, int *linedelim) ;
struct input_line *ReadParseFile(char *input_file, long *lineno );
struct input_line *ReadByLine(char *input_file );
struct input_line *ReadHeader(char *input_file  );
#endif /* qtl_fileio_h */
