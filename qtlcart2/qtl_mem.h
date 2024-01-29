//
//  qtl_mem.h
//  qtlcart2
//
//  Created by Christopher Basten on 12/31/23.
//

#ifndef qtl_mem_h
#define qtl_mem_h

#include <stdio.h>
#include <stdlib.h>

#define MEM_ACCT_FILE "memory_accounting.txt"
void free_Vec( void *vec, int lb, int ub , char *type, FILE *MEMLOG);
void free_PtrVec( void **vec, int lb, int ub , char *type, FILE *MEMLOG);
void Free_Matrix(void **mat, int lbr, int ubr, int lbc, int ubc, char *type,  FILE *MEMLOG);


char     *CharVec(    int lb, int ub , FILE *MEMLOG ) ;
void free_CharVec( char *vec, int lb, int ub ,   FILE *MEMLOG);
void PrintCharVec(char *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
char **CharPtrVec(int lb, int ub, FILE *MEMLOG ) ;
void free_CharPtrVec( char **vec, int lb, int ub  , FILE *MEMLOG);
char **CharMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) ;
void Free_CharMatrix(char **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG);


int      *IntVec(     int lb, int ub,  FILE *MEMLOG ) ;
void free_IntVec( int *vec, int lb, int ub ,   FILE *MEMLOG);
void PrintIntVec(int *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
int **IntPtrVec( int lb, int ub, FILE *MEMLOG );
void free_IntPtrVec( int **vec, int lb, int ub  , FILE *MEMLOG);
int **IntMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG);
void Free_IntMatrix(int **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG);

unsigned *UnsignedVec(int lb, int ub,  FILE *MEMLOG ) ;
void free_UnsignedVec( unsigned *vec, int lb, int ub ,   FILE *MEMLOG);
void PrintUnsignedVec(unsigned *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
unsigned **UnsignedPtrVec( int lb, int ub, FILE *MEMLOG );
void free_UnsignedPtrVec( unsigned **vec, int lb, int ub  , FILE *MEMLOG);
unsigned **UnsignedMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG);
void Free_UnsignedMatrix(unsigned **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG);

float    *FloatVec(   int lb, int ub,  FILE *MEMLOG ) ;
void free_FloatVec( float *vec, int lb, int ub ,   FILE *MEMLOG);
void PrintFloatVec(float *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
float **FloatPtrVec( int lb, int ub, FILE *MEMLOG );
void free_FloatPtrVec( float **vec, int lb, int ub  , FILE *MEMLOG);
float **FloatMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG);
void Free_FloatMatrix(float **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG);

double   *DoubVec(    int lb, int ub , FILE *MEMLOG ) ;
void free_DoubVec( double *vec, int lb, int ub ,   FILE *MEMLOG);
void PrintDoubleVec(double *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
double **DoubPtrVec( int lb, int ub, FILE *MEMLOG );
void free_DoubPtrVec( double **vec, int lb, int ub  , FILE *MEMLOG);
double **DoubMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG);
void Free_DoubMatrix(double **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG);






#endif /* qtl_mem_h */
