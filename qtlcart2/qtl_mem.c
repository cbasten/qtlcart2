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

// need to use as free_Vec( thevec+lb , lb, ub, "thetype", OUTPTR);
void free_Vec( void *vec, int lb, int ub , char *type, FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\t%s\t%d\t%p\n",type, ub-lb+1,vec );
    free( (char *)   vec   );
}
void free_PtrVec( void **vec, int lb, int ub , char *type, FILE *MEMLOG) {
    if ( !(MEMLOG==NULL))
        fprintf(MEMLOG, "deallocation\t%s *\t%d\t%p\n",type, ub-lb+1,vec );
    free( (char *)   vec   );
}
//  Free_CharMatrix( (void **) matrix, lbr, ubr, lbc, ubc, MEMLOG);
void Free_Matrix(void **mat, int lbr, int ubr, int lbc, int ubc, char *type,  FILE *MEMLOG) {
    int i;
    for ( i=lbr; i<=ubr; i++ )
        free_Vec((mat[i]+lbc), lbc,ubc,type, MEMLOG);
    free_PtrVec( mat+lbr , lbr,ubr,type, MEMLOG);
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

//  Free_CharMatrix( (void **) matrix, lbr, ubr, lbc, ubc, MEMLOG);
void Free_CharMatrix(void **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG) {
    int i;
    for ( i=lbr; i<=ubr; i++ )
        free_Vec((mat[i]+lbc), lbc,ubc,"char", MEMLOG);
    free_PtrVec( mat+lbr , lbr,ubr,"char*", MEMLOG);
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
        fprintf(MEMLOG, "allocation\tCharPtrVec\t%d\t%d\t%d\t%p\n",lb,ub,ub-lb+1,vec );
    
    for ( ii = 0; ii <= ub-lb; ii++ )
        vec[ii] = NULL;
    return vec - lb;
}

char **CharMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    char **mat;
    mat = CharPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = CharVec(lbc,ubc, MEMLOG);
    return(mat);
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

int **IntMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    int **mat;
    mat = IntPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = IntVec(lbc,ubc, MEMLOG);
    return(mat);
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

unsigned **UnsignedMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    unsigned **mat;
    mat = UnsignedPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = UnsignedVec(lbc,ubc, MEMLOG);
    return(mat);
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

float **FloatMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    float **mat;
    mat = FloatPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = FloatVec(lbc,ubc, MEMLOG);
    return(mat);
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

double **DoubMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) {
    int i;
    double **mat;
    mat = DoubPtrVec(lbr,ubr, MEMLOG);
    for (i = lbr; i <= ubr; i++)
      mat[i] = DoubVec(lbc,ubc, MEMLOG);
    return(mat);
}
