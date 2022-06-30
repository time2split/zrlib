#ifndef ZRPP_H
#error "No manual include allowed"
#endif

#define ZRARGS_HEAD(H, ...) H
#define ZRARGS_TAIL(H, ...) __VA_ARGS__

#define ZRPREARGS(...) ZRIF(ZRARGS_NEMPTY(__VA_ARGS__))(__VA_ARGS__,)
#define ZRARGS_STR(...) ZREVAL2(ZRSTR ZRDEFER2() (ZRCAT(__VA_ARGS__)))

/*
 * EMPTY
 * https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 */

#define ZRARGS_EMPTY(...) ZRCHECK(ZRCAT2(_ZRARGS_IS_EMPTY_CASE_, _ZRARGS_EMPTY_BITS(__VA_ARGS__)))
#define ZRARGS_NEMPTY(...) ZRCOMPL(ZRARGS_EMPTY(__VA_ARGS__))

#define _ZRARGS_TRIGGER_PARENTHESIS_(...) ,
#define _ZRARGS_IS_EMPTY_CASE_0001 ZRPROBE()

/* test if there is just one argument, eventually an empty one */
/* test if _TRIGGER_PARENTHESIS_ together with the argument
 /* test if the argument together with a adds a comma */
/* test if placing it between _TRIGGER_PARENTHESIS_ and the parenthesis adds a comma */
#define _ZRARGS_EMPTY_BITS(...) ZRCAT4(\
			ZRARGS_HAS_COMMA(__VA_ARGS__), \
			ZRARGS_HAS_COMMA(_ZRARGS_TRIGGER_PARENTHESIS_ __VA_ARGS__), \
			ZRARGS_HAS_COMMA(__VA_ARGS__ (/*empty*/)), \
			ZRARGS_HAS_COMMA(_ZRARGS_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/)))

/*
 * NARGS
 */

#define ZRARGS_SIZE ZRNARGS0
#define ZRNARGS0(...) ZREVAL1(ZRIFELSE(ZRARGS_EMPTY(__VA_ARGS__)) \
	(0) \
	(ZRNARGS(__VA_ARGS__)) \
)
#define ZRNARGS(...) _ZRNARGS(__VA_ARGS__, _ZRNARGS_REVERSE)
#define _ZRNARGS(...) _ZRNARGS_GETN(__VA_ARGS__)

/*
 * EVEN/ODD
 */

#define _ZRARGS_XODD(X,nb,V, ...) \
	X(V) ZRIF(ZRGT_1(nb))(,_ZRARGS_XEVEN ZRDEFER2() (X,ZRDEC(nb),__VA_ARGS__))
#define _ZRARGS_XEVEN(X,nb,V, ...) _ZRARGS_XODD(X,ZRDEC(nb),__VA_ARGS__)

#define ZRARGS_XEVEN(X, ...) _ZRARGS_XEVEN(X,ZRDEC(ZRNARGS(__VA_ARGS__)),__VA_ARGS__)
#define ZRARGS_XODD(X, ...)  _ZRARGS_XODD(X,ZRDEC(ZRNARGS(__VA_ARGS__)),__VA_ARGS__)

#define _ZRARGS_EVENODD_EXPAND(...) __VA_ARGS__
#define ZRARGS_EVEN(...) ZRARGS_XEVEN(_ZRARGS_EVENODD_EXPAND,__VA_ARGS__)
#define ZRARGS_ODD(...)  ZRARGS_XODD(_ZRARGS_EVENODD_EXPAND,__VA_ARGS__)

#define ZRARGS_XEVEN_E(X,...) ZREVAL(ZRARGS_XEVEN(X,__VA_ARGS__))
#define ZRARGS_XODD_E(X,...)  ZREVAL(ZRARGS_XODD(X,__VA_ARGS__))
#define ZRARGS_EVEN_E(...)    ZREVAL(ZRARGS_EVEN(__VA_ARGS__))
#define ZRARGS_ODD_E(...)     ZREVAL(ZRARGS_ODD(__VA_ARGS__))

/*
 * XAPPLY
 */

