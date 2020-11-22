/**
 * @author zuri
 * @date mardi 19 novembre 2019, 19:40:53 (UTC+0100)
 */

#ifndef ZRMAP_H
#define ZRMAP_H

#include <zrlib/config.h>
#include <zrlib/base/struct.h>

#include <stdalign.h>
#include <stddef.h>
#include <stdbool.h>

#pragma zrlib conf generate.target "/src/base/Map/Map.c" "."
#pragma zrlib conf generate.prefix ZRMap_

/* ========================================================================= */

typedef struct ZRMapS ZRMap;
typedef struct ZRMapStrategyS ZRMapStrategy;

typedef struct ZRMapKeyValS
{
	void *key;
	void *val;
} ZRMapKeyVal;

/* ========================================================================= */

struct ZRMapStrategyS
{
	/*
	 * The insert/delete functions are responsible to update properly the map.nbObj value.
	 *
	 * @parameter
	 * **out a pointer to the added object, may be NULL
	 */
	void (*fput)(_______ ZRMap *map, void *key, void *value, void **out);
	bool (*fputIfAbsent)(ZRMap *map, void *key, void *value, void **out);
	bool (*freplace)(___ ZRMap *map, void *key, void *value, void **out);

	void* (*fget)(ZRMap *map, void *key);

	bool (*fdelete)(ZRMap *map, void *key, void *cpy_out);
	void (*fdeleteAll)(ZRMap *map);

	size_t (*fcpyKeyValPtr)(ZRMap *map, ZRMapKeyVal *cpyTo, size_t offset, size_t maxNbCpy);

	/**
	 * (optional)
	 */
	void (*fdestroy)(ZRMap *map);
};

struct ZRMapS
{
	ZRObjInfos keyInfos;
	ZRObjInfos objInfos;
	size_t nbObj;

	/*
	 * The strategy for memory management and insertion/deletion routines.
	 */
	ZRMapStrategy *strategy;
};

#define ZRMAP(M) ((ZRMap*)(M))

/* ========================================================================= */

#pragma zrlib generate function destroy
ZRMUSTINLINE
static inline void ZRMAP_DESTROY(ZRMap *map)
{
	map->strategy->fdestroy(map);
}

#pragma zrlib generate function cpyKeyValPtr
ZRMUSTINLINE
static inline size_t ZRMAP_CPYKEYVALPTR(ZRMap *map, ZRMapKeyVal *cpyTo, size_t offset, size_t maxNbCpy)
{
	return map->strategy->fcpyKeyValPtr(map, cpyTo, offset, maxNbCpy);
}

#pragma zrlib generate function get
ZRMUSTINLINE
static inline void* ZRMAP_GET(ZRMap *map, void *key)
{
	return map->strategy->fget(map, key);
}

#pragma zrlib generate function put
ZRMUSTINLINE
static inline void ZRMAP_PUT(ZRMap *map, void *key, void *value)
{
	map->strategy->fput(map, key, value, NULL);
}

#pragma zrlib generate function putIfAbsent
ZRMUSTINLINE
static inline bool ZRMAP_PUTIFABSENT(ZRMap *map, void *key, void *value)
{
	return map->strategy->fputIfAbsent(map, key, value, NULL);
}

#pragma zrlib generate function replace
ZRMUSTINLINE
static inline bool ZRMAP_REPLACE(ZRMap *map, void *key, void *value)
{
	return map->strategy->freplace(map, key, value, NULL);
}

#pragma zrlib generate function putThenGet
ZRMUSTINLINE
static inline void ZRMAP_PUTTHENGET(ZRMap *map, void *key, void *value, void **out)
{
	map->strategy->fput(map, key, value, out);
}

#pragma zrlib generate function putIfAbsentThenGet
ZRMUSTINLINE
static inline bool ZRMAP_PUTIFABSENTTHENGET(ZRMap *map, void *key, void *value, void **out)
{
	return map->strategy->fputIfAbsent(map, key, value, out);
}

#pragma zrlib generate function replaceThenGet
ZRMUSTINLINE
static inline bool ZRMAP_REPLACETHENGET(ZRMap *map, void *key, void *value, void **out)
{
	return map->strategy->freplace(map, key, value, out);
}

#pragma zrlib generate function cpyThenDelete
ZRMUSTINLINE
static inline bool ZRMAP_CPYTHENDELETE(ZRMap *map, void *key, void *cpy_out)
{
	return map->strategy->fdelete(map, key, cpy_out);
}

#pragma zrlib generate function delete
ZRMUSTINLINE
static inline bool ZRMAP_DELETE(ZRMap *map, void *key)
{
	return map->strategy->fdelete(map, key, NULL);
}

#pragma zrlib generate function deleteAll
ZRMUSTINLINE
static inline void ZRMAP_DELETEALL(ZRMap *map)
{
	map->strategy->fdeleteAll(map);
}

/* Help */

#pragma zrlib generate function nbObj
ZRMUSTINLINE
static inline size_t ZRMAP_NBOBJ(ZRMap *map)
{
	return map->nbObj;
}

#pragma zrlib generate function keySize
ZRMUSTINLINE
static inline size_t ZRMAP_KEYSIZE(ZRMap *map)
{
	return map->keyInfos.size;
}

#pragma zrlib generate function objSize
ZRMUSTINLINE
static inline size_t ZRMAP_OBJSIZE(ZRMap *map)
{
	return map->objInfos.size;
}

/* ========================================================================= */
/* HELP */

#pragma zrlib generate function getKeyValPtr
ZRMUSTINLINE
static inline ZRMapKeyVal ZRMAP_GETKEYVALPTR(ZRMap *map, size_t offset)
{
	ZRMapKeyVal ret;
	ZRMAP_CPYKEYVALPTR(map, &ret, offset, 1);
	return ret;
}

/* ========================================================================= */

#pragma zrlib write generate headers

#endif
