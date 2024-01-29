//
//  qtl_mem.c
//  qtlcart2
//
//  Created by Christopher Basten on 12/31/23.
//

#include "qtl_mem.h"
char mem_alloc_error_text[128];

void MemAllocError(char *error_text, FILE *outf) {
  fprintf(outf, "MemAllocError run-time error...\n");
  fprintf(outf, "%s\n", error_text);
  fprintf(outf, "...now exiting to system...\n");
  exit(1);
}

// PrintCharVec(thevec, lb, ub, "Title", '\t', "\n", OUTF);
void PrintCharVec(char *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT ) {
    int i;
    if ( head != NULL )
        fprintf(OUT, "%s%c", head, delimiter);
    for ( i=lb ; i< ub; i++ )
        fprintf(OUT, "%c%c", vec[i],delimiter);
    fprintf(OUT, "%c%s", vec[ub], line_end);
    
}
// PrintIntVec(thevec, lb, ub, "Title", '\t', "\n", OUTF);
void PrintIntVec(int *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT ) {
    int i;
    if ( head != NULL )
        fprintf(OUT, "%s%c", head, delimiter);
    for ( i=lb ; i< ub; i++ )
        fprintf(OUT, "%i%c", vec[i],delimiter);
    fprintf(OUT, "%i%s", vec[ub], line_end);
    
}
// PrintUnsignedVec(thevec, lb, ub, "Title", '\t', "\n", OUTF);
void PrintUnsignedVec(unsigned *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT ) {
    int i;
    if ( head != NULL )
        fprintf(OUT, "%s%c", head, delimiter);
    for ( i=lb ; i< ub; i++ )
        fprintf(OUT, "%i%c", vec[i],delimiter);
    fprintf(OUT, "%i%s", vec[ub], line_end);
    
}
// PrintFloatVec(thevec, lb, ub, "Title", '\t', "\n", OUTF);
void PrintFloatVec(float *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT ) {
    int i;
    if ( head != NULL )
        fprintf(OUT, "%s%c", head, delimiter);
    for ( i=lb ; i< ub; i++ )
        fprintf(OUT, "%f%c", vec[i],delimiter);
    fprintf(OUT, "%f%s", vec[ub], line_end);
    
}
// PrintDoubleVec(thevec, lb, ub, "Title", '\t', "\n", OUTF);
void PrintDoubleVec(double *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT ) {
    int i;
    if ( head != NULL )
        fprintf(OUT, "%s%c", head, delimiter);
    for ( i=lb ; i< ub; i++ )
        fprintf(OUT, "%f%c", vec[i],delimiter);
    fprintf(OUT, "%f%s", vec[ub], line_end);
    
}

void free_CharVec( char *vec, int lb, int ub ,   FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tCharVec\t%d\t%p\n",  ub-lb+1,(vec+lb) );
    free( (char *)   (vec+lb)   );
}
char *CharVec( int lb, int ub, FILE *MEMLOG ) {
    int ii;
    char *vec;
    
    vec = ( char * ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( char ) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error CharVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tCharVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );
    
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = '\0';
    return vec - lb;
}
void free_CharPtrVec( char **vec, int lb, int ub  , FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tCharPtrVec\t%d\t%p\n", ub-lb+1,(vec+lb) );
    free( (char *)   (vec+lb)   );
}
char **CharPtrVec( int lb, int ub, FILE *MEMLOG ) {
    int ii;
    char **vec;
    
    vec = ( char ** ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( char *) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error CharPtrVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tCharPtrVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec  );
    
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = NULL;
    return vec - lb;
}
//  Free_CharMatrix( (void **) matrix, lbr, ubr, lbc, ubc, MEMLOG);
void Free_CharMatrix(char **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG) {
    int i;
    for ( i=lbr; i<=ubr; i++ )
        free_CharVec(mat[i], lbc,ubc, MEMLOG);
    free_CharPtrVec( mat , lbr,ubr, MEMLOG);
}
char **CharMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    char **mat;
    mat = CharPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = CharVec(lbc,ubc, MEMLOG);
    return(mat);
}


void free_IntVec( int *vec, int lb, int ub ,   FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tIntVec\t%d\t%p\n",  ub-lb+1,(vec+lb) );
    free( (char *)   (vec+lb)   );
}
int *IntVec( int lb, int ub, FILE *MEMLOG ) {
    int ii;
    int *vec;
    
    vec = ( int * ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( int ) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error IntVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tIntVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = 0;
    return vec - lb;
}
void free_IntPtrVec( int **vec, int lb, int ub ,  FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tIntPtrVec\t%d\t%p\n",  ub-lb+1,vec+lb );
    free( (char *)   (vec+lb)   );
}
int **IntPtrVec( int lb, int ub, FILE *MEMLOG ) {
    int ii;
    int **vec;
    
    vec = ( int ** ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( int *) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error IntPtrVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tIntPtrVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );
    
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = NULL;
    return vec - lb;
}
void Free_IntMatrix(int **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG) {
    int i;
    for ( i=lbr; i<=ubr; i++ )
        free_IntVec(mat[i], lbc,ubc, MEMLOG);
    free_IntPtrVec( mat , lbr,ubr, MEMLOG);
}
int **IntMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    int **mat;
    mat = IntPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = IntVec(lbc,ubc, MEMLOG);
    return(mat);
}

