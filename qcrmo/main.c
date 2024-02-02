//
//  qcrmo   QTL Cartographer Random Model
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
    double sim_alpha;
    double sim_beta;
    
    
    
    int map_function;
    double map_function_param;
    char *map_function_name;
    
    char *param_file;
    char *input_file;
    char *output_file;
    char *log_file;
    char *init_time;
    char *end_time;
    char *seed_file;
    int verbosity;
    long ran_seed;

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
void dealloc_params(the_params *lparams, FILE *MEMLOG) {
    if (lparams!=NULL ) {
        if (lparams->map_function_name != NULL )
            free_CharVec(lparams->map_function_name , 0, MAXNAME,MEMLOG);
        if (lparams->param_file != NULL )
            free_CharVec(lparams->param_file , 0, MAXNAME,MEMLOG);
        if (lparams->log_file != NULL )
            free_CharVec(lparams->log_file , 0, MAXNAME,MEMLOG);
        if (lparams->output_file != NULL )
            free_CharVec(lparams->output_file , 0, MAXNAME,MEMLOG);
        if (lparams->init_time != NULL )
            free_CharVec(lparams->init_time , 0, MAXNAME,MEMLOG);
        if (lparams->end_time != NULL )
            free_CharVec(lparams->end_time , 0, MAXNAME,MEMLOG);
        if (lparams->seed_file != NULL )
            free_CharVec(lparams->seed_file , 0, MAXNAME,MEMLOG);

        free( (char*) lparams );
        
    }
}
the_params *create_default_params(  FILE *MEMLOG) {
    the_params *return_params;
    char *this_time;
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
    
    return_params->param_file = CharVec(0,MAXNAME,MEMLOG);
    strcpy(return_params->param_file,"qcrma_rc.tsv");
    return_params->log_file = CharVec(0,MAXNAME,MEMLOG);
    strcpy(return_params->log_file, "qcrma.log");
    return_params->output_file = CharVec(0,MAXNAME,MEMLOG);
    strcpy(return_params->output_file, "qcrma_map.tsv");
    // Get the time
    return_params->init_time  = CharVec(0,MAXNAME,MEMLOG);
    this_time = thistime( return_params->init_time, MAXNAME);
    return_params->end_time  = CharVec(0,MAXNAME,MEMLOG);
    return_params->seed_file = CharVec(0,MAXNAME,MEMLOG);
    strcpy(return_params->seed_file, SEED_FILE);
    return_params->verbosity=1;

    return(return_params);
    
}


// Ignore if param_file is "NULL", then either read parameters from the parameter file, or write the default values for editing

