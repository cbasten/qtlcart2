//
//  qtl_fileio.c
//  qtlcart2
//
//  Created by Christopher Basten on 1/4/24.
//

#include "qtl_fileio.h"

FILE *fileopen(char *name, char *mode) {
 /* Open files, writing an error message if it fails. */
  FILE *fp = fopen(name, mode);
  if (fp == NULL) {
    printf("Can't open %s for ", name);
    switch (mode[0]) {
     case 'r':
      printf("reading\n");
      break;
     case 'w':
      printf("writing\n");
      break;
     case 'a':
      printf("appending\n");
      break;
     default:
      printf("some strange mode\n");
      break;
    }
  }
  return fp;
}

void fileclose(char *name, FILE *fp) {
 /* Close files, writing an error message if it fails. */
  if (fp != NULL && fclose(fp) == EOF)
    printf("Error closing %s\n", name);
}

/* return 0 if no file 1 if file exists */
int isfile( char *filename ) {
    int isf;
    FILE *fptr;
    
    isf = 0;
    if ( *( filename ) != '\0' ) {
        fptr = fopen( filename, "r" );
        if ( fptr != NULL ) {
            isf = 1;
            fclose( fptr );
        }
    }
    return ( isf );
}


// Guess line endings (Mac, UNIX, DOS) and the delimiter
// For DOS input, will need to trim all \r's after reading
int GuessInputType(char *infile,  int *delimiter, int *linedelim) {
    FILE *DATA;
    char ch, c ;
    int is_dos = 0;
    int commas=0, tabs=0, semicols=0;
    *linedelim='\n';// Assume Unix
    DATA=fileopen( infile, "r");
    while ( (ch=fgetc(DATA)) != EOF ) {
        if ( ch==',')
            commas++;
        if (ch =='\t')
            tabs++;
        if (ch ==';')
            semicols++;
        if ( ch == '\n'  ) {  /* Get \n first, it is UNIX*/
            *linedelim='\n';
            break;
        } else if (ch == '\r' ) {  /* Get \r first, it is DOS or MAC */
            c=fgetc(DATA);
            if ( c=='\n')  {/* followed by \n, it is DOS. We will trim lines if DOS*/
                *linedelim='\n';
                is_dos=1;
            }
            break;
        }
    }
    fileclose(infile,DATA);
    
    if ( tabs>0 ) //if you have tabs, then tabs are the delimiter
        *delimiter = '\t';
    else if ( commas>0 )  //  you don't have tabs, thus if commas are there, assume they are the delimiter
        *delimiter = ',';
    else if ( semicols>0)  //  no tabs or commas, but there are semicolons, thus ; are the delimiter
        *delimiter=';';
    else
        *delimiter = '?';  // should we make it a space?
    //there should be an else here that indicates we can't parse the file.
    return(is_dos);
}
