/**
 * @author zuri
 * @date dim. 08 nov. 2020 14:23:38 CET
 */

#ifndef ZRVECTORTABLE_H
#define ZRVECTORTABLE_H

#include <zrlib/base/Table/Table.h>
#include <zrlib/base/Allocator/Allocator.h>

ZRObjInfos ZRTableIInfosObjInfos(void);

void ZRTableIInfos(void *iinfos, ZRObjAlignInfos *columnInfos, size_t nb);
void ZRTableIInfos_objAlignInfos(void *iinfos);
void ZRTableIInfos_allocator(void *iinfos, ZRAllocator *allocator);
void ZRTableIInfos_staticStrategy(void *iinfos);
void ZRTableIInfos_done(void *iinfos);

ZRObjInfos ZRTable_objInfos(void *iinfos);

void ZRTable_init(ZRTable *table, void *iinfos);
ZRTable* ZRTable_new(void *iinfos);

#endif
