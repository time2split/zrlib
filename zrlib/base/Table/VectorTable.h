/**
 * @author zuri
 * @date dim. 08 nov. 2020 14:23:38 CET
 */

#ifndef ZRVECTORTABLE_H
#define ZRVECTORTABLE_H

#include <zrlib/base/Table/Table.h>
#include <zrlib/base/Allocator/Allocator.h>

enum ZRVectorTableModeE
{
	ZRVectorTableModeE_object = 1,
	ZRVectorTableModeE_column = 2,
	ZRVectorTableModeE_both   = 3,
};
#define ZRVectorTableModeE_default ZRVectorTableModeE_object
#define ZRVECTORTABLEMODEE_NBBITS 2

ZRObjInfos ZRVectorTableIInfosObjInfos(void);

void ZRVectorTableIInfos(void *iinfos, ZRObjAlignInfos *columnInfos, size_t nb);
void ZRVectorTableIInfos_mode(void *iinfos, enum ZRVectorTableModeE mode);
void ZRVectorTableIInfos_allocator(void *iinfos, ZRAllocator *allocator);
void ZRVectorTableIInfos_staticStrategy(void *iinfos);
void ZRVectorTableIInfos_done(void *iinfos);

ZRObjInfos ZRVectorTable_objInfos(void *iinfos);

void ZRVectorTable_init(ZRTable *table, void *iinfos);
ZRTable* ZRVectorTable_new(void *iinfos);

#endif
