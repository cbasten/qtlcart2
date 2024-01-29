//
//  qcrma   QTL Cartographer Random Map
//
//  Created by Christopher Basten on 1/29/2024.
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
#include "qtl_sim.h"
#include "qtl_mapfunc.h"

#define MAXNAME 511
#define N_INTS 16777216

// define the parameters for this program:  These all enter through a parameter file
typedef struct local_params {
    int sim_chrom;
    int sim_len_bp;
    int sim_len_bp_sd;
    int sim_snps_chrom;
    int sim_snps_chrom_sd;
    int map_function;
    double sim_alpha;
    double sim_beta;
    double map_function_param;
    char *map_function_name;
    
    struct input_line *anchor;
}  the_params;

// Print the parameter from the local_params structure in the expected input format
void print_params( the_params *localparams, FILE *OUTPUT ) {
    
    fprintf(OUTPUT, "Key\tValue\tNote\n");
    fprintf(OUTPUT, "map_function_name\t%s\tName of the mapping function\n",localparams->map_function_name);
    fprintf(OUTPUT, "map_function_param\t%f\tMapping function parameter\n",localparams->map_function_param);
    fprintf(OUTPUT, "map_function\t%d\tInteger of the mapping function\n",localparams->map_function );
    fprintf(OUTPUT, "sim_chrom\t%d\tNumber of chromosomes to simulate\n",localparams->sim_chrom);
    fprintf(OUTPUT, "sim_len_bp\t%d\tMean length of chromosomes to simulate\n",localparams->sim_len_bp);
    fprintf(OUTPUT, "sim_len_bp_sd\t%d\tSD of chromosome length to simulate\n",localparams->sim_len_bp_sd);
    fprintf(OUTPUT, "sim_snps_chrom\t%d\tNumber of SNPs per chromosome to simulate\n",localparams->sim_snps_chrom);
    fprintf(OUTPUT, "sim_snps_chrom_sd\t%d\tSD of SNPs per chromosomes to simulate\n",localparams->sim_snps_chrom_sd);
    fprintf(OUTPUT, "sim_alpha\t%f\tAlpha parameter of Beta to simulate allele frequencies\n",localparams->sim_alpha);
    fprintf(OUTPUT, "sim_beta\t%f\tBeta parameter of Beta to simulate allele frequencies\n",localparams->sim_beta);

    
}

