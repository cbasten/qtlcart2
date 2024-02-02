//
//  qtl_read_std.h
//  qtlcart2
//
//  Created by Christopher Basten on 2/2/24.
//

#ifndef qtl_read_std_h
#define qtl_read_std_h

#include <stdio.h>
#include "qtl_string.h"
#include "qtl_fileio.h"

typedef struct a_marker {
    char *name;
    char *chrom_name;
    double cm;
    long bp;
    struct input_line *marker_line;
    struct a_marker *prev;
    struct a_marker *next;
}  gen_marker;

gen_marker *AllocGenMarker( gen_marker *mptr , FILE *MEMLOG);
gen_marker *DeAllocGenMarker( gen_marker *mptr , FILE *MEMLOG );
gen_marker *read_std_marker_meta(char *marker_meta_file, FILE *LOG, FILE *MEMLOG );
#endif /* qtl_read_std_h */
