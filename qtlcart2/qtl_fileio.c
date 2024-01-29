//
//  qtl_fileio.c
//  qtlcart2
//
//  Created by Christopher Basten on 1/4/24.
//

#include "qtl_fileio.h"
#define MAXLINE 512

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

struct input_line *ReadParseFile(char *input_file, long *lineno ) {
    int i, is_dos, delimiter, linedelim;
    FILE *INP;
    struct input_line *anchor=NULL, *new_one, *old_one;
    char *expandbuff = NULL;
    size_t lexpandbuff ;
    ssize_t lreadbuff;
    *lineno = 0;
    if ( input_file == NULL   )
        return(NULL);
    if ( (i=isfile(input_file)) == 0 )
        return(NULL) ;
    is_dos = GuessInputType(input_file, &delimiter, &linedelim);
    INP=fileopen(input_file,"r");
    lexpandbuff = (size_t) MAXLINE;
    expandbuff = calloc(lexpandbuff, sizeof(char));
    
    lreadbuff = getdelim(&expandbuff, &lexpandbuff,linedelim, INP) ;
    anchor = ParseALine2(NULL,0, expandbuff, *lineno, delimiter );
    new_one = anchor;
    while (!feof(INP)) {
        lexpandbuff = (size_t) MAXLINE;
        expandbuff = calloc(lexpandbuff, sizeof(char));
        lreadbuff = getdelim(&expandbuff, &lexpandbuff,linedelim, INP) ;
        // empty line is end of file
        if(strlen(expandbuff)==0) {
            free( expandbuff);
            fileclose(input_file, INP);
            return(anchor);
        }
        ++(*lineno);
        old_one = new_one;
        new_one = ParseALine2(old_one, 0, expandbuff, *lineno, delimiter);
        if ( new_one != NULL)
            old_one = new_one; 
    }
    fileclose(input_file, INP);
    free( expandbuff);
    return(anchor);
}

struct input_line *ReadByLine(char *input_file ) {
    int i, is_dos, delimiter, linedelim, ntoks;
    long lineno=0;
    FILE *INP;
    struct input_line   *new_one, *header_line;
    char *expandbuff = NULL;
    size_t lexpandbuff ;
    ssize_t lreadbuff;
    lineno = 0;
    if ( input_file == NULL   )
        return(NULL);
    if ( (i=isfile(input_file)) == 0 )
        return(NULL) ;
    header_line = ReadHeader(input_file);
    is_dos = GuessInputType(input_file, &delimiter, &linedelim);
    INP=fileopen(input_file,"r");
    lexpandbuff = (size_t) MAXLINE;
    expandbuff = calloc(lexpandbuff, sizeof(char));
    lreadbuff = getdelim(&expandbuff, &lexpandbuff,linedelim, INP) ;
    new_one = AllocInputLine(expandbuff,header_line->columns); // allocate space for header

    while (!feof(INP)) {
        lreadbuff = getdelim(&new_one->input, &lexpandbuff,linedelim, INP) ;
        // jump over empty lines
        if(strlen(new_one->input)==0) {
            DeAllocInputLine(new_one);
            fileclose(input_file, INP);
            return(header_line);
        }
        ++(lineno);
        
        i = ParseThisLine(new_one,   lineno, delimiter, &ntoks);
        if ( i==1   ){
        // ntoks < 0 means new_one or new_one->fields is null
        // ntoks < new_one->columns  too few
        // ntoks > new_one->columns  too many
            break; 
        }
        if ( lineno<10)
            PrintInputLine(new_one, new_one->columns, '\t', stdout);
    }
    fileclose(input_file, INP);
    DeAllocInputLine(new_one);

    return(header_line);
}

// Given an input file name, return the header parsed by the delimiter.
//
struct input_line *ReadHeader(char *input_file  ) {
    int i, is_dos, delimiter, linedelim, ncols = -1;
    FILE *INP;
    struct input_line *anchor=NULL ;
    char *expandbuff = NULL;
    size_t lexpandbuff ;
    ssize_t lreadbuff;
    if ( input_file == NULL   )
        return(NULL); // no file name, return NULL
    if ( (i=isfile(input_file)) == 0 )
        return(NULL) ; // Can't open, return NULL
    is_dos = GuessInputType(input_file, &delimiter, &linedelim);  // Get the delimiter
    INP=fileopen(input_file,"r");  // open file, get first line
    lexpandbuff = (size_t) MAXLINE;
    expandbuff = calloc(lexpandbuff, sizeof(char));
    lreadbuff = getdelim(&expandbuff, &lexpandbuff,linedelim, INP) ;
    fileclose(input_file, INP);
    ncols = CountFields(expandbuff, delimiter); // count fields
    
    anchor = AllocInputLine(expandbuff,ncols); // allocate space for header
    i = ParseThisLine(anchor,0,delimiter,&ncols); // parse it

    free( expandbuff); // free the mem
    return(anchor); // return the parsed header. 
}
