//
//  qtl_fileio.h
//  qtlcart2
//
//  Created by Christopher Basten on 1/4/24.
//

#ifndef qtl_fileio_h
#define qtl_fileio_h
#include <stdio.h>
void fileclose(char *name, FILE *fp);
FILE *fileopen(char *name, char *mode);
int isfile( char *filename );
int GuessInputType(char *infile,  int *delimiter, int *linedelim) ;
#endif /* qtl_fileio_h */
