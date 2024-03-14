#include <zrlib/base/Allocator/Allocator.h>

#pragma zrlib write generate functions
#pragma zrlib begin

void* ZRAlloc(ZRAllocator *allocator, size_t nbBytes)
{
	return ZRALLOC(allocator, nbBytes);
}

void* ZRAAlloc(ZRAllocator *allocator, size_t alignment, size_t nbBytes)
{
	return ZRAALLOC(allocator, alignment, nbBytes);
}

void* ZRObjAlloc_nb(ZRAllocator *allocator, ZRObjInfos objInfos, size_t nb)
{
	return ZROBJALLOC_NB(allocator, objInfos, nb);
}

void* ZRObjAlloc(ZRAllocator *allocator, ZRObjInfos objInfos)
{
	return ZROBJALLOC(allocator, objInfos);
}

void* ZRRealloc(ZRAllocator *allocator, void *allocated, size_t nbBytes)
{
	return ZRREALLOC(allocator, allocated, nbBytes);
}

void ZRFree(ZRAllocator *allocator, void *allocated)
{
	ZRFREE(allocator, allocated);
}

void ZRAlloc_objects(ZRAllocator *allocator, ZRObjectP *objects, size_t nb)
{
	ZRALLOC_OBJECTS(allocator, objects, nb);
}

#pragma zrlib end




