//
//  qtl_string.c
//  qtlcart2
//
//  Created by Christopher Basten on 1/8/24.
//

#include "qtl_string.h"

#define RET_OK 0
#define RET_FAIL 1
#define FALSE 0
#define TRUE 1

char *thistime( char *time_buffer, size_t tlen) {
 
    time_t tptr;
    struct tm *tms;
    size_t len;
    
    time( &tptr );
    tms = localtime( &tptr );
    len = strftime( time_buffer, tlen, "%H:%M:%S on %A, %d %B %Y", tms );

    if ( len == 0 )
        return NULL;
    else
        return time_buffer;

    
}
/*
 Trim whitespace off the front and end of a string.
 */
char *trim(char *str) {
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;
    
    if( str == NULL )
        return NULL;
    len = strlen(str);
    if( len==0 )
        return str;
    
    endp = str + len;
    
    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) {
        ++frontp;
    }
    if( endp != frontp ) {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }
    
    if ( str + len - 1 != endp )
        *(endp + 1) = '\0';
    else if ( frontp != str &&  endp == frontp )
        *str = '\0';
    
    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str ) {
        while( *frontp ) { *endp++ = *frontp++; }
        *endp = '\0';
    }
    return str;
}


char *strupr(char *s) {
  int len, ii;
  if ( s == NULL )
    return(s);
  len = (int) strlen(s);
  for (ii = 0; ii < len; ii++)
    if ( islower(s[ii]) )
      s[ii] = toupper(s[ii]);
  return s;
}

char *strlwr(char *s) {
  int len, ii;
  if ( s == NULL )
    return(s);
  len = (int) strlen(s);
  for (ii = 0; ii < len; ii++)
    if ( isupper(s[ii]) )
      s[ii] = tolower(s[ii]);
  return(s);
}
void PrintInputLine( struct input_line *thisline, int ntoks, char thedelim, FILE *OUT ) {

    int i;
    for (i=1; i<thisline->columns; i++ )
        fprintf(OUT, "%s%c", thisline->fields[i], thedelim);
    fprintf(OUT, "%s\n", thisline->fields[thisline->columns]);
 }

void ListFields( struct input_line *thisline, int ntoks,   FILE *OUT ) {
    
    int i;
    for (i=1; i<=ntoks; i++ )
        fprintf(OUT, "%d\t%s\n",i, thisline->fields[i] );
     
 }
int PrintAllInputLines( struct input_line *anchor, int ntoks, char thedelim, FILE *OUT ) {
    struct input_line *thisline;
    int counter=0;
    for ( thisline = anchor; thisline!=NULL ; thisline = thisline->next) {
        PrintInputLine( thisline, ntoks, thedelim, OUT);
        counter++;
    }
    return(counter);

}

int FindColumnHead( struct input_line *anchor, char *key, char *buffer) {
    int column = -1;
    int i, is_same;
    if (anchor==NULL )
        return(column);
    column--;
    if ( anchor->fields==NULL)
        return(column);
    column--;
    if ( anchor->columns==0 )
        return(column);
    column--;
    for ( i=1; i<=anchor->columns; i++ ) {
        strcpy( buffer, anchor->fields[i]);
        strlwr(buffer);
        if ( (is_same=strcmp( buffer, key) ) == 0 )
            return( i);
    }
    return(column);
}

char *FindKeyValue( struct input_line *anchor, int key_col, int val_col, char *key ) {
    char *return_value = NULL;
    struct input_line *thisline;
    int is_same;
    
    for ( thisline=anchor; thisline != NULL; thisline = thisline->next)
        if ( (is_same=strcmp( thisline->fields[key_col], key) ) == 0 )
            return( thisline->fields[val_col]);
    
    
    return( return_value );
    
    
    
}

/*
 
 
 */
struct input_line *AllocInputLine(char *line, int num_fields) {
    struct input_line *new_input_line;
    new_input_line = (struct input_line *) malloc( (unsigned) sizeof(struct input_line));
    new_input_line->input = line;
    new_input_line->fields= CharPtrVec(1, num_fields, NULL ) ;
    new_input_line->columns = num_fields;
    new_input_line->next=NULL;
    new_input_line->prev=NULL;
    return(new_input_line);
}

struct input_line *DeAllocInputLine(struct input_line *inp ) {
    struct input_line *ret_line=NULL;
    if ( inp != NULL ){
        if ( inp->prev !=NULL ) {
            ret_line = inp->prev;
            ret_line->next = inp->next;
            if ( inp->next !=NULL)
                inp->next->prev = ret_line;
        } else if ( inp->next !=NULL) {
            ret_line = inp->next;
            ret_line->prev = inp->prev;
            if ( inp->prev != NULL )
                inp->prev->next = ret_line;
        }   else
            ret_line = NULL;
    }
    if ( inp->input != NULL)// used calloc to create this, not my subroutine
        free( inp->input );
//        free_Vec( inp->input, 0, (int) strlen(inp->input) , "char", NULL);
    if ( inp->fields!=NULL )
        free_CharPtrVec( inp->fields, 1, inp->columns , NULL);
    free( (char*) inp );
    return(ret_line);
}

int CountFields(char *line, char the_delim){
    int num_fields=1;
    if (line == NULL)
        return(0);
    char *cptr = line;
    char ch;
    int inquote = FALSE;
    while((ch=*cptr) != '\0'  ){//&& fld < upper_bound_fields
        if(ch == '"') {
            inquote = ! inquote;
        } else if (ch == the_delim && ! inquote){
            num_fields++;
        }
        cptr++;
    }
    return(num_fields);
}

