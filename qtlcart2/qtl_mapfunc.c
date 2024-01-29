//
//  qtl_mapfunc.c
//  qtlcart2
//
//  Created by Christopher Basten on 1/22/24.
//

#include "qtl_mapfunc.h"
#include <string.h>
#define MAPDELTA 0.00001

double Haldane(double rr);
double iHaldane(double mm);
double Morgan(double rr);
double iMorgan(double mm);
double Kosambi(double rr);
double iKosambi(double mm);
double Rao(double rr, double  map_param);
double iRao(double mm, double map_param);
double Sturt(double rr , double map_param);
double iSturt(double mm,double  map_param);
double Karlin(double rr, double  map_param);
double iKarlin(double mm, double  map_param);
double Felsenstein(double rr, double  map_param);
double iFelsenstein(double mm, double  map_param);
double CarterFalconer(double rr);
double iCarterFalconer(double mm);

char *MapFunctionNameFromInt(int map_function){
    if ( map_function==2 )
        return("KOSAMBI");
    if ( map_function==3 )
        return("MORGAN");
    if ( map_function==4 )
        return("CARTERFALCONER");
    if ( map_function==5 )
        return("RAOETAL");
    if ( map_function==6 )
        return("STURT");
    if ( map_function==7)
        return("FELSENSTEIN");
    if ( map_function==8 )
        return("KARLIN");

    return("HALDANE");
}
int MapFunctionFromName(char *param_value) {
     
    if ( !strncmp(param_value, "HAL", 3) )
        return(1);
    if ( !strncmp(param_value, "KOS", 3) )
        return(2);
    if ( !strncmp(param_value, "MOR", 3) )
        return(3);
    if ( !strncmp(param_value, "CAR", 3) )
        return(4);
    if ( !strncmp(param_value, "RAO", 3) )
        return(5);
    if ( !strncmp(param_value, "STU", 3) )
        return(6);
    if ( !strncmp(param_value, "FEL", 3) )
        return(7);
    if ( !strncmp(param_value, "KAR", 3) )
        return(8);

    return(1);
}
/*
This is a function to convert between recombination
frequencies and distance in Morgans or centiMorgans.

value is what will be converted.
flag indicates how it will be converted:
flag =

 
   -3  => value cM to rvalue M
   -2  => value cM to rvalue Rec. Freq.
   -1  => value  M to rvalue Rec. Freq.
    0  => value is returned unchanged.
    1  => value Rec. Freq. to rvalue M
    2  => value Rec. Freq. to rvalue cM
    3  => value M to rvalue cM
  
 

The global variable whosemf determines which mapfunction to
use.

  whosemf =
             1 => Haldane (1919)
             2 => Kosambi (1944)
             3 => Morgan (1928) (Fixed)
             4 => Carter and Falconer (1951)
             5 => Rao et al (1977)
             6 => Sturt (1976)
             7 => Felsenstein (1979)
             8 => Karlin (1984)
See Ben Hui Liu (1998) "Statistical Genomics: Linkage, Mapping and QTL Analysis" p319
CRC Press

Send Morgans or r to the following functions:


Kosambi, iKosambi
Haldane, iHaldane
Morgan, iMorgan
CarterFalconer, iCarterFalconer
Rao, iRao
Sturt, iSturt
Felsenstein, iFelsenstein
Karlin, iKarlin


Check that
  0.0 < m
  0.0 < r < 0.5
*/
double mapfunc(double value, int flag, int whosemf, double mapparam) {
    double rvalue;
    double mval,rval;
    /* 0.0 < value
     0.0 < value < 0.5 if value is a recombination frequency */
    if ( flag == 0)
        return (value);
    if ( flag == -3 ) // cM to M
        return( 0.01*value );
    if ( flag == 3 ) // M to cM
        return( 100.0 *value);
    
    if ( value < (double) 0.0 )
        return((double) -1.0);  /* -1 for a negative distance/probability */
    else if ( value == (double) 0.0 )
        return((double) 0.0);
    if ( flag > 0 && value >= (double) 0.5 ) /* -2 for a rec. prob. >= 1/2 */
        return((double) -2.0);
    
    mval = rval = value;
    if (flag == -2 )
        mval = mval* (double) 0.01;
    
    
    if ( flag < 0 ) {
        switch(whosemf) {
            default: case 1: rval = iHaldane(mval);  break;
            case 2: rval = iKosambi(mval);  break;
            case 3: rval = iMorgan(mval);  break;
            case 4: rval = iCarterFalconer(mval);  break;
            case 5: rval = iRao(mval,mapparam);  break;
            case 6: rval = iSturt(mval,mapparam);  break;
            case 7: rval = iFelsenstein(mval,mapparam);  break;
            case 8: rval = iKarlin(mval,mapparam);  break;
        }
    }  else if ( flag > 0 ) {
        switch(whosemf) {
            default: case 1: mval = Haldane(rval);  break;
            case 2: mval = Kosambi(rval);  break;
            case 3: mval = Morgan(rval);  break;
            case 4: mval = CarterFalconer(rval);  break;
            case 5: mval = Rao(rval,mapparam);  break;
            case 6: mval = Sturt(rval,mapparam);  break;
            case 7: mval = Felsenstein(rval,mapparam);  break;
            case 8: mval = Karlin(rval,mapparam);  break;
        }
    }
    
    
    if (flag == 2 )  /*change Morgans to Centimorgans*/
        rvalue = mval* (double) 100.0;
    else if ( flag == 1 )
        rvalue = mval;
    else if ( flag == -1 || flag == -2)
        rvalue = rval;
    else
        rvalue = value;
    return (rvalue);
}

