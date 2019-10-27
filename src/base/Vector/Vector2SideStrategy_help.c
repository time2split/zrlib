/**
 * @author zuri
 * @date samedi 26 octobre 2019, 18:24:01 (UTC+0200)
 */

ZRVector* ZRVector2SideStrategy_alloc(size_t initialSpace, size_t objSize, ZRAllocator *allocator)
{
	size_t const vecSize = sizeof(ZRVector) + sizeof(ZRVector_2SideData) + (initialSpace * objSize);
	ZRVector *ret = ZRALLOC(allocator, vecSize);
	return ret;
}

ZRVector* ZRVector2SideStrategy_createFixed(size_t initialSpace, size_t objSize, ZRAllocator *allocator)
{
	ZRVectorStrategy *strategy = ZRALLOC(allocator, sizeof(ZRVector_2SideStrategy));
	ZRVector_2SideStrategy_init(strategy, allocator, initialSpace, 0);
	ZRVector *vec = ZRVector2SideStrategy_alloc(initialSpace, objSize, allocator);
	ZRVector_init(vec, objSize, strategy);
	return vec;
}

ZRVector* ZRVector2SideStrategy_createDynamic(size_t initialSpace, size_t objSize, ZRAllocator *allocator)
{
	ZRVector *vec = ZRVector2SideStrategy_createFixed(initialSpace, objSize, allocator);
	ZRVector_2SideStrategy_dynamicMemory(vec->strategy);
	return vec;
}

ZRVector* ZRVector2SideStrategy_createFixedM(size_t initialArraySpace, size_t initialMemorySpace, size_t objSize, ZRAllocator *allocator)
{
	ZRVectorStrategy *strategy = ZRALLOC(allocator, sizeof(ZRVector_2SideStrategy));
	ZRVector_2SideStrategy_init(strategy, allocator, initialArraySpace, initialMemorySpace);
	ZRVector *vec = ZRVector2SideStrategy_alloc(initialArraySpace, objSize, allocator);
	ZRVector_init(vec, objSize, strategy);
	return vec;
}

ZRVector* ZRVector2SideStrategy_createDynamicM(size_t initialArraySpace, size_t initialMemorySpace, size_t objSize, ZRAllocator *allocator)
{
	ZRVector *vec = ZRVector2SideStrategy_createFixedM(initialArraySpace, initialMemorySpace, objSize, allocator);
	ZRVector_2SideStrategy_dynamicMemory(vec->strategy);
	return vec;
}
