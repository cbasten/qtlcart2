//
//  qtl_sim.c
//  qtlcart2
//
//  Created by Christopher Basten on 1/20/24.
//

#include "qtl_sim.h"
char alleles[5] = {'N','A','C','G','T'};
//double gennor(double av,double sd)

void random_alleles(char *a1, char *a2, double *ref_freq, double *var_freq, double alpha, double beta) {
    long ia1, ia2;
    double freq;
    freq = genbet(alpha,beta);
    if ( freq>0.5) {
        *ref_freq = freq;
        *var_freq = 1.0-freq;
    } else {
        *var_freq = freq;
        *ref_freq = 1.0-freq;
    }
    ia1 = iran(1122L , 4 );
    *a1 = alleles[(int)ia1];
    do {
        ia2 = iran(1122L, 4);
    } while (ia2==ia1);
    *a2 = alleles[(int)ia2] ;
}
void swap(int* xp, int* yp)
{
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
}

// Function to perform Selection Sort
void selectionSort(int arr[], int n)
{
  int i, j, min_idx;

  // One by one move boundary of
  // unsorted subarray
  for (i = 0; i < n - 1; i++) {
      // Find the minimum element in
      // unsorted array
      min_idx = i;
      for (j = i + 1; j < n; j++)
          if (arr[j] < arr[min_idx])
              min_idx = j;

      // Swap the found minimum element
      // with the first element
      swap(&arr[min_idx], &arr[i]);
  }
}


int *sim_phys_chrom( double mean_len_bp, double sd_len_bp, double mean_snps, double sd_snps,  FILE *MEM_LOG ) {
    int *bp_positions,i , n_snps, this_position;
    double length, f_pos;
    bp_positions = NULL;
    if ( mean_len_bp < mean_snps )
        return(bp_positions);
    if ( sd_len_bp <= 0.0 || sd_snps <= 0.0  )
        return(bp_positions);
    if ( sd_snps == 0.0 )
        n_snps = (int) mean_snps ;
    else
        n_snps = (int) gennor( mean_snps,  sd_snps );
    bp_positions = IntVec(0,n_snps, MEM_LOG);
    length = gennor( mean_len_bp,  sd_len_bp );
    for (i=0; i< n_snps; i++ ) {
        f_pos =  ranf(i);
        this_position=(int) (length*f_pos);
        bp_positions[i] = this_position  ;
    }
    
    selectionSort(bp_positions, n_snps);
    for ( i= n_snps; i>0; i--)
        bp_positions[i] = bp_positions[i-1];
    bp_positions[0] = n_snps ;
    
    return(bp_positions);
}

//  phys_genome = IntMatrix( 1, nchrom, 0, phys_genome[c][0] )
int **sim_phys_genome( double nchrom,  double mean_len_bp, double sd_len_bp, double mean_snps, double sd_snps,   FILE *MEM_LOG  ) {
    int **phys_genome=NULL;
    int i;
    if ( nchrom < 1 )
        return(phys_genome);
    phys_genome = IntPtrVec( 1, nchrom,  MEM_LOG ) ;
    for ( i=1; i<=nchrom; i++ )
        phys_genome[i] = sim_phys_chrom(  mean_len_bp,  sd_len_bp, mean_snps,  sd_snps,  MEM_LOG);
    
    return(phys_genome);
}

double **recomb_matrix( int **phys_genome, int nchrom, FILE *MEM_LOG , FILE *LOG) {
    int i,j;
    double **recomb_matrix=NULL, scale,scale2, centromere;
    int lb_cent, ub_cent ;
    scale = 1000000.0; // 1,000,000 bp is 1 cM
    scale2 = 10.0*scale; //  near centromere, 10 fold lowere recom rate

    if ( phys_genome==NULL )
        return(recomb_matrix);
    recomb_matrix = DoubPtrVec( 1, nchrom, MEM_LOG );
    for ( i=1; i<=nchrom; i++ ) {
        centromere = ranf(i) * phys_genome[i][phys_genome[i][0]] ;
        lb_cent = (int) ( centromere - 0.1* phys_genome[i][phys_genome[i][0]] );
        ub_cent = (int) ( centromere + 0.1* phys_genome[i][phys_genome[i][0]] );
        recomb_matrix[i] = DoubVec( 1, phys_genome[i][0] ,  MEM_LOG);
        recomb_matrix[i][1] = 0.0; // (double) phys_genome[i][1] / scale ;
        fprintf(LOG, "Chrom %d , Length %d, Centromere %f, lower %d upper %d\n", i, phys_genome[i][phys_genome[i][0]], centromere, lb_cent, ub_cent);
        for ( j=2; j<= phys_genome[i][0]; j++ ) {
            if ( phys_genome[i][j] > lb_cent && phys_genome[i][j] < ub_cent)
                recomb_matrix[i][j] = recomb_matrix[i][j-1] + (double) (phys_genome[i][j]-phys_genome[i][j-1]) / scale2 ;
            else
                recomb_matrix[i][j] = recomb_matrix[i][j-1] + (double) (phys_genome[i][j]-phys_genome[i][j-1]) / scale ;
        }
    }

    return(recomb_matrix);
}

void ShowGenomeBPcM(int nchrom,  int **phys_genome, double **recomb_matrix ,double alpha, double beta, FILE *OUT) {
    int i,j;
    char ref,var;
    double ref_freq, var_freq;
    fprintf(OUT, "Marker\tChrom\tbp\tcM\tAlleles\tRef\tVar\tFreqRef\tFreqVar\n");
    for (i=1; i<=nchrom; i++ ) {
        for ( j=1; j<= phys_genome[i][0]; j++ ) {
            random_alleles(&ref,&var,&ref_freq, &var_freq, alpha, beta);
            if ( var>ref)
                fprintf(OUT, "c%dm%d\tc%d\t%d\t%f\t%c/%c\t%c\t%c\t%f\t%f\n",i,j,i,phys_genome[i][j],recomb_matrix[i][j],ref,var,ref,var,ref_freq,var_freq);
            else
                fprintf(OUT, "c%dm%d\tc%d\t%d\t%f\t%c/%c\t%c\t%c\t%f\t%f\n",i,j,i,phys_genome[i][j],recomb_matrix[i][j],var,ref,ref,var,ref_freq,var_freq);

        }
    }
}


void FreeGenomeBPcM(int nchrom,  int **phys_genome, double **recomb_matrix , FILE *MEM_LOG ) {
    int i;
    if ( phys_genome != NULL && recomb_matrix!=NULL ) {
        for (i=1; i<=nchrom; i++ ) {
            free_DoubVec(  recomb_matrix[i]  , 1, phys_genome[i][0],  MEM_LOG);
            free_IntVec(  phys_genome[i] ,0, phys_genome[i][0],  MEM_LOG);
        }
        free_DoubPtrVec(recomb_matrix, 1, nchrom, MEM_LOG);
        free_IntPtrVec(phys_genome, 1, nchrom, MEM_LOG);
    }
}