/*Inverse of Kosambi mapping function*/
double iKosambi(double mm) {
    double rr;
    rr = (double) 0.5 * ((double) 1.0 - (double) exp( -4.0 * mm)) / ((double) 1.0 + (double) exp( -4.0 * mm));
    return (rr);
}
/*Kosambi mapping function*/
double Kosambi(double rr) {
    double mm;
    mm = (double) 0.25 * (double) log((1.0 + 2.0 * rr) / (1.0 - 2.0 * rr));
    return (mm);
}

/*Inverse of Morgan mapping function*/
double iMorgan(double mm) {
    double rr;
    rr = mm;
    return (rr);
}
/*Morgan mapping function*/
double Morgan(double rr) {
    double mm;
    mm = rr;
    return (mm);
}

/*Inverse of Hadane mapping function*/
double iHaldane(double mm) {
    double rr;
    rr = (double) 0.5 * ((double) 1.0 - (double) exp(-2.0 * mm));;
    return (rr);
}
/*Hadane mapping function*/
double Haldane(double rr) {
    double mm;
    mm = (double) -0.5 * (double) log(1.0 - 2.0 * rr);
    return (mm);
}
/*Inverse of CarterFalconer mapping function*/
double iCarterFalconer(double mm) {
    double rr,ru,rl,delta,mt;
    delta = (double) MAPDELTA;
    rl = delta;
    ru = (double) 0.5-delta;
    do {
        rr = (double) 0.5*(rl+ru);
        mt = CarterFalconer(rr);
        if ( mt > mm )
            ru = rr;
        else
            rl = rr;
    } while ( fabs(mt-mm) > delta );
    return (rr);
}
/*CarterFalconer mapping function*/
double CarterFalconer(double rr) {
    double mm;
    mm = (double) 0.5*( (double) atan(2.0*rr) + (double) 0.5* (double) log( (1.0+2.0*rr)/(1.0-2.0*rr) ) ) ;
    return (mm);
}
/*Inverse of Felsenstein mapping function*/
double iFelsenstein(double mm, double map_param) {
    double rr ;
    rr = ((double) 1.0- (double) exp(2.0*(map_param-2.0)*mm))/((double) 2.0*((double) 1.0-(map_param- (double) 1.0)* (double) exp(2.0*(map_param-2.0)*mm)));
    return (rr);
}
/*Felsenstein mapping function,  map_param is the map_parameter*/
double Felsenstein(double rr, double map_param)
{
    double mm ;   /* map_param can't be 2 */
    mm = (double) log( (1.0-2.0*rr)/(1.0-2.0*(map_param-1.0)*rr) )/((double) 2.0*(map_param-(double) 2.0)) ;
    return (mm);
}

/*Inverse of Karlin mapping function*/
double iKarlin(double mm, double map_param) {
    double rr ;
    rr = (double) 0.5*((double) 1.0 - (double) pow((1.0-2.0*mm/map_param),map_param) );
    return (rr);
}
/*Karlin mapping function*/
double Karlin(double rr, double map_param) {
    double mm;
    mm = (double) 0.5 * map_param *((double) 1.0- (double) pow((1.0-2.0*rr),1.0/map_param));
    return (mm);
}
/*Inverse of Rao mapping function*/
double iRao(double mm,double map_param) {
    double rr,ru,rl,delta,mt;
    delta = (double) MAPDELTA;
    rl = delta;
    ru = (double) 0.5-delta;
    do {
        rr = (double) 0.5*(rl+ru);
        mt = Rao(rr,map_param);
        if ( mt > mm )
            ru = rr;
        else
            rl = rr;
    } while ( fabs(mt-mm) > delta );
    return (rr);
}
/*Rao mapping function*/

double Rao(double rr, double map_param) {
    double mm ;
    mm = map_param*((double)2.0*map_param-(double)1.0)*((double)1.0-(double)4.0*map_param)* (double) log(1.0-2.0*rr)/(double)6.0 + ((double)8.0*map_param*(map_param-(double)1.0)*((double)2.0*map_param-(double)1.0)*(double) atan(2.0*rr) +  map_param*((double)1.0-map_param)*((double)4.0*map_param+(double)1.0) * (double) log((1.0+2.0*rr)/(1.0-2.0*rr)))/(double)3.0 + ((double)1.0-map_param)*((double)1.0-(double)2.0*map_param)*((double)1.0-(double)4.0*map_param)*rr;
    return (mm);
}
/*Inverse of Sturt mapping function*/
double iSturt(double mm,double map_param) {
    double rr ;
    if ( mm < map_param )
        rr = (double) 0.5*((double) 1.0-((double) 1.0-mm/map_param)*(double) exp(mm*(1.0-2.0*map_param)/map_param) );
    else
        rr = (double) 0.5;
    return (rr);
}
/*Sturt mapping function*/
double Sturt(double rr , double map_param) {
    double mm,ml,mu,rt,delta;
    ml = delta = (double) MAPDELTA;
    do {
        mu = ml+ (double) 1.0;
        rt = iSturt(mu, map_param);
        if (rt < rr )
            ml = ml + (double) 1.0;
        
    } while ( rt < rr );
    do {
        mm = (double) 0.5*(ml+mu);
        rt = iSturt(mm, map_param);
        if ( rt > rr )
            mu = mm;
        else
            ml = mm;
    } while ( fabs(rt-rr) > delta );
    return (mm);
}