int set_params_from_file(the_params *return_params, char *param_file,FILE *LOG,  FILE *MEMLOG) {
 
    struct input_line *parameters;
    long lineno=0;
    int k_col, v_col, is_param;
    char   *param_value;
    char *buffer=NULL;
    FILE *OUTPUT;

    if ( !strcmp(return_params->param_file, "NULL") )
        return(0);// If param file is set to NULL, then ignore it.
    if ( (is_param=isfile(param_file))==0 ) {  //if no param file, then write one and exit
        OUTPUT=fileopen(return_params->param_file, "w");
        print_params(return_params, OUTPUT);
        fileclose(return_params->param_file, OUTPUT);
        exit(0);
    }
    // else read the params from the file.
    is_param=0;
    parameters = ReadParseFile(param_file, &lineno);
    buffer = CharVec(0,MAXNAME,MEMLOG);
    k_col = FindColumnHead( parameters, "key", buffer);
    v_col = FindColumnHead( parameters, "value", buffer);
    if ( k_col >0 && v_col > 0 ) {
        param_value = FindKeyValue( parameters, k_col,v_col, "map_function_name" );
        if ( param_value !=NULL) {
            is_param++;
            param_value = strupr(param_value);
            if ( LOG != NULL)
                fprintf(LOG,"map_function_name : %s\n", param_value);
            strcpy(return_params->map_function_name  , param_value );
            return_params->map_function = MapFunctionFromName(param_value);
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "map_function_param" );
        if ( param_value !=NULL) {
            is_param++;
            if ( LOG != NULL)
                fprintf(LOG,"map_function_param : %s\n", param_value);
            return_params->map_function_param= atof( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_alpha" );
        if ( param_value !=NULL) {
            is_param++;
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
            is_param++;
            if ( LOG != NULL)
                fprintf(LOG,"sim_chrom : %s\n", param_value);
            return_params->sim_chrom = atoi( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_len_bp" );
        if ( param_value !=NULL) {
            is_param++;
            if ( LOG != NULL)
                fprintf(LOG,"sim_len_bp : %s\n", param_value);
            return_params->sim_len_bp= atoi( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_len_bp_sd" );
        if ( param_value !=NULL) {
            is_param++;
            if ( LOG != NULL)
                fprintf(LOG,"sim_len_bp_sd : %s\n", param_value);
            return_params->sim_len_bp_sd= atoi( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_snps_chrom" );
        if ( param_value !=NULL) {
            is_param++;
            if ( LOG != NULL)
                fprintf(LOG,"sim_snps_chrom : %s\n", param_value);
            return_params->sim_snps_chrom= atoi( param_value );
        }
        param_value = FindKeyValue( parameters, k_col,v_col, "sim_snps_chrom_sd" );
        if ( param_value !=NULL) {
            is_param++;
            if ( LOG != NULL)
                fprintf(LOG,"sim_snps_chrom_sd : %s\n", param_value);
            return_params->sim_snps_chrom_sd= atoi( param_value );
        }
    }
     
    while ( parameters != NULL)
        parameters = DeAllocInputLine(parameters);
    if ( buffer !=NULL )
        free_CharVec(  buffer  ,  0, MAXNAME,   MEMLOG);
    return(is_param);
}
// Print a usage banner
void usage_banner(the_params *local_params, char *mem_file,int quit,FILE *OUTPUT) {
    fprintf(OUTPUT,"qcrma :  Create a random genetic map\n");
    fprintf(OUTPUT,"\t-P  parameter file     :  %s\n",local_params->param_file);
    fprintf(OUTPUT,"\t-l  log file           :  %s\n",local_params->log_file);
    fprintf(OUTPUT,"\t-o  output file        :  %s\n",local_params->output_file);
    if ( mem_file != NULL )
        fprintf(OUTPUT,"\t-M  memory file        :  %s\n",mem_file);
    fprintf(OUTPUT,"\t-m  map function       :  %s\n",local_params->map_function_name);
    fprintf(OUTPUT,"\t-p  map func param     :  %f\n",local_params->map_function_param);
    fprintf(OUTPUT,"\t-c  chromosomes        :  %d\n",local_params->sim_chrom);
    fprintf(OUTPUT,"\t-b  ave. chrom len     :  %d\n",local_params->sim_len_bp);
    fprintf(OUTPUT,"\t-d  sd of len          :  %d\n",local_params->sim_len_bp_sd);
    fprintf(OUTPUT,"\t-s  ave. snps per chrom:  %d\n",local_params->sim_snps_chrom);
    fprintf(OUTPUT,"\t-t  sd snps per chrom  :  %d\n",local_params->sim_snps_chrom_sd);
    fprintf(OUTPUT,"\t-A  alpha              :  %f\n",local_params->sim_alpha);
    fprintf(OUTPUT,"\t-B  beta               :  %f\n",local_params->sim_beta);

    fprintf(OUTPUT,"Will exit if -h on command line\n");
    fprintf(OUTPUT,"Will ignore parameter file if set to NULL\n");
    fprintf(OUTPUT,"If set to a file that doesn't exist, then create default parameter file and exit.\n");
    fprintf(OUTPUT,"-V disables verbosity, or use with an integer to set the level\n");
    fprintf(OUTPUT,"Program initiated at %s\n", local_params->init_time);
    if ( quit==1)
        exit(1);
}
/*
 
 -c chroms
 -b bplen per chrom
 -d sd bp len per chrom
 
 -s snps per chrom
 -t sd snps per chrom
 
 -m map func name
 -p map func param
 
 
 -A alpha
 -B beta
 -S random number seed
 
 */
int set_params_from_cmd(the_params *local_params, int argc, const char * argv[], char *env[], char *mem_file) {
    int is_param=0, i;
    for ( i=1; i<argc; i++ ) {
        if (  ( strcmp(argv[i], "-l") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            strcpy(local_params->log_file , argv[i+1]); // log file
        if (  ( strcmp(argv[i], "-o") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            strcpy(local_params->output_file , argv[i+1]); // output file
        if (  ( strcmp(argv[i], "-S") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->ran_seed = atol(argv[i+1]);// random number seed
        if (  ( strcmp(argv[i], "-c") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->sim_chrom = atoi(argv[i+1]);// number of chromosomes
        if (  ( strcmp(argv[i], "-b") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->sim_len_bp = atoi(argv[i+1]);// average length of chroms in bp
        if (  ( strcmp(argv[i], "-d") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->sim_len_bp_sd = atoi(argv[i+1]);// standard dev
        if (  ( strcmp(argv[i], "-s") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->sim_snps_chrom = atoi(argv[i+1]);// number of snps per chromosome
        if (  ( strcmp(argv[i], "-t") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->sim_snps_chrom_sd = atoi(argv[i+1]);// sd
        if (  ( strcmp(argv[i], "-m") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) ) {
            strcpy(local_params->map_function_name , argv[i+1]); // map fuction name
            local_params->map_function = MapFunctionFromName(local_params->map_function_name );
        }
        if (  ( strcmp(argv[i], "-p") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->map_function_param = atof(argv[i+1]);// map function param
        if (  ( strcmp(argv[i], "-A") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->sim_alpha = atof(argv[i+1]);// alpha for Beta dist
        if (  ( strcmp(argv[i], "-B") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
            local_params->sim_beta = atof(argv[i+1]);//beta for Beta dist
        if (  ( strcmp(argv[i], "-V") == 0  ) ){
            if ( i+1 < argc && ! (argv[i+1][0] == '-' ) )
                local_params->verbosity = atoi(argv[i+1]);// standard dev
            else
                local_params->verbosity=0;
        }
        if (  ( strcmp(argv[i], "-h") == 0  )   )
            usage_banner(local_params, mem_file,1, stdout);
    }
    set_ranseed(local_params->ran_seed, local_params->seed_file);  // Set the random number seed for all random number generators
    return(is_param);
}



int main(int argc, const char * argv[], char *env[]) {
    FILE *MEMLOG=NULL, *OUT=NULL, *LOG=OUT;
    int i, nparams_read;
    char  *mem_file=NULL, *this_time;
 
    int **simulated_physical_genome;
    double **cm_matrix;
    the_params *local_params;
#if defined(MEMDEBUG)
    mem_file = CharVec(0,MAXNAME,NULL);
    strcpy(mem_file,"qcrma_mem.txt");
    MEMLOG = fileopen(mem_file,"w");
    fprintf(MEMLOG, "Function\tType\tLower\tUpper\tNumber\tAddress\n");
#endif
    local_params=create_default_params( MEMLOG );  // Create parameters, set defaults
    for ( i=1; i<argc; i++ ) { // Is there a parameter file set with a switch? Look for it.
        if (  ( strcmp(argv[i], "-P") == 0  ) && i+1 < argc && ! (argv[i+1][0] == '-' ) )
                strcpy(local_params->param_file , argv[i+1]);  // parameter file
        
    }
    nparams_read = set_params_from_file(local_params, local_params->param_file,NULL,  MEMLOG);  // Read from that file.
    nparams_read = set_params_from_cmd(local_params,argc, argv, env, mem_file);

// If log file is not the string NULL, then print a message
    if ( !(strcmp(local_params->log_file,"NULL")==0 ) ) {
        LOG= fileopen(local_params->log_file, "a");
        usage_banner(local_params, mem_file,0, LOG);
        if (MEMLOG!=NULL)
            fprintf(LOG,"Memory tracking to %s\n",mem_file);
    }
    if ( local_params->verbosity>0 )
        usage_banner(local_params, mem_file, 0, stdout);
    
    //Simulate a physical map, then the recombination matrix
    simulated_physical_genome = sim_phys_genome( local_params->sim_chrom,  local_params->sim_len_bp, local_params->sim_len_bp_sd, local_params->sim_snps_chrom, local_params->sim_snps_chrom_sd,   MEMLOG  );
    cm_matrix = recomb_matrix( simulated_physical_genome, local_params->sim_chrom, MEMLOG, LOG );

    if ( !(strcmp(local_params->output_file,"NULL")==0 ) ) {
        OUT=fileopen(local_params->output_file,"w");
        ShowGenomeBPcM(local_params->sim_chrom, simulated_physical_genome, cm_matrix  ,local_params->sim_alpha, local_params->sim_beta, OUT);
        fileclose(local_params->output_file, OUT);
    }
    FreeGenomeBPcM(local_params->sim_chrom,  simulated_physical_genome, cm_matrix  ,  MEMLOG );

    if ( (i =isfile(local_params->seed_file))==1 )   //print the seed to the seedfile if there is one.
        print_ranseed( local_params->seed_file );
    this_time = thistime( local_params->end_time, MAXNAME);
    if ( local_params->verbosity>0 )
        printf("Started at %s\nFinished at %s\nNow deallocating memory\n",local_params->init_time, local_params->end_time);
    if ( LOG != NULL ) {
        fprintf(LOG,"Started at %s\nFinished at %s\nNow deallocating memory\n",local_params->init_time, local_params->end_time);
        fileclose(local_params->log_file, LOG);
    }
    dealloc_params(local_params, MEMLOG);
    if ( MEMLOG != NULL)
        fileclose(mem_file, MEMLOG);
    if ( mem_file != NULL )
        free_CharVec(  mem_file ,  0, MAXNAME, MEMLOG);
    return 0;
}
