#include <zrlib/base/Table/VectorTable.h>
#include <zrlib/base/Vector/Vector2SideStrategy.h>
#include <zrlib/lib/init.h>

typedef struct
{
	ZRTableStrategy strategy;
} Strategy;

#define VTS(S) ((Strategy*)(S))
#define VTS_T(S) VTS(S)->strategy

typedef struct
{
	ZRTable table;
	ZRAllocator *allocator;

	unsigned staticStrategy :1;
} VectorTable;

#define VT(T) ((VectorTable*)(T))
#define VT_T(T) (&(VT(T)->table))
#define VT_S(T) ((Strategy*)VT_T(T)->strategy)

typedef enum
{
	AlignInfos_base,
	AlignInfos_columns,
	AlignInfos_strategy,
	AlignInfos_struct,
	ALIGNINFOS_NB,
} AlignInfos;

/* ========================================================================= */

static void fdone(ZRTable *table)
{
	VectorTable *vtable = VT(table);
}

static void fdestroy(ZRTable *table)
{
	VectorTable *vtable = VT(table);
	fdone(VT_T(vtable));

	if (!vtable->staticStrategy)
		ZRFREE(vtable->allocator, VT_S(vtable));

	ZRFREE(vtable->allocator, vtable);
}

/* ========================================================================= */

typedef struct
{
	ZRObjAlignInfos ainfos[ALIGNINFOS_NB];
	ZRAllocator *allocator;

	ZRObjAlignInfos *columnInfos;
	size_t nbColumns;

	unsigned staticStrategy :1;
	unsigned changefdestroy :1;
} InitInfos;

#define II(I) ((InitInfos*)(I))

static void makeAlignInfos(ZRObjAlignInfos *ainfos, InitInfos *initInfos)
{
	ainfos[AlignInfos_base] = ZRTYPE_OBJALIGNINFOS(VectorTable);
	ainfos[AlignInfos_columns] = ZRTYPENB_OBJALIGNINFOS(ZRObjAlignInfos, initInfos->nbColumns + 1);
	ainfos[AlignInfos_strategy] = initInfos->staticStrategy ? ZRTYPE_OBJALIGNINFOS(Strategy) : ZROBJALIGNINFOS_DEF0();
	ainfos[AlignInfos_struct] = ZROBJALIGNINFOS_DEF0();
	ZRStruct_bestOffsetsPos(ALIGNINFOS_NB - 1, ainfos, 1);
}

void initInfos_validate(InitInfos *initInfos)
{
	makeAlignInfos(initInfos->ainfos, initInfos);
}

ZRObjInfos ZRTableIInfosObjInfos(void)
{
	return ZRTYPE_OBJINFOS(InitInfos);
}

void ZRTableIInfos(void *iinfos, ZRObjAlignInfos *columnInfos, size_t nb)
{
	InitInfos *const initInfos = II(iinfos);

	*initInfos = (InitInfos ) { //
		.columnInfos = columnInfos,
		.nbColumns = nb,
		};
}

void ZRTableIInfos_done(void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
}

void ZRTableIInfos_allocator(void *iinfos, ZRAllocator *allocator)
{
	InitInfos *const initInfos = II(iinfos);
	initInfos->allocator = allocator;
}

void ZRTableIInfos_staticStrategy(void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
	initInfos->staticStrategy = 1;
	initInfos_validate(initInfos);
}

ZRObjInfos ZRTable_objInfos(void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
	return ZRTYPE_OBJINFOS(initInfos->ainfos[AlignInfos_struct]);
}

static void Strategy_init(Strategy *strategy, InitInfos *initInfos)
{
	*strategy = (Strategy ) { //
		.strategy = (ZRTableStrategy ) { //
			.fdestroy = initInfos->changefdestroy ? fdestroy : fdone,
			} ,
		};
}

void ZRTable_init(ZRTable *table, void *iinfos)
{
	VectorTable *vtable = VT(table);
	InitInfos *const initInfos = II(iinfos);
	ZRTableStrategy *strategy;
	Strategy ref;
	ZRObjAlignInfos *ainfos;

	Strategy_init(&ref, initInfos);

	if (initInfos->staticStrategy)
	{
		strategy = ZRARRAYOP_GET(vtable, 1, initInfos->ainfos[AlignInfos_strategy].offset);
		ZRPTYPE_CPY(strategy, &ref);
	}
	else
		strategy = zrlib_internPType(&ref);

	ainfos = ZRARRAYOP_GET(vtable, 1, initInfos->ainfos[AlignInfos_columns].offset);
	ZRPTYPE_CPY_NB(ainfos, initInfos->columnInfos, initInfos->nbColumns + 1);

	*vtable = (VectorTable ) { //
		.table = (ZRTable ) { //
			.strategy = strategy,
			.ainfos = ainfos,
			.nbColumns = initInfos->nbColumns,
			} ,
		};
}

ZRTable* ZRTable_new(void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
	ZRTable *table = ZROBJALLOC(initInfos->allocator, ZROBJALIGNINFOS_CPYOBJINFOS(initInfos->ainfos[ALIGNINFOS_NB - 1]));

	initInfos->changefdestroy = 1;
	ZRTable_init(table, iinfos);
	initInfos->changefdestroy = 0;
	return table;
}
