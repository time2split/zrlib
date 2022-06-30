#ifndef ZRPP_H
#error "No manual include allowed"
#endif

#define _ZRGETB(A,B,...) B

#define ZRCHECK(...) _ZRGETB(__VA_ARGS__,0)
#define ZRPROBE() ~,1


/* Boolean */


#define ZRCOMPL(B) ZRCAT2(ZRCOMPL_, B)
#define ZRCOMPL_0 1
#define ZRCOMPL_1 0


#define ZRNOT(B) ZRCHECK(ZRCAT2(_ZRNOT_, B)())
#define _ZRNOT_0() ZRPROBE()
#define _ZRNOT_() ZRPROBE()


#define ZRBOOL(B) ZRCOMPL(ZRCHECK(ZRCAT2(_ZRBOOL_IS_, B)()))
#define _ZRBOOL_IS_0() ZRPROBE()
#define _ZRBOOL_IS_() ZRPROBE()


#define ZRGT_1(N) ZRCOMPL(ZRCHECK(ZRCAT2(ZRGT_1_, N)()))
#define ZRGT_1_() ZRPROBE()
#define ZRGT_1_0() ZRPROBE()
#define ZRGT_1_1() ZRPROBE()


#define ZRBITOR(x) ZRCAT2(_ZRBITOR, x)
#define _ZRBITOR_0(y) y
#define _ZRBITOR_1(y) 1

#define ZRBITAND(x) ZRCAT2(_ZRBITAND_, x)
#define _ZRBITAND_0(y) 0
#define _ZRBITAND_1(y) y

#define ZRAND(...) ZRCONCAT(ZRAND_,ZRNARGS(__VA_ARGS__))(__VA_ARGS__)
#define ZRAND_2(_1,_2) ZRCONCAT(ZRBITAND_,ZRBOOL(_1))(ZRBOOL(_2))

#define ZROR(...) ZRCONCAT(ZROR_,ZRNARGS(__VA_ARGS__))(__VA_ARGS__)
#define ZROR_2(_1,_2) ZRCONCAT(ZRBITOR_,ZRBOOL(_1))(ZRBOOL(_2))


/* IF */


#define _ZRIF(B) __ZRIF(ZRBOOL(B))
#define _ZRNIF(B) __ZRIF(ZRNOT(B))
#define __ZRIF(B) ZRCAT2(__ZRIF_, B)
#define __ZRIF_0(t, ...) __VA_ARGS__
#define __ZRIF_1(t, ...) t


#define ZRIF(B) _ZRIF(B)(ZRXPAND,ZRTRASH)
#define ZRNIF(B) _ZRNIF(B)(ZRXPAND,ZRTRASH)
#define ZRIFELSE(B) _ZRIF(B)(_ZRIFELSE_XPAND_TRASH,_ZRIFELSE_TRASH_XPAND)
#define _ZRIFELSE_XPAND(...) __VA_ARGS__
#define _ZRIFELSE_XPAND_TRASH(...) __VA_ARGS__ ZRTRASH
#define _ZRIFELSE_TRASH_XPAND _ZRIFELSE_XPAND ZRTRASH
