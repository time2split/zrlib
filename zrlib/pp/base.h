#ifndef ZRPP_H
#error "No manual include allowed"
#endif

#define ZREXPAND ZRXPAND
#define ZRXPAND(...) __VA_ARGS__
#define ZRTRASH(...)
#define ZREMPTY()
#define ZRSTR(...) # __VA_ARGS__

#define ZRIS_PARENTH(P) CHECK(_ZRIS_PARENTH P)
#define _ZRIS_PARENTH() PROBE()

#define ZRCONCAT(_1,_2)         ZRCONCAT_2(_1,_2)
#define ZRCONCAT_2(_1,_2)       ZRCONCAT_(_1,_2)
#define ZRCONCAT_3(_1,_2,_3)    ZRCONCAT_2(_1, ZRCONCAT_2(_2,_3))
#define ZRCONCAT_4(_1,_2,_3,_4) ZRCONCAT_2(_1, ZRCONCAT_3(_2,_3,_4))
#define ZRCONCAT_(A,B) A ## B

#define ZRCAT(...) ZRCONCAT_2(ZRCAT,ZRNARGS(__VA_ARGS__))(__VA_ARGS__)
#define ZRCAT2(A,B) _ZRCAT(A,B)
#define ZRCAT3(A,B,C) ZRCAT2(A,ZRCAT2(B,C))
#define ZRCAT4(A,B,C,D) ZRCAT2(A,ZRCAT3(B,C,D))
#define _ZRCAT(A,B) A ## B

