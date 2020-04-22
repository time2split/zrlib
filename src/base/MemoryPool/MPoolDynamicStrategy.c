/**
 * @author zuri
 * @date mardi 29 octobre 2019, 19:39:20 (UTC+0100)
 */

#include <zrlib/base/Allocator/Allocator.h>
#include <zrlib/base/Bits/Bits.h>
#include <zrlib/base/MemoryPool/MPoolDynamicStrategy.h>
#include <zrlib/base/Vector/Vector2SideStrategy.h>
#include <zrlib/base/ArrayOp.h>
#include <zrlib/base/ReserveOp_bits.h>

#include <stddef.h>
#include <stdio.h>

// ============================================================================

typedef struct ZRMPoolDynamicStrategyS ZRMPoolDynamicStrategy;
typedef struct ZRMPoolDSS ZRMPoolDS;
typedef struct ZRMPoolDS_bucketS ZRMPoolDS_bucket;

// ============================================================================

struct ZRMPoolDS_bucketS
{
	size_t nbBlocks;
	size_t nbAvailables;
	size_t nbZRBits;

	/**
	 * A sequence of bit which each inform of the availability of the block.
	 * 1 = empty, 0 = locked
	 */
	ZRBits *bits;

	/**
	 * The memory blocks and the bits.
	 *
	 * The first nbBlocks*pool->blockSize blocks are for the blocks, and the rest is the bits area.
	 */
	char blocks[];
};

// ============================================================================

struct ZRMPoolDynamicStrategyS
{
	ZRMemoryPoolStrategy strategy;

	ZRAllocator *allocator;

	size_t initialBucketSize;

	size_t maxFreeBuckets;

	/**
	 * (Optional)
	 */
	ZRVector* (*fcreateBuckets)(ZRMemoryPool*);

	/**
	 * (Optional)
	 */
	void (*fdestroyBuckets)(ZRVector *buckets);
};

// ============================================================================


struct ZRMPoolDSS
{
	ZRMemoryPool pool;

	size_t nbAvailables;

	size_t nbFreeBuckets;

	ZRVector *buckets;
};

// ============================================================================

#define ZRMPOOLDS(pool) ((ZRMPoolDS*)(pool))
#define ZRMPOOL_STRATEGY(pool) ((ZRMPoolDynamicStrategy*)((pool)->strategy))

#define DEFAULT_MAX_FREE_BUCKETS 10
#define INITIAL_BUCKETS_SPACE 1024
#define INITIAL_BUCKET_SIZE   1024
#define INITIAL_BITS_SPACE ((1024*64)/ZRBITS_NBOF)

// ============================================================================
// Internal functions

/**
 * Add a bucket enough large to store $nbBlocks blocks.
 */
inline static ZRMPoolDS_bucket* addBucket(ZRMemoryPool *pool, size_t nbBlocks)
{
	ZRMPoolDS *data = ZRMPOOLDS(pool);
	ZRMPoolDynamicStrategy *strategy = ZRMPOOL_STRATEGY(pool);
	size_t const initialBucketSize = strategy->initialBucketSize;
	size_t nbBlocksToAlloc = initialBucketSize;

	while (nbBlocks > nbBlocksToAlloc)
		nbBlocksToAlloc += initialBucketSize;

	size_t const blockSpace = nbBlocksToAlloc * pool->blockSize;
	size_t const nbZRBits = nbBlocksToAlloc / ZRBITS_NBOF + ((nbBlocksToAlloc % ZRBITS_NBOF) ? 1 : 0);
	size_t const alignment = sizeof(ZRBits);
	size_t const bitSpace = nbZRBits * sizeof(ZRBits) + alignment - 1;
	ZRMPoolDS_bucket *bucket = ZRALLOC(strategy->allocator, sizeof(ZRMPoolDS_bucket) + blockSpace + bitSpace);
	ZRVECTOR_ADD(data->buckets, &bucket);

	{
		char *bits = &bucket->blocks[blockSpace];
		size_t rest = (size_t)bits % alignment;

		if (rest > 0)
			bucket->bits = (ZRBits*)(bits + alignment - rest);
		else
			bucket->bits = (ZRBits*)(bits);
	}
	bucket->nbZRBits = nbZRBits;
	bucket->nbBlocks = nbBlocksToAlloc;
	bucket->nbAvailables = nbBlocksToAlloc;
	pool->nbBlocks += nbBlocksToAlloc;
	data->nbAvailables += nbBlocksToAlloc;
	data->nbFreeBuckets++;

	ZRARRAYOP_FILL(bucket->bits, sizeof(ZRBits), nbZRBits, &((ZRBits ) { ZRRESERVEOPBITS_FULLEMPTY } ));
	return bucket;
}

// ============================================================================
// MemoryPool functions

size_t fstrategySize(void)
{
	return sizeof(ZRMPoolDynamicStrategy);
}

void finitPool(ZRMemoryPool *pool)
{
	ZRMPoolDS *const data = ZRMPOOLDS(pool);
	data->nbFreeBuckets = 0;
	data->nbAvailables = 0;
	data->buckets = ZRMPOOL_STRATEGY(pool)->fcreateBuckets(pool);
	addBucket(pool, 0);
}

