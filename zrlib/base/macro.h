/**
 * @author zuri
 * @date mardi 18 décembre 2018, 22:46:00 (UTC+0100)
 */

#ifndef ZRBASE_MACRO_H
#define ZRBASE_MACRO_H

#include <zrlib/pp.h>
#include <zrlib/macro.h>

#include <stdalign.h>
#include <stddef.h>

typedef size_t ZRID;
#define ZRID_ABSENT SIZE_MAX

typedef struct
{
	alignas(max_align_t) char buffer[2048];
} ZRInitInfos_t[1];

typedef size_t (*zrfhash)(void *a);
typedef size_t (*zrfuhash)(void *a, void *data);

typedef int (*zrfcmp)(void *a, void *b);
typedef int (*zrfucmp)(void *a, void *b, void *data);

#define ZRSWAP(type,a,b) ZRBLOCK( \
	type _c; \
	_c = a, a = b, b = _c; \
)

#define ZRPTYPE_CPY(a,b) memcpy((a), (b), sizeof(*a))
#define ZRPTYPE_CPY_NB(a,b,NB) memcpy((a), (b), sizeof(*a) * (NB))
#define ZRPTYPE_0(a) memset((a), 0, sizeof(*a))
#define ZRPTYPE_OBJECTP(a) ZROBJECTP_DEF( ZROBJINFOS_DEF(alignof(*a), sizeof(*a)), (a))

#define ZRSTRING_OBJECTP(S) ZROBJECTP_DEF( ZROBJINFOS_DEF(alignof(char), strlen(S) + 1), S )

#define ZRTYPE_SIZE_ALIGNMENT(T) sizeof(T), __alignof(T)
#define ZRTYPE_ALIGNMENT_SIZE(T) __alignof(T), sizeof(T)
#define ZRTYPE_OBJINFOS(T) ZROBJINFOS_DEF(__alignof(T), sizeof(T))
#define ZRTYPENB_OBJINFOS(T,NB) ZROBJINFOS_DEF(__alignof(T), sizeof(T) * (NB))
#define ZRTYPE_OBJALIGNINFOS(T) ZROBJALIGNINFOS_DEF(0, __alignof(T), sizeof(T))
#define ZRTYPENB_OBJALIGNINFOS(T,NB) ZROBJALIGNINFOS_DEF(0, __alignof(T), sizeof(T) * (NB))

#define ZRDEREF(P) *(P)
#define ZRADDR(P)  &(P)

#define ZRSIZE_UNKNOWN SIZE_MAX

#define _ZRLIST_VPOI_X(rest,V) V, ZRTYPE_OBJINFOS(*V) ZRIF(rest)(ZRPARENTHESIS())
#define ZRLIST_VARIABLESP_OBJINFOS(...) ZRARGS_XAPPLY_E(_ZRLIST_VPOI_X,ZRXARGS_R,__VA_ARGS__)
#define ZRPARENTHESIS() ,


#endif