#define ZRXARGS_NDR(nb,depth,rest) nb,depth,rest,
#define ZRXARGS_ND(nb,depth,rest) nb,depth,
#define ZRXARGS_NR(nb,depth,rest) nb,rest,
#define ZRXARGS_DR(nb,depth,rest) depth,rest,
#define ZRXARGS_N(nb,depth,rest) nb,
#define ZRXARGS_D(nb,depth,rest) depth,
#define ZRXARGS_R(nb,depth,rest) rest,
#define ZRXARGS_(nb,depth,rest)

#define ZRARGS_XAPPLY(X,XARGS,...) _ZRARGS_XAPPLY(X,XARGS,ZRNARGS(__VA_ARGS__),0,ZRNARGS(__VA_ARGS__),__VA_ARGS__)
#define ZRARGS_XAPPLY_E(X,XARGS,...) ZREVAL(ZRARGS_XAPPLY(X,XARGS,__VA_ARGS__))
#define _ZRARGS_XAPPLY(X, XARGS, nb_total, depth, rest, V, ...) \
	ZRIF(rest) \
	( \
		X ZRDEFER1() (XARGS(nb_total,depth,ZRDEC(rest))V) \
		_ZRARGS_XAPPLY_INDIRECT ZRDEFER2() () (X,XARGS,nb_total,ZRINC(depth),ZRDEC(rest),__VA_ARGS__) \
	)
#define _ZRARGS_XAPPLY_INDIRECT() _ZRARGS_XAPPLY

#define ZRARGS_XAPPLY_DATA(X,XARGS,data_list,...) _ZRARGS_XAPPLY_DATA(X,XARGS,data_list,ZRNARGS(__VA_ARGS__),0,ZRNARGS(__VA_ARGS__),__VA_ARGS__)
#define ZRARGS_XAPPLY_DATA_E(X,XARGS,data_list,...) ZREVAL(ZRARGS_XAPPLY_DATA(X,XARGS,data_list,__VA_ARGS__))
#define _ZRARGS_XAPPLY_DATA(X, XARGS, data_list, nb_total, depth, rest, V, ...) \
	ZRIF(rest) \
	( \
		X ZRDEFER1() (ZRPREARGS (ZREXPAND data_list) XARGS(nb_total,depth,ZRDEC(rest)) V) \
		_ZRARGS_XAPPLY_DATA_INDIRECT ZRDEFER2() () (X,XARGS,data_list,nb_total,ZRINC(depth),ZRDEC(rest),__VA_ARGS__) \
	)
#define _ZRARGS_XAPPLY_DATA_INDIRECT() _ZRARGS_XAPPLY_DATA

#define ZRARGS_XAPPLYREC(X,XARGS,...) _ZRARGS_XAPPLYREC(X,XARGS,ZRNARGS(__VA_ARGS__),0,ZRDEC(ZRDEC(ZRNARGS(__VA_ARGS__))),__VA_ARGS__)
#define ZRARGS_XAPPLYREC_E(X,XARGS,...) ZREVAL(ZRARGS_XAPPLYREC(X,XARGS,__VA_ARGS__))
#define _ZRARGS_XAPPLYREC(X, XARGS, nb_total, depth, rest, A, B, ...) \
	X ZRDEFERN(ZRINC(rest)) (XARGS(nb_total,depth,rest) A, \
	ZRIFELSE(rest) \
	( _ZRARGS_XAPPLYREC_INDIRECT ZRDEFER2() () (X, XARGS, nb_total, ZRINC(depth), ZRDEC(rest), B, __VA_ARGS__) ) \
	(B) \
)
#define _ZRARGS_XAPPLYREC_INDIRECT() _ZRARGS_XAPPLYREC

/*
 * Map
 */

#define ZRARGS_MAP_E(MAP,...) ZREVAL(ZRARGS_MAP(MAP,__VA_ARGS__))
#define ZRARGS_MAP(MAP,...) \
	MAP(ZRARGS_HEAD(__VA_ARGS__)) ZRNIF(ZRARGS_EMPTY(ZRARGS_TAIL(__VA_ARGS__)))(,ZRARGS_MAP ZRDEFER3() (MAP, ZRARGS_TAIL(__VA_ARGS__)))
