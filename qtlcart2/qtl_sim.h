//
//  qtl_sim.h
//  qtlcart2
//
//  Created by Christopher Basten on 1/20/24.
//

#ifndef qtl_sim_h
#define qtl_sim_h

#include <stdio.h>
#include <math.h>
#include "qtl_mem.h"
#include "qtl_ran.h"

void swap(int* xp, int* yp);
void selectionSort(int arr[], int n);
void ShowGenomeBPcM(int nchrom,  int **phys_genome, double **recomb_matrix ,double alpha, double beta, FILE *OUT);
double **recomb_matrix( int **phys_genome, int nchrom , FILE *MEM_LOG, FILE *LOG);
int **sim_phys_genome( double nchrom,  double mean_len_bp, double sd_len_bp, double mean_snps, double sd_snps,   FILE *MEM_LOG  );
int *sim_phys_chrom( double mean_len_bp, double sd_len_bp, double mean_snps, double sd_snps,  FILE *MEM_LOG );
void FreeGenomeBPcM(int nchrom,  int **phys_genome, double **recomb_matrix , FILE *MEM_LOG ) ;
#endif /* qtl_sim_h */
