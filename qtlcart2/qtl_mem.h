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
int      *IntVec(     int lb, int ub,  FILE *MEMLOG ) ;
unsigned *UnsignedVec(int lb, int ub,  FILE *MEMLOG ) ;
float    *FloatVec(   int lb, int ub,  FILE *MEMLOG ) ;
double   *DoubVec(    int lb, int ub , FILE *MEMLOG ) ;

void PrintCharVec(char *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
void PrintIntVec(int *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
void PrintUnsignedVec(unsigned *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
void PrintFloatVec(float *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );
void PrintDoubleVec(double *vec, int lb, int ub, char *head, char delimiter, char *line_end , FILE *OUT );

void Free_CharMatrix(void **mat, int lbr, int ubr, int lbc, int ubc,   FILE *MEMLOG);
char **CharPtrVec(int lb, int ub, FILE *MEMLOG ) ;
char **CharMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG) ;

double **DoubMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG);
double **DoubPtrVec( int lb, int ub, FILE *MEMLOG );
float **FloatMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG);
float **FloatPtrVec( int lb, int ub, FILE *MEMLOG );
int **IntMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG);
int **IntPtrVec( int lb, int ub, FILE *MEMLOG );
unsigned **UnsignedMatrix(int lbr, int ubr, int lbc, int ubc, FILE *MEMLOG);
unsigned **UnsignedPtrVec( int lb, int ub, FILE *MEMLOG );

#endif /* qtl_mem_h */
