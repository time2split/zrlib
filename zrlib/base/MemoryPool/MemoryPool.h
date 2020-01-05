/**
 * @author zuri
 * @date lundi 28 octobre 2019, 20:07:09 (UTC+0100)
 */

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <zrlib/config.h>
#include <zrlib/syntax_pad.h>

#include <stddef.h>
#include <string.h>

// ============================================================================

typedef struct ZRMemoryPoolS ZRMemoryPool;
typedef struct ZRMemoryPoolStrategyS ZRMemoryPoolStrategy;

// ============================================================================

struct ZRMemoryPoolS
{
	size_t blockSize;
	size_t nbBlocks;

	/**
	 * The strategy for memory management and operations on the pool
	 */
	ZRMemoryPoolStrategy *strategy;

	/*
	 * Data for Strategy purpose.
	 */
	char sdata[];
};

struct ZRMemoryPoolStrategyS
{
	size_t (*fsdataSize)(ZRMemoryPool *pool);
	size_t (*fstrategySize)(void);

	void (*finit)(ZRMemoryPool *pool);
	void (*fdone)(ZRMemoryPool *pool);

	void* (*freserve)(ZRMemoryPool *pool, size_t nb);
	void _(*frelease)(ZRMemoryPool *pool, void *firstBlock, size_t nb);

/**
 * Clean the memory used by the pool.
 * The pool MUST NOT be used after this call.
 */
};

// ============================================================================

static inline void ZRMPOOL_INIT(ZRMemoryPool *pool, size_t blockSize, ZRMemoryPoolStrategy *strategy)
{
	*pool = (ZRMemoryPool ) { //
		.blockSize = blockSize, //
		.strategy = strategy, //
		};

	strategy->finit(pool);
}

static inline void ZRMPOOL_DONE(ZRMemoryPool *pool)
{
	pool->strategy->fdone(pool);
}

static inline size_t ZRMPOOL_NBBLOCKS(ZRMemoryPool *pool)
{
	return pool->nbBlocks;
}

static inline size_t ZRMPOOL_BLOCKSIZE(ZRMemoryPool *pool)
{
	return pool->blockSize;
}

static inline void* ZRMPOOL_RESERVE(ZRMemoryPool *pool)
{
	return pool->strategy->freserve(pool, 1);
}

static inline void* ZRMPOOL_RESERVE_NB(ZRMemoryPool *pool, size_t nb)
{
	return pool->strategy->freserve(pool, nb);
}

static inline void ZRMPOOL_RELEASE(ZRMemoryPool *pool, void *block)
{
	pool->strategy->frelease(pool, block, 1);
}

static inline void ZRMPOOL_RELEASE_NB(ZRMemoryPool *pool, void *firstBlock, size_t nb)
{
	pool->strategy->frelease(pool, firstBlock, nb);
}

// ============================================================================

void ZRMPool_init(ZRMemoryPool *pool, size_t objSize, ZRMemoryPoolStrategy *strategy);
void ZRMPool_done(ZRMemoryPool *pool);

size_t ZRMPool_nbBlocks(_ ZRMemoryPool *pool);
size_t ZRMPool_blockSize(ZRMemoryPool *pool);

void* ZRMPool_reserve(__ ZRMemoryPool *pool);
void* ZRMPool_reserve_nb(ZRMemoryPool *pool, size_t nb);

void ZRMPool_release(__ ZRMemoryPool *pool, void *block);
void ZRMPool_release_nb(ZRMemoryPool *pool, void *firstBlock, size_t nb);

#endif