// Either read parameters from the parameter file, or write the default values for editing
the_params *set_params(char *param_file,FILE *LOG,  FILE *MEMLOG) {
    the_params *return_params;
    struct input_line *parameters;
    long lineno=0;
    int k_col, v_col, is_param;
    char   *param_value;
    char *buffer=NULL;
    FILE *OUTPUT;

    return_params = (the_params *) malloc((unsigned) sizeof(the_params));
    return_params->sim_chrom = 6;
    return_params->sim_len_bp = 1000000;
    return_params->sim_len_bp_sd = 100000;
    return_params->sim_snps_chrom = 1500;
    return_params->sim_snps_chrom_sd = 150;
    return_params->map_function = 1;
    return_params->map_function_param = 1.0;
    return_params->map_function_name = CharVec(0,MAXNAME,MEMLOG);
    return_params->sim_alpha=0.5;
    return_params->sim_beta=0.5;
    strcpy(return_params->map_function_name, "haldane");
    if ( (is_param=isfile(param_file))==0 ) {
        OUTPUT = fileopen(param_file,"w");
        print_params(return_params, OUTPUT);
        fprintf(LOG, "No params file %s:  will generate with defaults for editing\n", param_file);
        printf( "No params file %s:  will generate with defaults for editing and exit\n", param_file);
        exit(1);
    }
    parameters = ReadParseFile(param_file, &lineno);
    buffer = CharVec(0,MAXNAME,MEMLOG);
    k_col = FindColumnHead( parameters, "key", buffer);
    v_col = FindColumnHead( parameters, "value", buffer);
    if ( k_col >0 && v_col > 0 ) {
        param_value = FindKeyValue( parameters, k_col,v_col, "map_function_name" );
        if ( param_value !=NULL) {
            param_value = strupr(param_value);
            if ( LOG != NULL)
                fprintf(LOG,"map_function_name : %s\n", param_value);
            strcpy(return_params->map_function_name  , param_value );
            return_params->map_function = MapFunctionFromName(param_value);
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "map_function_param" );
        if ( param_value !=NULL) {
            if ( LOG != NULL)
                fprintf(LOG,"map_function_param : %s\n", param_value);
            return_params->map_function_param= atof( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_alpha" );
        if ( param_value !=NULL) {
            if ( LOG != NULL)
                fprintf(LOG,"random allele freq alpha : %s\n", param_value);
            return_params->sim_alpha= atof( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_beta" );
        if ( param_value !=NULL) {
            if ( LOG != NULL)
                fprintf(LOG,"random allele freq beta : %s\n", param_value);
            return_params->sim_beta= atof( param_value );
        }
       param_value = FindKeyValue( parameters, k_col,v_col, "sim_chrom" );
        if ( param_value !=NULL) {
            if ( LOG != NULL)
                fprintf(LOG,"sim_chrom : %s\n", param_value);
            return_params->sim_chrom = atoi( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_len_bp" );
        if ( param_value !=NULL) {
            if ( LOG != NULL)
                fprintf(LOG,"sim_len_bp : %s\n", param_value);
            return_params->sim_len_bp= atoi( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_len_bp_sd" );
        if ( param_value !=NULL) {
            if ( LOG != NULL)
                fprintf(LOG,"sim_len_bp_sd : %s\n", param_value);
            return_params->sim_len_bp_sd= atoi( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_snps_chrom" );
        if ( param_value !=NULL) {
            if ( LOG != NULL)
                fprintf(LOG,"sim_snps_chrom : %s\n", param_value);
            return_params->sim_snps_chrom= atoi( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_snps_chrom_sd" );
        if ( param_value !=NULL) {
            if ( LOG != NULL)
                fprintf(LOG,"sim_snps_chrom_sd : %s\n", param_value);
            return_params->sim_snps_chrom_sd= atoi( param_value );
        }
    }
     
    while ( parameters != NULL)
        parameters = DeAllocInputLine(parameters);
    if ( buffer !=NULL )
        free_CharVec(  buffer  ,  0, MAXNAME,   MEMLOG);
    return(return_params);
}

// Print a usage banner
void usage_banner(char *param_file, char *log_file, char *output_file, char *mem_file,int quit,FILE *OUTPUT) {
    fprintf(OUTPUT,"qrma :  Create a random genetic map\n");
    fprintf(OUTPUT,"\t-P  parameter file :  %s\n",param_file);
    fprintf(OUTPUT,"\t-l  log file       :  %s\n",log_file);
    fprintf(OUTPUT,"\t-o  output file    :  %s\n",output_file);
    fprintf(OUTPUT,"\t-M  memory file    :  %s\n",mem_file);
    fprintf(OUTPUT,"Will exit if -h on command line, or there is no parameter file\n");
    if ( quit==1)
        exit(1);
}

int main(int argc, const char * argv[], char *env[]) {
    FILE *MEMLOG=NULL, *OUT=NULL, *LOG=OUT;
    int i;
    char *cstring=NULL, *param_file=NULL, *log_file = NULL, *output_file=NULL,*mem_file=NULL;
    char *time_buffer, *this_time;
    long ran_seed=0;
    int **simulated_physical_genome;
    double **cm_matrix;
    the_params *local_params;
// Set the default command line parameters, then process the arguments.
    param_file = CharVec(0,MAXNAME,NULL);
    strcpy(param_file,"qcrma_rc.tsv");
    log_file = CharVec(0,MAXNAME,NULL);
    strcpy(log_file, "qcrma.log");
    output_file = CharVec(0,MAXNAME,NULL);
    strcpy(output_file, "qcrma_map.tsv");
    mem_file = CharVec(0,MAXNAME,NULL);
    strcpy(mem_file,"qcrma_mem.txt");
    for ( i=1; i<argc; i++ ) {
        if (  ( strcmp(argv[i], "-P") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
                strcpy(param_file , argv[i+1]);  // parameter file
        if (  ( strcmp(argv[i], "-l") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
                strcpy(log_file , argv[i+1]); // log file
        if (  ( strcmp(argv[i], "-o") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
                strcpy(output_file , argv[i+1]); // output file
        if (  ( strcmp(argv[i], "-M") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
                strcpy(mem_file , argv[i+1]); // Memory tracking file
        if (  ( strcmp(argv[i], "-S") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
                ran_seed = atol(argv[i+1]);// random number seed
        if (  ( strcmp(argv[i], "-h") == 0  )   )
            usage_banner(param_file, log_file, output_file, mem_file,1, stdout);
    }

// If mem_file is a string, create it to track memory usage.
    if ( !(strcmp(mem_file,"NULL")==0) ) {
        MEMLOG = fileopen(mem_file,"w");
        fprintf(MEMLOG, "Function\tType\tLower\tUpper\tNumber\tAddress\n");
    } else
        MEMLOG=NULL;
// Get the time
    time_buffer = CharVec(0,MAXNAME,MEMLOG);
    this_time = thistime( time_buffer, MAXNAME);

// If log file is not the string NULL, then print a message
    if ( !(strcmp(log_file,"NULL")==0 ) ) {
        LOG= fileopen(log_file, "a");
        usage_banner(param_file, log_file, output_file, mem_file,0, LOG);
        fprintf(LOG,"It is currently %s\n", this_time);
        if (MEMLOG!=NULL)
            fprintf(LOG,"Memory tracking to %s\n",mem_file);
    } else
        LOG = stdout ;
    
    local_params = set_params(param_file,  LOG,MEMLOG);

    cstring = CharVec(0,MAXNAME,MEMLOG);
    strcpy(cstring, SEED_FILE);
    set_ranseed(ran_seed, cstring);

    simulated_physical_genome = sim_phys_genome( local_params->sim_chrom,  local_params->sim_len_bp, local_params->sim_len_bp_sd, local_params->sim_snps_chrom, local_params->sim_snps_chrom_sd,   MEMLOG  );
    cm_matrix = recomb_matrix( simulated_physical_genome, local_params->sim_chrom, MEMLOG, LOG );

    if ( !(strcmp(output_file,"NULL")==0 ) ) {
        OUT=fileopen(output_file,"w");
        ShowGenomeBPcM(local_params->sim_chrom, simulated_physical_genome, cm_matrix  ,local_params->sim_alpha, local_params->sim_beta, OUT);
        fileclose(output_file, OUT);
    }
    FreeGenomeBPcM(local_params->sim_chrom,  simulated_physical_genome, cm_matrix  ,  MEMLOG );

    if ( time_buffer !=NULL )
        free_CharVec(  time_buffer  ,  0, MAXNAME , MEMLOG);
    if ( output_file != NULL )
        free_CharVec(  output_file ,  0, MAXNAME, MEMLOG);
    if ( MEMLOG != NULL)
        fileclose(mem_file, MEMLOG);
    if ( mem_file != NULL )
        free_CharVec(  mem_file ,  0, MAXNAME, MEMLOG);
    if ( LOG != NULL )
        fileclose(log_file, LOG);
    if ( log_file != NULL)
        free_CharVec(  log_file ,  0, MAXNAME, MEMLOG);
    if ( param_file != NULL)
        free_CharVec(  param_file ,  0, MAXNAME, MEMLOG);

    return 0;
}
