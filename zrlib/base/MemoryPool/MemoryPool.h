/**
 * @author zuri
 * @date lundi 28 octobre 2019, 20:07:09 (UTC+0100)
 */

#ifndef ZRMEMORYPOOL_H
#define ZRMEMORYPOOL_H

#include <zrlib/config.h>
#include <zrlib/base/struct.h>

#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#pragma zrlib conf generate.target "/src/base/MemoryPool/MemoryPool.c" "."
#pragma zrlib conf generate.prefix ZRMPool_

// ============================================================================

typedef struct ZRMemoryPoolS ZRMemoryPool;
typedef struct ZRMemoryPoolStrategyS ZRMemoryPoolStrategy;

// ============================================================================

struct ZRMemoryPoolS
{
	ZRObjInfos blockInfos;
	size_t nbBlocks;

	/**
	 * The strategy for memory management and operations on the pool
	 */
	ZRMemoryPoolStrategy *strategy;
};

#pragma zrlib generate function ZRObjInfos blockInfos(ZRMemoryPool *)
#define ZRMPOOL_BLOCKINFOS(P) (P)->blockInfos

#pragma zrlib generate function size_t blockSize(ZRMemoryPool *)
#define ZRMPOOL_BLOCKSIZE(P) (P)->blockInfos.size

#pragma zrlib generate function size_t blockAlignment(ZRMemoryPool *)
#define ZRMPOOL_BLOCKALIGNMENT(P) (P)->blockInfos.alignment

#pragma zrlib generate function size_t nbBlocks(ZRMemoryPool *)
#define ZRMPOOL_NBBLOCKS(P) (P)->nbBlocks

struct ZRMemoryPoolStrategyS
{
	void (*finit)(ZRMemoryPool *pool);
	void (*fdestroy)(ZRMemoryPool *pool);
	void (*fclean)(ZRMemoryPool *pool);

	void* (*freserve)(ZRMemoryPool *pool, size_t nb);
	void* (*frelease)(ZRMemoryPool *pool, void *firstBlock, size_t nb);

	size_t (*fareaNbBlocks)(ZRMemoryPool *pool, void *firstBlock);

	/*
	 * Get a pointer to the begin of user MetaData.
	 * The pointer may not be aligned to the alignof(T), T the type attempted by the user.
	 */
	void* (*fareaMetaData)(ZRMemoryPool *pool, void *firstBlock);
};

// ============================================================================

#pragma zrlib generate function init
ZRMUSTINLINE
static inline void ZRMPOOL_INIT(ZRMemoryPool *pool, ZRObjInfos blockInfos, ZRMemoryPoolStrategy *strategy)
{
	*pool = (struct ZRMemoryPoolS)
	{
		.blockInfos = blockInfos,
		.strategy = strategy,
	};
	strategy->finit(pool);
}

#pragma zrlib generate function destroy
ZRMUSTINLINE
static inline void ZRMPOOL_DESTROY(ZRMemoryPool *pool)
{
	pool->strategy->fdestroy(pool);
}

#pragma zrlib generate function clean
ZRMUSTINLINE
static inline void ZRMPOOL_CLEAN(ZRMemoryPool *pool)
{
	pool->strategy->fclean(pool);
}

#pragma zrlib generate function areaNbBlocks
ZRMUSTINLINE
static inline size_t ZRMPOOL_AREANBBLOCKS(ZRMemoryPool *pool, void *firstBlock)
{
	return pool->strategy->fareaNbBlocks(pool, firstBlock);
}

#pragma zrlib generate function areaMetaData
ZRMUSTINLINE
static inline void* ZRMPOOL_AREAMETADATA(ZRMemoryPool *pool, void *firstBlock)
{
	return pool->strategy->fareaMetaData(pool, firstBlock);
}

#pragma zrlib generate function reserve
ZRMUSTINLINE
static inline void* ZRMPOOL_RESERVE(ZRMemoryPool *pool)
{
	return pool->strategy->freserve(pool, 1);
}

#pragma zrlib generate function reserve_nb
ZRMUSTINLINE
static inline void* ZRMPOOL_RESERVE_NB(ZRMemoryPool *pool, size_t nb)
{
	return pool->strategy->freserve(pool, nb);
}

#pragma zrlib generate function releaseArea
ZRMUSTINLINE
static inline void ZRMPOOL_RELEASEAREA(ZRMemoryPool *pool, void *block)
{
	pool->strategy->frelease(pool, block, SIZE_MAX);
}

#pragma zrlib generate function release_nb
ZRMUSTINLINE
static inline void* ZRMPOOL_RELEASE_NB(ZRMemoryPool *pool, void *firstBlock, size_t nb)
{
	return pool->strategy->frelease(pool, firstBlock, nb);
}

// ============================================================================

#pragma zrlib write generate headers

#endif
