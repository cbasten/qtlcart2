//
//  main.c
//  qtlcart2
//
//  Created by Christopher Basten on 12/31/23.
//

#include <math.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include "qtl_mem.h"
#include "qtl_fileio.h"
#include "qtl_ran.h"
#include "qtl_string.h"

#define MAXNAME 511
#define N_INTS 16777216
int main(int argc, const char * argv[]) {
    FILE *MEMLOG, *OUT;
    int i;
    char *cstring=NULL, **cmatrix=NULL, *input_file=NULL, *output_file=NULL;
    int *ivector=NULL, *countvec=NULL;
    long lineno=0;
    unsigned *uvector=NULL;
    float *fvector=NULL;
    double *dvector=NULL, diff;
    struct input_line *anchor=NULL, *new_one, *header_line;

    
    MEMLOG = fileopen("memory_log.txt","w");
    fprintf(MEMLOG, "Function\tType\tLower\tUpper\tNumber\tAddress\n");
    cmatrix = CharMatrix(1,10,0,MAXNAME,MEMLOG);
    for ( i=1; i<=10; i++ )
        sprintf(cmatrix[i], "Row %d",i);
    for (i=1; i<=10; i++)
        printf("%s\n",cmatrix[i]);
//    Free_CharMatrix( (void**) cmatrix, 1, 10, 0, MAXNAME, MEMLOG);
    Free_Matrix( (void**) cmatrix, 1, 10, 0, MAXNAME, "char",MEMLOG);
    input_file = CharVec(0,MAXNAME,MEMLOG);
    strcpy(input_file, "g2f_2022_phenotypic_raw_data.csv");
    anchor = ReadParseFile(input_file, &lineno );

    
    if ( anchor !=NULL ) {
        output_file = CharVec(0,MAXNAME,MEMLOG);
        strcpy(output_file, "g2f_2022_phenotypic_raw_data.tsv");
        OUT=fileopen(output_file,"w");
        for ( new_one = anchor; new_one !=NULL ; new_one=new_one->next)
            PrintInputLine( new_one, new_one->columns, '\t', OUT );
        fileclose(output_file, OUT);
    }
    header_line = ReadByLine( input_file ) ;
    cstring = CharVec(0,MAXNAME,MEMLOG);
    strcpy(cstring, SEED_FILE);
    set_ranseed(0, cstring);
    countvec = IntVec(0,7,MEMLOG);

    ivector = IntVec(1,N_INTS,MEMLOG);
    for ( i=1; i<=N_INTS; i++ ){
        ivector[i] =  (int) iran( 1L , 6L ) ;
        if ( ivector[i]>=0 && ivector[i]<=7 )
            countvec[ivector[i]]++;
    }
    PrintIntVec(ivector, 1, 64, "IntVector", '\t', "\n" , stdout );

    uvector = UnsignedVec(1,N_INTS,MEMLOG);
    for ( i=1; i<=N_INTS; i++ ) {
        uvector[i] =  (unsigned) iran( 1L , 6L ) ;
        if ( uvector[i]>=0 && uvector[i]<=7 )
            countvec[(int)uvector[i]]++;
    }
    PrintUnsignedVec(uvector, 1, 64, "UnsignedVector", '\t', "\n" , stdout );

    PrintIntVec(countvec, 0, 7, "Counts", '\t', "\n" , stdout );


    dvector = DoubVec(1,1024,MEMLOG);
    ran_arry(1, 1024,  dvector);
    PrintDoubleVec(dvector, 1, 64, "DoubleVector", '\t', "\n" , stdout );

    fvector = FloatVec(1,64,MEMLOG);
    for ( i=1; i<=64; i++ )
        fvector[i] = (float) gennor(150.0,14.0);
    PrintFloatVec(fvector, 1, 64, "FloatVector", '\t', "\n" , stdout );
    
    diff = 0.0;
    for ( i=1; i<=64; i++ )
        diff = fabs( dvector[i]-fvector[i]) ;
    printf( "Float and Double diff %f\n", diff);
    
    new_one=anchor;
    while ( new_one != NULL)
        new_one = DeAllocInputLine(new_one);
    anchor = NULL;
    
    if ( ivector !=NULL )
        free_Vec(  ivector+1 , 1, 1024, "int", MEMLOG);
    if ( uvector !=NULL )
        free_Vec(  uvector+1 , 1, 1024, "unsigned", MEMLOG);
    if ( fvector !=NULL )
        free_Vec(  fvector+1 , 1, 64, "float", MEMLOG);
    if ( dvector !=NULL )
        free_Vec(  dvector+1 , 1, 1024, "int", MEMLOG);
    if ( cstring !=NULL )
        free_Vec(  cstring+0 ,  0, MAXNAME, "char", MEMLOG);
    if ( input_file !=NULL )
        free_Vec(  input_file+0 ,  0, MAXNAME, "char", MEMLOG);
    if ( output_file != NULL )
        free_Vec(  output_file+0 ,  0, MAXNAME, "char", MEMLOG);
    fileclose("memory_log.txt", MEMLOG);
    
    
    
    return 0;
}