void free_UnsignedVec( unsigned *vec, int lb, int ub ,   FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tUnsignedVec\t%d\t%p\n",  ub-lb+1,(vec+lb) );
    free( (char *)   (vec+lb)   );
}
unsigned *UnsignedVec( int lb, int ub, FILE *MEMLOG ) {
    int ii;
    unsigned *vec;
    
    vec = ( unsigned * ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( unsigned ) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error UnsignedVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tUntsignedVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = 0;
    return vec - lb;
}
void free_UnsignedPtrVec( unsigned **vec, int lb, int ub ,  FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tUnsignedPtrVec\t%d\t%p\n",  ub-lb+1,vec+lb );
    free( (char *)   (vec+lb)   );
}
unsigned **UnsignedPtrVec( int lb, int ub, FILE *MEMLOG ) {
    int ii;
    unsigned **vec;
    
    vec = ( unsigned ** ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( unsigned *) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error uUnsignedPtrVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tUnsignedPtrVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );
    
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = NULL;
    return vec - lb;
}
void Free_UnsignedMatrix(unsigned **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG) {
    int i;
    for ( i=lbr; i<=ubr; i++ )
        free_UnsignedVec(mat[i], lbc,ubc, MEMLOG);
    free_UnsignedPtrVec( mat , lbr,ubr, MEMLOG);
}
unsigned **UnsignedMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    unsigned **mat;
    mat = UnsignedPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = UnsignedVec(lbc,ubc, MEMLOG);
    return(mat);
}

void free_FloatVec( float *vec, int lb, int ub ,   FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tFloatVec\t%d\t%p\n",  ub-lb+1,(vec+lb) );
    free( (char *)   (vec+lb)   );
}
float *FloatVec( int lb, int ub , FILE *MEMLOG) {
    int ii;
    float *vec;
    
    vec = ( float * ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( float ) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error FloatVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tFloatVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );

    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = ( float ) 0.0;
    return vec - lb;
}
void free_FloatPtrVec( float **vec, int lb, int ub ,  FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tFloatPtrVec\t%d\t%p\n",  ub-lb+1,vec+lb );
    free( (char *)   (vec+lb)   );
}
float **FloatPtrVec( int lb, int ub, FILE *MEMLOG ) {
    int ii;
    float **vec;
    
    vec = ( float ** ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( float *) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error FloatPtrVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tfloatPtrVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );
    
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = NULL;
    return vec - lb;
}
void Free_FloatMatrix(float **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG) {
    int i;
    for ( i=lbr; i<=ubr; i++ )
        free_FloatVec(mat[i], lbc,ubc, MEMLOG);
    free_FloatPtrVec( mat , lbr,ubr, MEMLOG);
}
float **FloatMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    float **mat;
    mat = FloatPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = FloatVec(lbc,ubc, MEMLOG);
    return(mat);
}



void free_DoubVec( double *vec, int lb, int ub ,   FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tDoubVec\t%d\t%p\n",  ub-lb+1,(vec+lb) );
    free( (char *)   (vec+lb)   );
}
double *DoubVec( int lb, int ub , FILE *MEMLOG) {
    int ii;
    double *vec;
    
    vec = ( double * ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( double ) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error DoubVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tDoubleVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );

    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = ( double ) 0.0;
    return vec - lb;
}
void free_DoubPtrVec( double **vec, int lb, int ub ,  FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\tDoublePtrVec\t%d\t%p\n",  ub-lb+1,vec+lb );
    free( (char *)   (vec+lb)   );
}
double **DoubPtrVec( int lb, int ub, FILE *MEMLOG ) {
    int ii;
    double **vec;
    
    vec = ( double ** ) malloc( ( size_t ) ( ub -lb + 1 ) * sizeof( double *) );
    if (!vec) {
        sprintf(mem_alloc_error_text, "Allocation error DoublePtrVec(%d,%d)", lb,ub);
        MemAllocError(mem_alloc_error_text,stderr);
    }
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "allocation\tdoublePtrVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );
    
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = NULL;
    return vec - lb;
}
void Free_DoubMatrix(double **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG) {
    int i;
    for ( i=lbr; i<=ubr; i++ )
        free_DoubVec(mat[i], lbc,ubc, MEMLOG);
    free_DoubPtrVec( mat , lbr,ubr, MEMLOG);
}
double **DoubMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    double **mat;
    mat = DoubPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = DoubVec(lbc,ubc, MEMLOG);
    return(mat);
}