/*
 Given a structure to hold the parsed line, populate it.  This will have an expectation of the number of  columns
 
 ParseALine
 */
int ParseThisLine(  struct input_line *line_store, long int lineno, char the_delim, int *ntoks) {
    char *line ;
    *ntoks = -1; 
    if ( line_store==NULL)
        return RET_FAIL;
    line = line_store->input;
    if(line == NULL)
        return RET_FAIL;
    if ( line_store->columns <1 || line_store->fields == NULL)
        return RET_FAIL;
    // chop of last char of input if it is a CR or LF (e.g.Windows file loading in Unix env.)
    // can be removed if sure fgets has removed both CR and LF from end of line
    if(*(line + strlen(line)-1) == '\r' || *(line + strlen(line)-1) == '\n')
        *(line + strlen(line)-1) = '\0';
    if(*(line + strlen(line)-1) == '\r' || *(line + strlen(line)-1 )== '\n')
        *(line + strlen(line)-1) = '\0';

    char *cptr = line;
    int fld = 1;
    int inquote = FALSE;
    char ch;
    for ( fld=1; fld<=line_store->columns; fld++)
        line_store->fields[fld]=NULL;
    fld=1;
    line_store->fields[fld]=cptr;

    while((ch=*cptr) != '\0'  ){//&& fld < upper_bound_fields
        if(ch == '"') {
            if(! inquote) {
                if (   fld<=line_store->columns)
                    line_store->fields[fld]=cptr+1;
                
            } else {
                *cptr = '\0';               // zero out " and jump over it
            }
            inquote = ! inquote;
        } else if(ch == the_delim && ! inquote){
            *cptr = '\0';                   // end of field, null terminate it
            if (   fld< line_store->columns)
                line_store->fields[++fld]=cptr+1;
            else
                ++fld;
        }
        cptr++;
    }
    *ntoks = fld;
    if(fld > line_store->columns ){
        fprintf(stderr, "Expected field count (%d) exceeded on line %ld  : ntoks = %d\n", line_store->columns,  lineno,fld);
        return RET_FAIL;
    } else if (fld < line_store->columns){
        fprintf(stderr, "Expected field count (%d) not reached on line %ld : ntoks =  %d\n", line_store->columns, lineno, fld);
        return RET_FAIL;
    }
    return RET_OK;
}



/*
 A complete example which leaves the fields as NULL-terminated strings in the original input buffer and provides access to them via an array of char pointers. The CSV processor has been confirmed to work with fields enclosed in "double quotes", ignoring any delimiter chars within them.
 https://stackoverflow.com/users/4528615/gus-gator
 
 Gus Gator supplied code.
 
 
 How does this add anything to the already accepted answer? – Matt Taylor Feb 4 '15 at 12:47
 2
 The accepted answer will treat this 4-element CSV as 6 elements: QA-Q000630115728222,QA-A0926511569122067,"In 1687 John Phillips, Miltons nephew, produced a Don Quixote made English.",2017-03-07T00:00:00.000Z Gus Gator's example will treat it as the proper 4 elements. – Adam Howell Dec 14 '18 at 18:31
 
 */
struct input_line  *ParseALine2(struct input_line *prev_line,int before, char *line, long lineno, char the_delim ){
    int num_fields;
    struct input_line *new_line;
    if(line == NULL)
        return NULL;

    // chop of last char of input if it is a CR or LF (e.g.Windows file loading in Unix env.)
    // can be removed if sure fgets has removed both CR and LF from end of line
    if(*(line + strlen(line)-1) == '\r' || *(line + strlen(line)-1) == '\n')
        *(line + strlen(line)-1) = '\0';
    if(*(line + strlen(line)-1) == '\r' || *(line + strlen(line)-1 )== '\n')
        *(line + strlen(line)-1) = '\0';

    num_fields = CountFields(line, the_delim);
    if (num_fields==0)
        return( NULL );
    new_line = AllocInputLine(line, num_fields);
    new_line->row = lineno ;
    char *cptr = line;
    int fld=1;
    int inquote = FALSE;
    char ch;


    new_line->fields[fld]=cptr;

    while((ch=*cptr) != '\0'  ){//&& fld < upper_bound_fields
        if(ch == '"') {
            if(! inquote)
                new_line->fields[fld]=cptr+1;
            else
                *cptr = '\0';               // zero out " and jump over it
            inquote = ! inquote;
        } else if(ch == the_delim && ! inquote){
            *cptr = '\0';                   // end of field, null terminate it
            new_line->fields[++fld]=cptr+1;
        }
        cptr++;
    }
    if ( before==0 ){
        new_line->prev = prev_line;
        if ( prev_line!=NULL) {
            new_line->next = prev_line->next;
            if ( prev_line->next !=NULL)
                new_line->next->prev = new_line;
            prev_line->next = new_line;
        }
    } else {
        new_line->next = prev_line;
        if ( prev_line!=NULL) {
            new_line->prev = prev_line->prev;
            if ( prev_line->prev!=NULL)
                new_line->prev->next  = new_line;
            prev_line->prev = new_line;
        }
    }
    return new_line ;
}

#undef RET_OK
#undef RET_FAIL
#undef FALSE
#undef TRUE 
