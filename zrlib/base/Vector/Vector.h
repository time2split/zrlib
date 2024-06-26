/**
 * @author zuri
 * @date samedi 19 janvier 2019, 18:03:00 (UTC+0100)
 */

#ifndef ZRVECTOR_H
#define ZRVECTOR_H

#include <zrlib/config.h>
#include <zrlib/base/Array.h>
#include <zrlib/base/Allocator/Allocator.h>

#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <string.h>

#pragma zrlib conf generate.target "/src/base/Vector/Vector.c" "."
#pragma zrlib conf generate.prefix ZRVector_

// ============================================================================

typedef struct ZRVectorS ZRVector;
typedef struct ZRVectorStrategyS ZRVectorStrategy;

// ============================================================================

struct ZRVectorStrategyS
{
	/*
	 * The insert/delete functions are responsible to update properly the vec.nbObj value.
	 */
	void (*finsert)(ZRVector *vec, size_t pos, size_t nb);
	void (*fdelete)(ZRVector *vec, size_t pos, size_t nb);

	void (*fchangeObjSize)(ZRVector *vec, ZRObjInfos objInfos);
	void (*fmemoryTrim)(ZRVector *vec);

	void (*fdestroy)(ZRVector *vec);
};

struct ZRVectorS
{
	/*
	 * The strategy for memory management and insertion/deletion routines.
	 */
	ZRVectorStrategy *strategy;

	ZRArray array;
};

#pragma zrlib generate function ZRArray array(ZRVector*)
#define ZRVECTOR_ARRAY(vec) (vec)->array

#pragma zrlib generate function void* arrayP(ZRVector*)
#define ZRVECTOR_ARRAYP(vec) (vec)->array.array

#pragma zrlib generate function size_t nbObj(ZRVector*)
#define ZRVECTOR_NBOBJ(vec) (vec)->array.nbObj

#pragma zrlib generate function size_t capacity(ZRVector*)
#define ZRVECTOR_CAPACITY(vec) (vec)->array.capacity

#pragma zrlib generate function ZRObjInfos objInfos(ZRVector*)
#define ZRVECTOR_OBJINFOS(vec) (vec)->array.objInfos

#pragma zrlib generate function size_t objSize(ZRVector*)
#define ZRVECTOR_OBJSIZE(vec) (vec)->array.objInfos.size

#pragma zrlib generate function size_t objAlignment(ZRVector*)
#define ZRVECTOR_OBJALIGNMENT(vec) (vec)->array.objInfos.alignment

// ============================================================================

static inline void ZRVECTOR_ADD_NB(ZRVector *vec, size_t nb, void *src);
static inline void ZRVECTOR_DELETE_ALL(ZRVector *vec);
static inline void ZRVECTOR_CHANGEOBJSIZE(ZRVector *vec, ZRObjInfos objInfos);

// ============================================================================
// ROUTINE FUNCTION

static inline void ZRVECTOR_FINSERT(ZRVector *vec, size_t pos, size_t nb)
{
	vec->strategy->finsert(vec, pos, nb);
}

static inline void ZRVECTOR_FDELETE(ZRVector *vec, size_t pos, size_t nb)
{
	vec->strategy->fdelete(vec, pos, nb);
}

// ============================================================================

#pragma zrlib generate function copy
ZRMUSTINLINE
static inline void ZRVECTOR_COPY(ZRVector *restrict dest, ZRVector *restrict src)
{
	assert(dest->array.objInfos.alignment == src->array.objInfos.alignment);
	ZRVECTOR_DELETE_ALL(dest);
	ZRVECTOR_CHANGEOBJSIZE(dest, ZRVECTOR_OBJINFOS(src));

	dest->array.objInfos.size = src->array.objInfos.size;
	dest->array.nbObj = 0;
	ZRVECTOR_ADD_NB(dest, src->array.nbObj, src->array.array);
}

#pragma zrlib generate function destroy
ZRMUSTINLINE
static inline void ZRVECTOR_DESTROY(ZRVector *vec)
{
	vec->strategy->fdestroy(vec);
}

#pragma zrlib generate function get
ZRMUSTINLINE
static inline void* ZRVECTOR_GET(ZRVector *vec, size_t pos)
{
	return ZRARRAYOP_GET(ZRVECTOR_ARRAYP(vec), ZRVECTOR_OBJSIZE(vec), pos);
}

#pragma zrlib generate function cpy
ZRMUSTINLINE
static inline void ZRVECTOR_CPY(ZRVector *vec, size_t pos, void *dest)
{
	ZRARRAYOP_CPY(dest, ZRVECTOR_OBJSIZE(vec), 1, ZRVECTOR_GET(vec, pos));
}

