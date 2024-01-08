//
//  qtl_ran.h
//  qtlcart2
//
//  Created by Christopher Basten on 1/5/24.
//

#ifndef qtl_ran_h
#define qtl_ran_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ranlib.h"
#define RAN_SEED 20051215
#define SEED_FILE "qtl_seed.txt"

extern long ix;
void set_ranseed(long int ranseed, char *seedfile );
void print_ranseed( char *seedfile );
double ranf( long int inix );
long iran(long int xix, long int in);
double *ran_arry(int lr, int ur,  double *arry) ;
double ranf(long int inix) ;
void shuffle_ivector( int *v, int lb, int ub );

/* Prototypes for all user accessible RANLIB routines */

extern void advnst(long k);
extern double genbet(double aa,double bb);
extern double genchi(double df);
extern double genexp(double av);
extern double genf(double dfn, double dfd);
extern double gengam(double a,double r);
extern void genmn(double *parm,double *x,double *work);
extern void genmul(long n,double *p,long ncat,long *ix);
extern void genmul2(long n,long ncat,long *ix);
extern double gennch(double df,double xnonc);
extern double gennf(double dfn, double dfd, double xnonc);
extern double gennor(double av,double sd);
extern void genprm(long *iarray,int larray);
extern double genunf(double low,double high);
extern void getsd(long *iseed1,long *iseed2);
extern void gscgn(long getset,long *g);
extern long ignbin(long n,double pp);
extern long ignnbn(long n,double p);
extern long ignlgi(void);
extern long ignpoi(double mu);
extern long ignuin(long low,long high);
extern void initgn(long isdtyp);
extern long mltmod(long a,long s,long m);
extern void phrtsd(char* phrase,long* seed1,long* seed2);
extern double RANF(void);
extern void setall(long iseed1,long iseed2);
extern void setant(long qvalue);
extern void setgmn(double *meanv,double *covm,long p,double *parm);
extern void setsd(long iseed1,long iseed2);
extern double sexpo(void);
extern double sgamma(double a);
extern double snorm(void);
extern long lennob( char *str );
extern void gsrgs(long getset,long *qvalue);
extern void gssst(long getset,long *qset);
extern double fsign( double num, double sign );
extern void inrgcm(void);








#endif /* qtl_ran_h */