static inline void* freserveInBucket(ZRMemoryPool *pool, size_t nb, ZRMPoolDS_bucket *bucket)
{
	if (nb > bucket->nbAvailables)
		return NULL ;

	ZRMPoolDS *const data = ZRMPOOLDS(pool);
	ZRBits *bits;
	size_t pos  = ZRRESERVEOPBITS_RESERVEFIRSTAVAILABLES(bucket->bits, bucket->nbZRBits, nb);

	if (bucket->nbAvailables == bucket->nbBlocks)
		data->nbFreeBuckets--;

	data->nbAvailables -= nb;
	bucket->nbAvailables -= nb;
	return ZRARRAYOP_GET(bucket->blocks, pool->blockSize, pos);
}

void* freserve(ZRMemoryPool *pool, size_t nb)
{
	ZRMPoolDS *const data = ZRMPOOLDS(pool);
	size_t const nbBuckets = data->buckets->nbObj;

	for (int i = 0; i < nbBuckets; i++)
	{
		ZRMPoolDS_bucket *bucket = *(ZRMPoolDS_bucket**)ZRVECTOR_GET(data->buckets, i);
		void *ret = freserveInBucket(pool, nb, bucket);

		if (ret == NULL)
			continue;

		return ret;
	}
	return freserveInBucket(pool, nb, addBucket(pool, nb));
}

static inline bool freleaseInBucket(ZRMemoryPool *pool, ZRMPoolDS_bucket *bucket, size_t bucket_i, void *firstBlock, size_t nb)
{
	ZRMPoolDS *const data = ZRMPOOLDS(pool);

	// TODO : change this check of pointer bound. Not conform to standard.
	if (bucket->blocks > (char*)firstBlock || (char*)firstBlock >= &bucket->blocks[pool->blockSize * bucket->nbBlocks])
		return false;

	data->nbAvailables += nb;
	bucket->nbAvailables += nb;

	if (bucket->nbAvailables == bucket->nbBlocks)
	{
		// Clean the bucket
		if (data->nbFreeBuckets == ZRMPOOL_STRATEGY(pool)->maxFreeBuckets)
		{
			data->nbAvailables -= bucket->nbBlocks;
			pool->nbBlocks -= bucket->nbBlocks;
			ZRFREE(ZRMPOOL_STRATEGY(pool)->allocator, bucket);
			ZRVECTOR_DELETE(data->buckets, bucket_i);
			return true;
		}
		data->nbFreeBuckets++;
	}
	size_t const blockPos = ((char*)firstBlock - bucket->blocks) / pool->blockSize;
	ZRRESERVEOPBITS_RELEASENB(bucket->bits, blockPos, nb);
	return true;
}

void frelease(ZRMemoryPool *pool, void *firstBlock, size_t nb)
{
	ZRMPoolDS *const data = ZRMPOOLDS(pool);
	size_t const nb_i = data->buckets->nbObj;

	for (size_t i = 0; i < nb_i; i++)
	{
		if (freleaseInBucket(pool, *(ZRMPoolDS_bucket**)ZRVECTOR_GET(data->buckets, i), i, firstBlock, nb))
			return;
	}
	fprintf(stderr, "The block %p does not belong to the pool %p", firstBlock, pool);
	exit(1);
}

/**
 * Clean the memory used by the pool.
 * The pool MUST NOT be used after this call.
 */
void fdone(ZRMemoryPool *pool)
{
	ZRMPoolDS *const data = ZRMPOOLDS(pool);
	ZRAllocator *const allocator = ZRMPOOL_STRATEGY(pool)->allocator;
	ZRVector *const buckets = data->buckets;
	size_t const nbBuckets = ZRVECTOR_NBOBJ(buckets);

	for (size_t i = 0; i < nbBuckets; i++)
	{
		ZRMPoolDS_bucket *const bucket = *(ZRMPoolDS_bucket**)ZRVECTOR_GET(buckets, i);
		ZRFREE(allocator, bucket);
	}
	ZRMPOOL_STRATEGY(pool)->fdestroyBuckets(buckets);
}

// ============================================================================

static ZRVector* fcreateBuckets(ZRMemoryPool *pool)
{
	return ZRVector2SideStrategy_createDynamic(INITIAL_BUCKETS_SPACE, sizeof(ZRMPoolDS_bucket*), ZRMPOOL_STRATEGY(pool)->allocator);
}

static void fdestroyBuckets(ZRVector *buckets)
{
	ZRVector2SideStrategy_destroy(buckets);
}

void ZRMPoolDS_init(ZRMemoryPoolStrategy *strategy, ZRAllocator *allocator, size_t initialBucketSize, size_t maxFreeBuckets)
{
	*(ZRMPoolDynamicStrategy*)strategy = (ZRMPoolDynamicStrategy ) { //
		.strategy = (ZRMemoryPoolStrategy ) { //
			.fstrategySize = fstrategySize, //
			.finit = finitPool, //
			.fdone = fdone, //
			.freserve = freserve, //
			.frelease = frelease, //
			},//
		.allocator = allocator, //
		.initialBucketSize = initialBucketSize, //
		.maxFreeBuckets = maxFreeBuckets, //
		.fcreateBuckets = fcreateBuckets, //
		.fdestroyBuckets = fdestroyBuckets, //
		};
}

// ============================================================================

#include "MPoolDynamicStrategy_help.c"
