/**
 * @author zuri
 * @date mer. 29 juil. 2020 13:26:18 CEST
 */

#include <zrlib/base/Context.h>
#include <zrlib/base/Allocator/CAllocator.h>

#include <stdlib.h>
#include <threads.h>

// ============================================================================

thread_local ZRContext CONTEXT;

// ============================================================================

size_t ZRContext_initForThread(void)
{
	ZRAllocator *allocator = malloc(sizeof(ZRAllocator));
	ZRCAllocator_init(allocator);

	CONTEXT = (ZRContext ) { //
		.allocator = allocator,
		};
}

void ZRContext_destroyForThread(void)
{
	free(CONTEXT.allocator);
}

ZRContext* ZRContext_current(void)
{
	return &CONTEXT;
}

ZRIdentifier* ZRContext_getStrategyIdentifier(ZRContext *context, size_t strategySize)
{

}
