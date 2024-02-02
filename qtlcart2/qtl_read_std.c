//
//  qtl_read_std.c
//  qtlcart2
//
//  Created by Christopher Basten on 2/2/24.
//

#include "qtl_read_std.h"

gen_marker *AllocGenMarker( gen_marker *mptr , FILE *MEMLOG){
    gen_marker *r_marker=NULL;
    r_marker = (gen_marker *) malloc((unsigned) sizeof(gen_marker));
    if ( mptr!=NULL) {
        r_marker->prev = mptr;
        r_marker->next = mptr->next;
        mptr->next = r_marker;
        if ( r_marker->next != NULL)
            r_marker->next->prev = r_marker;
    }
    return(r_marker);
}
gen_marker *DeAllocGenMarker( gen_marker *mptr , FILE *MEMLOG ){
    gen_marker *r_marker=NULL;
    if (mptr==NULL)
        return(r_marker);
    if (mptr->prev == NULL) {
        r_marker=mptr->next;
        r_marker->prev = NULL;
    } else {
        r_marker=mptr->prev;
        r_marker->next = mptr->next;
        if ( r_marker->next !=NULL)
            r_marker->next->prev = r_marker;
    }
    
    free( (char*) mptr);
    return(r_marker);
}


void PrintGeneticMap( gen_marker *gen_map, FILE *OUT) {
    gen_marker *gptr;
    fprintf(OUT, "Marker\tChrom\tcM\n");
    for ( gptr=gen_map->next; gptr!=NULL; gptr=gptr->next)
        fprintf(OUT, "%s\t%s\t%f\n", gptr->name, gptr->chrom_name, gptr->cm);
}

/*
 Marker  Chrom   bp      cM      Alleles Ref     Var     FreqRef FreqVar
 c1m1    c1      783     0.000000        G/T     G       T       0.946960        0.053040
 c1m2    c1      2139    0.001356        A/G     G       A       0.749607        0.250393
 c1m3    c1      2908    0.002125        A/T     T       A       0.702105        0.297895
 c1m4    c1      3393    0.002610        C/G     C       G       0.765262        0.234738
 c1m5    c1      3689    0.002906        C/T     T       C       0.739427        0.260573

 */
gen_marker *read_std_marker_meta(char *marker_meta_file, FILE *LOG, FILE *MEMLOG ) {
    gen_marker *r_meta=NULL, *c_meta=NULL;
    char *buffer; 
    long lineno=0;
    struct input_line *meta_file, *mptr ;
    int chrom_col, bp_col, cm_col, alleles_col, ref_col, var_col, freq_col, varfreq_col ;
     
    meta_file = ReadParseFile(marker_meta_file, &lineno);
    buffer = CharVec(0,MAXTOKEN,MEMLOG);
    
    alleles_col = FindColumnHead( meta_file, "alleles", buffer);
    freq_col = FindColumnHead( meta_file, "freqref", buffer);
    varfreq_col = FindColumnHead( meta_file, "freqvar", buffer);
    ref_col = FindColumnHead( meta_file, "ref", buffer);
    var_col = FindColumnHead( meta_file, "var", buffer);

    
    
    
    chrom_col = FindColumnHead( meta_file, "chrom", buffer);
    if ( chrom_col < 0 )
        fprintf(LOG,"ERROR:  can not find column headed by *chrom*");
    bp_col = FindColumnHead( meta_file, "bp", buffer);
    if ( bp_col < 0 )
        fprintf(LOG,"ERROR:  can not find column headed by *bp*");
    cm_col = FindColumnHead( meta_file, "cm", buffer);
    if ( cm_col < 0 )
        fprintf(LOG,"ERROR:  can not find column headed by *cm*");
    if ( chrom_col<0 || bp_col<0 || cm_col<0 )
        return( NULL );
    r_meta = AllocGenMarker(NULL, MEMLOG);
    r_meta->marker_line=meta_file;
    c_meta = r_meta;
    for ( mptr=meta_file->next; mptr!=NULL; mptr=mptr->next ) {
        r_meta = AllocGenMarker(r_meta, MEMLOG);
        r_meta->marker_line = mptr;
        r_meta->chrom_name = mptr->fields[chrom_col] ; // must have a chromosome name
        r_meta->bp = atof( mptr->fields[bp_col]); // what if they are missing?
        r_meta->cm = atof( mptr->fields[cm_col]);  // might want to interpolate?
        r_meta->name = mptr->fields[1];
    }
    return( c_meta );
}
