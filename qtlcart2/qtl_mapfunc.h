//
//  qtl_mapfunc.h
//  qtlcart2
//
//  Created by Christopher Basten on 1/22/24.
//

#ifndef qtl_mapfunc_h
#define qtl_mapfunc_h

#include <stdio.h>
#include <math.h>
double mapfunc(double value, int flag, int whosemf, double mapparam);
int MapFunctionFromName(char *param_value);
char *MapFunctionNameFromInt(int map_function);
#endif /* qtl_mapfunc_h */
