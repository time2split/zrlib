#ifndef ZRMACRO_MATH
#define ZRMACRO_MATH

#include <zrlib/pp.h>

#define ZRMIN_2(a,b) ((a) < (b) ? (a) : (b))
#define ZRMIN_X(A,B) ZRMIN_2(A,B)
#define ZRMIN(...) ZRARGS_XAPPLYREC_E(ZRMIN_X,ZRXARGS_,__VA_ARGS__)

#define ZRMAX_2(a,b) ((a) > (b) ? (a) : (b))
#define ZRMAX_X(A,B) ZRMAX_2(A,B)
#define ZRMAX(...) ZRARGS_XAPPLYREC_E(ZRMAX_X,ZRXARGS_,__VA_ARGS__)

#define ZRISPOW2(I) (((I) & ((I) - 1)) == 0)
#define ZRISPOW2SAFE(I) ((I) > 0 && ZRISPOW2(I))

#endif