#pragma zrlib generate function cpy_nb
ZRMUSTINLINE
static inline void ZRVECTOR_CPY_NB(ZRVector *vec, size_t pos, size_t nb, void *dest)
{
	ZRARRAYOP_CPY(dest, ZRVECTOR_OBJSIZE(vec), nb, ZRVECTOR_GET(vec, pos));
}

#pragma zrlib generate function set
ZRMUSTINLINE
static inline void ZRVECTOR_SET(ZRVector *vec, size_t pos, void *obj)
{
	ZRARRAYOP_SET(ZRVECTOR_ARRAYP(vec), ZRVECTOR_OBJSIZE(vec), pos, obj);
}

#pragma zrlib generate function set_nb
ZRMUSTINLINE
static inline void ZRVECTOR_SET_NB(ZRVector *vec, size_t pos, size_t nb, void *src)
{
	ZRARRAYOP_DEPLACE(ZRVECTOR_GET(vec, pos), ZRVECTOR_OBJSIZE(vec), nb, src);
}

#pragma zrlib generate function insert
ZRMUSTINLINE
static inline void ZRVECTOR_INSERT(ZRVector *vec, size_t pos, void *obj)
{
	vec->strategy->finsert(vec, pos, 1);
	ZRVECTOR_SET(vec, pos, obj);
}

#pragma zrlib generate function insert_nb
ZRMUSTINLINE
static inline void ZRVECTOR_INSERT_NB(ZRVector *vec, size_t pos, size_t nb, void *src)
{
	vec->strategy->finsert(vec, pos, nb);
	ZRVECTOR_SET_NB(vec, pos, nb, src);
}

#pragma zrlib generate function changeObjSize
ZRMUSTINLINE
static inline void ZRVECTOR_CHANGEOBJSIZE(ZRVector *vec, ZRObjInfos objInfos)
{
	vec->strategy->fchangeObjSize(vec, objInfos);
}

#pragma zrlib generate function memoryTrim
ZRMUSTINLINE
static inline void ZRVECTOR_MEMORYTRIM(ZRVector *vec)
{
	vec->strategy->fmemoryTrim(vec);
}

#pragma zrlib generate function fill
ZRMUSTINLINE
static inline void ZRVECTOR_FILL(ZRVector *vec, size_t pos, size_t nb, void *obj)
{
	vec->strategy->finsert(vec, pos, nb);
	ZRARRAYOP_FILL(ZRVECTOR_GET(vec, pos), ZRVECTOR_OBJSIZE(vec), nb, obj);
}

#pragma zrlib generate function reserve
ZRMUSTINLINE
static inline void ZRVECTOR_RESERVE(ZRVector *vec, size_t pos)
{
	vec->strategy->finsert(vec, pos, 1);
}

#pragma zrlib generate function reserve_nb
ZRMUSTINLINE
static inline void ZRVECTOR_RESERVE_NB(ZRVector *vec, size_t pos, size_t nb)
{
	vec->strategy->finsert(vec, pos, nb);
}

#pragma zrlib generate function delete
ZRMUSTINLINE
static inline void ZRVECTOR_DELETE(ZRVector *vec, size_t pos)
{
	vec->strategy->fdelete(vec, pos, 1);
}

#pragma zrlib generate function delete_nb
ZRMUSTINLINE
static inline void ZRVECTOR_DELETE_NB(ZRVector *vec, size_t pos, size_t nb)
{
	vec->strategy->fdelete(vec, pos, nb);
}

#pragma zrlib generate function delete_all
ZRMUSTINLINE
static inline void ZRVECTOR_DELETE_ALL(ZRVector *vec)
{
	vec->strategy->fdelete(vec, 0, ZRVECTOR_NBOBJ(vec));
}

#pragma zrlib generate function add
ZRMUSTINLINE
static inline void ZRVECTOR_ADD(ZRVector *vec, void *obj)
{
	ZRVECTOR_INSERT(vec, ZRVECTOR_NBOBJ(vec), obj);
}

#pragma zrlib generate function add_nb
ZRMUSTINLINE
static inline void ZRVECTOR_ADD_NB(ZRVector *vec, size_t nb, void *src)
{
	ZRVECTOR_INSERT_NB(vec, ZRVECTOR_NBOBJ(vec), nb, src);
}

#pragma zrlib generate function addFirst
ZRMUSTINLINE
static inline void ZRVECTOR_ADDFIRST(ZRVector *vec, void *obj)
{
	ZRVECTOR_INSERT(vec, 0, obj);
}

