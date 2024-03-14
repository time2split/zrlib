/**
 * @author zuri
 * @date samedi 19 janvier 2019, 20:35:53 (UTC+0100)
 */

#ifndef ZRCONTEXT_H
#define ZRCONTEXT_H

#include <zrlib/base/Allocator/Allocator.h>
#include <zrlib/base/Identifier/Identifier.h>

typedef struct ZRContextS ZRContext;

struct ZRContextS{
//	Context *parent;
	ZRAllocator *allocator;
};

size_t ZRContext_initForThread(void);
void ZRContext_destroyForThread(void);

ZRContext* ZRContext_current(void);

ZRIdentifier* ZRContext_getStrategyIdentifier(ZRContext *context, size_t strategySize);

#endif