#pragma zrlib generate function addFirst_nb
ZRMUSTINLINE
static inline void ZRVECTOR_ADDFIRST_NB(ZRVector *vec, size_t nb, void *src)
{
	ZRVECTOR_INSERT_NB(vec, 0, nb, src);
}

#pragma zrlib generate function dec
ZRMUSTINLINE
static inline void ZRVECTOR_DEC(ZRVector *vec)
{
	ZRVECTOR_DELETE(vec, ZRVECTOR_NBOBJ(vec) - 1);
}

#pragma zrlib generate function dec_nb
ZRMUSTINLINE
static inline void ZRVECTOR_DEC_NB(ZRVector *vec, size_t nb)
{
	ZRVECTOR_DELETE_NB(vec, ZRVECTOR_NBOBJ(vec) - nb, nb);
}

#pragma zrlib generate function decFirst
ZRMUSTINLINE
static inline void ZRVECTOR_DECFIRST(ZRVector *vec)
{
	ZRVECTOR_DELETE(vec, 0);
}

#pragma zrlib generate function decFirst_nb
ZRMUSTINLINE
static inline void ZRVECTOR_DECFIRST_NB(ZRVector *vec, size_t nb)
{
	ZRVECTOR_DELETE_NB(vec, 0, nb);
}

#pragma zrlib generate function pop
static inline void ZRVECTOR_POP(ZRVector *vec, void *dest)
{
	ZRARRAYOP_CPY(dest, ZRVECTOR_OBJSIZE(vec), 1, ZRVECTOR_GET(vec, ZRVECTOR_NBOBJ(vec) - 1));
	ZRVECTOR_DEC(vec);
}

#pragma zrlib generate function pop_nb
ZRMUSTINLINE
static inline void ZRVECTOR_POP_NB(ZRVector *vec, size_t nb, void *dest)
{
	ZRARRAYOP_CPY(dest, ZRVECTOR_OBJSIZE(vec), nb, ZRVECTOR_GET(vec, ZRVECTOR_NBOBJ(vec) - 1));
	ZRVECTOR_DEC_NB(vec, nb);
}

#pragma zrlib generate function popFirst
ZRMUSTINLINE
static inline void ZRVECTOR_POPFIRST(ZRVector *vec, void *dest)
{
	ZRARRAYOP_CPY(dest, ZRVECTOR_OBJSIZE(vec), 1, ZRVECTOR_GET(vec, 0));
	ZRVECTOR_DECFIRST(vec);
}

#pragma zrlib generate function popFirst_nb
ZRMUSTINLINE
static inline void ZRVECTOR_POPFIRST_NB(ZRVector *vec, size_t nb, void *dest)
{
	ZRARRAYOP_CPY(dest, ZRVECTOR_OBJSIZE(vec), nb, ZRVECTOR_GET(vec, 0));
	ZRVECTOR_DECFIRST_NB(vec, nb);
}

// Pointer help functions

#pragma zrlib generate function setPtr_nb
ZRMUSTINLINE
static inline void ZRVECTOR_SETPTR_NB(ZRVector *vec, size_t pos, size_t nb, void *src, size_t srcObjSize)
{
	assert(ZRVECTOR_OBJSIZE(vec) == sizeof(void*));

	for (; nb; nb--, pos++)
	{
		ZRVECTOR_SET(vec, pos, &src);
		src = (char*)src + srcObjSize;
	}
}

#pragma zrlib generate function insertPtr_nb
ZRMUSTINLINE
static inline void ZRVECTOR_INSERTPTR_NB(ZRVector *vec, size_t pos, size_t nb, void *src, size_t srcObjSize)
{
	vec->strategy->finsert(vec, pos, nb);
	ZRVECTOR_SETPTR_NB(vec, pos, nb, src, srcObjSize);
}

#pragma zrlib generate function addPtr_nb
ZRMUSTINLINE
static inline void ZRVECTOR_ADDPTR_NB(ZRVector *vec, size_t nb, void *src, size_t srcObjSize)
{
	ZRVECTOR_INSERTPTR_NB(vec, ZRVECTOR_NBOBJ(vec), nb, src, srcObjSize);
}

#pragma zrlib generate function addFirstPtr_nb
ZRMUSTINLINE
static inline void ZRVECTOR_ADDFIRSTPTR_NB(ZRVector *vec, size_t nb, void *src, size_t srcObjSize)
{
	ZRVECTOR_INSERTPTR_NB(vec, 0, nb, src, srcObjSize);
}

// ============================================================================

#pragma zrlib write generate headers

#endif
