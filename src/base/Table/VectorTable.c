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
	ZRVector *objects;
	ZRVector **columns;
} VectorTable;

#define VT(T) ((VectorTable*)(T))
#define VT_T(T) (&(VT(T)->table))
#define VT_S(T) ((Strategy*)VT_T(T)->strategy)

typedef enum
{
	AlignInfos_base,
	AlignInfos_columnInfos,
	AlignInfos_columnsP,
	AlignInfos_columns,
	AlignInfos_objects,
	AlignInfos_strategy,
	AlignInfos_struct,
	ALIGNINFOS_NB,
} AlignInfos;

/* ========================================================================= */

ZRMUSTINLINE
static inline void* get_object(ZRTable *table, size_t line)
{
	VectorTable *vtable = VT(table);
	return ZRVECTOR_GET(vtable->objects, line);
}

ZRMUSTINLINE
static inline void* get_column_object(ZRTable *table, size_t line, size_t column)
{
	VectorTable *vtable = VT(table);
	return ZRARRAYOP_GET(
		ZRVECTOR_GET(vtable->objects, line),
		ZRTABLE_OBJAINFOS(table, column).offset
		);
}

static void* fget_column_object(ZRTable *table, size_t line, size_t column)
{
	return get_column_object(table, line, column);
}

static void fcpy_column_object(ZRTable *table, size_t line, size_t column, size_t nb, void *dest)
{
	VectorTable *vtable = VT(table);
	void *col = get_column_object(table, line, column);
	size_t objSize = ZRTABLE_OBJINFOS(table).size;
	size_t colSize = ZRTABLE_OBJINFOS(table,column).size;

	while (nb--)
	{
		memcpy(dest, col, colSize);
		dest = ZRARRAYOP_GET(dest, colSize);
		col = ZRARRAYOP_GET(col, objSize);
	}
}

static void fset_column_object(ZRTable *table, size_t line, size_t column, size_t nb, void *src)
{
	VectorTable *vtable = VT(table);
	size_t objSize = ZRTABLE_OBJINFOS(table).size;
	size_t colSize = ZRTABLE_OBJINFOS(table, column).size;
	void *col = get_column_object(table, line, column);

	while (nb--)
	{
		memcpy(col, src, colSize);
		col = ZRARRAYOP_GET(col, objSize);
		src = ZRARRAYOP_GET(src, colSize);
	}
}

static void fcpy_object(ZRTable *table, size_t line, size_t nb, void *dest)
{
	VectorTable *vtable = VT(table);
	size_t objSize = ZRTABLE_OBJINFOS(table).size;
	void *obj = get_object(table, line);
	memcpy(dest, obj, objSize * nb);
}

static void fset_object(ZRTable *table, size_t line, size_t nb, void *src)
{
	VectorTable *vtable = VT(table);
	size_t objSize = ZRTABLE_OBJINFOS(table).size;
	void *obj = get_object(table, line);
	memcpy(obj, src, objSize * nb);
}

ZRMUSTINLINE
static inline void reserve_object(ZRTable *table, size_t line, size_t nb)
{
	VectorTable *vtable = VT(table);
	ZRVECTOR_RESERVE_NB(vtable->objects, line, nb);
}

static void freserve_object(ZRTable *table, size_t line, size_t nb)
{
	reserve_object(table, line, nb);
	table->nbLines += nb;
}

ZRMUSTINLINE
static inline void delete_object(ZRTable *table, size_t line, size_t nb)
{
	VectorTable *vtable = VT(table);
	ZRVECTOR_DELETE_NB(vtable->objects, line, nb);
}

static void fdelete_object(ZRTable *table, size_t line, size_t nb)
{
	delete_object(table, line, nb);
	table->nbLines -= nb;
}

static inline void* fgetObjects_object(ZRTable *table, size_t line, size_t nb)
{
	VectorTable *vtable = VT(table);
	return ZRVECTOR_GET(vtable->objects, line);
}

static void fgetObjects_end_object(ZRTable *table, void *objects)
{
}

static inline void* fgetColumn_object(ZRTable *table, size_t line, size_t column, size_t nb)
{
	VectorTable *vtable = VT(table);
	size_t nbLines = ZRTABLE_NBLINES(table);
	void *col = ZROBJALLOC_NB(vtable->allocator, ZRTABLE_OBJINFOS(table, column), nb);
	fcpy_column_object(table, line, column, nb, col);
	return col;
}

static void fgetColumn_end_object(ZRTable *table, void *column)
{
	VectorTable *vtable = VT(table);
	ZRFREE(vtable->allocator, column);
}

static void fdone_object(ZRTable *table)
{
	VectorTable *vtable = VT(table);
	ZRVector_destroy(vtable->objects);
}

static void fdestroy_object(ZRTable *table)
{
	VectorTable *vtable = VT(table);
	fdone_object(VT_T(vtable));
	ZRFREE(vtable->allocator, vtable);
}

/* ========================================================================= */

static void* fget_column_column(ZRTable *table, size_t line, size_t column)
{
	VectorTable *vtable = VT(table);
	return ZRVECTOR_GET(vtable->columns[column], line);
}

static void fcpy_column_column(ZRTable *table, size_t line, size_t column, size_t nb, void *dest)
{
	VectorTable *vtable = VT(table);
	memcpy(dest, ZRVECTOR_GET(vtable->columns[column], line), nb * table->ainfos[column].size);
}

static void fset_column_column(ZRTable *table, size_t line, size_t column, size_t nb, void *src)
{
	VectorTable *vtable = VT(table);
	ZRVECTOR_INSERT_NB(vtable->columns[column], line, nb, src);
}

static void fcpy_column(ZRTable *table, size_t line, size_t nb, void *dest)
{
	VectorTable *vtable = VT(table);
	size_t nbCols = table->nbColumns;
	size_t objSize = ZRTABLE_OBJINFOS(table).size;
	ZRObjAlignInfos infos;

	while (nb--)
	{
		for (size_t i = 0; i < nbCols; i++)
		{
			infos = table->ainfos[i];
			memcpy(ZRARRAYOP_GET(dest, infos.offset), ZRVECTOR_GET(vtable->columns[i], line), infos.size);
		}
		line++;
		dest = ZRARRAYOP_GET(dest, objSize);
	}
}

static void fset_column(ZRTable *table, size_t line, size_t nb, void *src)
{
	VectorTable *vtable = VT(table);
	size_t nbCols = table->nbColumns;
	size_t objSize = ZRTABLE_OBJINFOS(table).size;
	ZRObjAlignInfos infos;

	while (nb--)
	{
		for (size_t i = 0; i < nbCols; i++)
		{
			infos = table->ainfos[i];
			ZRVECTOR_SET(vtable->columns[i], line, ZRARRAYOP_GET(src, infos.offset));
		}
		line++;
		src = ZRARRAYOP_GET(src, objSize);
	}
}

ZRMUSTINLINE
static inline void reserve_column(ZRTable *table, size_t line, size_t nb)
{
	VectorTable *vtable = VT(table);
	size_t nbCols = table->nbColumns;

	for (size_t i = 0; i < nbCols; i++)
		ZRVECTOR_RESERVE_NB(vtable->columns[i], line, nb);
}

static void freserve_column(ZRTable *table, size_t line, size_t nb)
{
	reserve_column(table, line, nb);
	table->nbLines += nb;
}

ZRMUSTINLINE
static inline void delete_column(ZRTable *table, size_t line, size_t nb)
{
	VectorTable *vtable = VT(table);
	size_t nbCols = table->nbColumns;

	for (size_t i = 0; i < nbCols; i++)
		ZRVECTOR_DELETE_NB(vtable->columns[i], line, nb);
}

static void fdelete_column(ZRTable *table, size_t line, size_t nb)
{
	delete_column(table, line, nb);
	table->nbLines -= nb;
}

static inline void* fgetObjects_column(ZRTable *table, size_t line, size_t nb)
{
	VectorTable *vtable = VT(table);
	size_t nbCols = table->nbColumns;
	size_t nbLines = table->nbLines;
	size_t objSize = ZRTABLE_OBJINFOS(table).size;
	size_t lineMax = line + nb;
	void *ret = ZROBJALLOC_NB(vtable->allocator, ZRTABLE_OBJINFOS(table), ZRTABLE_NBLINES(table));

	for (size_t col = 0; col < nbCols; col++)
	{
		ZRObjAlignInfos infos = table->ainfos[col];
		ZRVector *column = vtable->columns[col];
		void *obj = ret;

		for (size_t line_i = line; line_i < lineMax; line_i++)
		{
			memcpy(ZRARRAYOP_GET(obj, infos.offset), ZRVECTOR_GET(column, line_i), infos.size);
			obj = ZRARRAYOP_GET(obj, objSize);
		}
	}
	return ret;
}

static void fgetObjects_end_column(ZRTable *table, void *objects)
{
	VectorTable *vtable = VT(table);
	ZRFREE(vtable->allocator, objects);
}

static inline void* fgetColumn_column(ZRTable *table, size_t line, size_t column, size_t nb)
{
	VectorTable *vtable = VT(table);
	return ZRVECTOR_GET(vtable->columns[column], line);
}

static void fgetColumn_end_column(ZRTable *table, void *column)
{
}

static void fdone_column(ZRTable *table)
{
	VectorTable *vtable = VT(table);
	size_t nbCols = table->nbColumns;

	for (size_t i = 0; i < nbCols; i++)
		ZRVector_destroy(vtable->columns[i]);
}

static void fdestroy_column(ZRTable *table)
{
	VectorTable *vtable = VT(table);
	fdone_column(VT_T(vtable));
	ZRFREE(vtable->allocator, vtable);
}

/* ========================================================================= */

static void* fget_column_both(ZRTable *table, size_t line, size_t column)
{
	fget_column_column(table, line, column);
	fget_column_object(table, line, column);
}

static void fcpy_column_both(ZRTable *table, size_t line, size_t column, size_t nb, void *dest)
{
	fcpy_column_column(table, line, column, nb, dest);
	fcpy_column_object(table, line, column, nb, dest);
}

static void fset_column_both(ZRTable *table, size_t line, size_t column, size_t nb, void *src)
{
	fset_column_column(table, line, column, nb, src);
	fset_column_object(table, line, column, nb, src);
}

static void fcpy_both(ZRTable *table, size_t line, size_t nb, void *dest)
{
	fcpy_column(table, line, nb, dest);
	fcpy_object(table, line, nb, dest);
}

static void fset_both(ZRTable *table, size_t line, size_t nb, void *src)
{
	fset_column(table, line, nb, src);
	fset_object(table, line, nb, src);
}

static void freserve_both(ZRTable *table, size_t line, size_t nb)
{
	reserve_column(table, line, nb);
	reserve_object(table, line, nb);
	table->nbLines += nb;
}

static void fdelete_both(ZRTable *table, size_t line, size_t nb)
{
	delete_column(table, line, nb);
	delete_object(table, line, nb);
	table->nbLines -= nb;
}

#define fgetObjects_both fgetObjects_object
#define fgetObjects_end_both fgetObjects_end_object
#define fgetColumn_both fgetColumn_column
#define fgetColumn_end_both fgetColumn_end_column

static void fdone_both(ZRTable *table)
{
	fdone_column(table);
	fdone_object(table);
}

static void fdestroy_both(ZRTable *table)
{
	VectorTable *vtable = VT(table);
	fdone_both(VT_T(vtable));
	ZRFREE(vtable->allocator, vtable);
}

/* ========================================================================= */

typedef struct
{
	ZRObjAlignInfos ainfos[ALIGNINFOS_NB];
	ZRAllocator *allocator;

	ZRObjAlignInfos *columnInfos;
	size_t nbColumns;

	unsigned mode :ZRVECTORTABLEMODEE_NBBITS;
	unsigned staticStrategy :1;
	unsigned changefdestroy :1;
} InitInfos;

#define II(I) ((InitInfos*)(I))
#define MODE_OBJECT(MODE) (MODE & ZRVectorTableModeE_object)
#define MODE_COLUMN(MODE) (MODE & ZRVectorTableModeE_column)
#define MAINOBJINFOS(II)    ZROBJALIGNINFOS_CPYOBJINFOS((II)->columnInfos[(II)->nbColumns])
#define COLOBJINFOS(II,COL) ZROBJALIGNINFOS_CPYOBJINFOS((II)->columnInfos[COL])

static void default_objects(ZRInitInfos_t *iinfos, InitInfos *initInfos)
{
	ZRVector2SideStrategyIInfos(iinfos, MAINOBJINFOS(initInfos));
	ZRVector2SideStrategyIInfos_allocator(iinfos, initInfos->allocator);

	if (initInfos->staticStrategy)
		ZRVector2SideStrategyIInfos_staticStrategy(iinfos);
}

static ZRObjAlignInfos default_objects_infos(InitInfos *initInfos)
{
	ZRInitInfos_t iinfos;
	default_objects(&iinfos, initInfos);
	return ZROBJINFOS_CPYOBJALIGNINFOS(ZRVector2SideStrategy_objInfos(&iinfos));
}

static void default_column(ZRInitInfos_t *iinfos, InitInfos *initInfos, size_t column)
{
	ZRVector2SideStrategyIInfos(iinfos, COLOBJINFOS(initInfos, column));
	ZRVector2SideStrategyIInfos_allocator(iinfos, initInfos->allocator);

	if (initInfos->staticStrategy)
		ZRVector2SideStrategyIInfos_staticStrategy(iinfos);
}

static void makeColumnsInfos(ZRObjAlignInfos *infos, InitInfos *initInfos)
{
	size_t const nbCols = initInfos->nbColumns;
	ZRInitInfos_t iinfos;

	for (size_t i = 0; i < nbCols; i++)
	{
		default_column(&iinfos, initInfos, i);
		infos[i] = ZRVector2SideStrategy_objAlignInfos(&iinfos);
	}
	ZRStruct_bestOffsets(nbCols, infos);
}

static ZRObjAlignInfos default_columns_infos(InitInfos *initInfos)
{
	size_t const nbCols = initInfos->nbColumns;
	ZRObjAlignInfos infos[nbCols + 1];
	makeColumnsInfos(infos, initInfos);
	return infos[nbCols];
}

static void makeAlignInfos(ZRObjAlignInfos *ainfos, InitInfos *initInfos)
{
	if (MODE_OBJECT(initInfos->mode))
		ainfos[AlignInfos_objects] = default_objects_infos(initInfos);
	else
		ainfos[AlignInfos_objects] = ZROBJALIGNINFOS_DEF0();

	if (MODE_COLUMN(initInfos->mode))
	{
		ainfos[AlignInfos_columns] = default_columns_infos(initInfos);
		ainfos[AlignInfos_columnsP] = ZRTYPENB_OBJALIGNINFOS(ZRVector*, initInfos->nbColumns);
	}
	else
	{
		ainfos[AlignInfos_columns] = ZROBJALIGNINFOS_DEF0();
		ainfos[AlignInfos_columnsP] = ZROBJALIGNINFOS_DEF0();
	}
	ainfos[AlignInfos_base] = ZRTYPE_OBJALIGNINFOS(VectorTable);
	ainfos[AlignInfos_columnInfos] = ZRTYPENB_OBJALIGNINFOS(ZRObjAlignInfos, initInfos->nbColumns + 1);
	ainfos[AlignInfos_strategy] = initInfos->staticStrategy ? ZRTYPE_OBJALIGNINFOS(Strategy) : ZROBJALIGNINFOS_DEF0();
	ainfos[AlignInfos_struct] = ZROBJALIGNINFOS_DEF0();
	ZRStruct_bestOffsetsPos(ALIGNINFOS_NB - 1, ainfos, 1);
}

static ZRAllocator* initInfos_getAllocator(InitInfos *initInfos)
{
	if (initInfos->allocator != NULL)
		return initInfos->allocator;

	return zrlib_getServiceFromID(ZRSERVICE_ID(ZRService_allocator)).object;
}

static void initInfos_preInitValidate(InitInfos *initInfos)
{
	initInfos->allocator = initInfos_getAllocator(initInfos);
}

static void initInfos_validate(InitInfos *initInfos)
{
	makeAlignInfos(initInfos->ainfos, initInfos);
}

ZRObjInfos ZRVectorTableIInfosObjInfos(void)
{
	return ZRTYPE_OBJINFOS(InitInfos);
}

void ZRVectorTableIInfos(void *iinfos, ZRObjAlignInfos *columnInfos, size_t nb)
{
	InitInfos *const initInfos = II(iinfos);

	*initInfos = (InitInfos ) { //
		.columnInfos = columnInfos,
		.nbColumns = nb,
		.mode = ZRVectorTableModeE_default,
		};
	initInfos_validate(initInfos);
}

void ZRVectorTableIInfos_done(void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
}

void ZRVectorTableIInfos_mode(void *iinfos, enum ZRVectorTableModeE mode)
{
	InitInfos *const initInfos = II(iinfos);
	initInfos->mode = (unsigned)mode;
	initInfos_validate(initInfos);
}

void ZRVectorTableIInfos_allocator(void *iinfos, ZRAllocator *allocator)
{
	InitInfos *const initInfos = II(iinfos);
	initInfos->allocator = allocator;
}

void ZRVectorTableIInfos_staticStrategy(void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
	initInfos->staticStrategy = 1;
	initInfos_validate(initInfos);
}

ZRObjInfos ZRVectorTable_objInfos(void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
	return ZRTYPE_OBJINFOS(initInfos->ainfos[AlignInfos_struct]);
}

static void Strategy_init_object(Strategy *strategy, InitInfos *initInfos)
{
	*strategy = (Strategy ) { //
		.strategy = (ZRTableStrategy ) { //
			.fget_column = fget_column_object,
			.fcpy_column = fcpy_column_object,
			.fset_column = fset_column_object,
			.fcpy = fcpy_object,
			.fset = fset_object,
			.freserve = freserve_object,
			.fdelete = fdelete_object,
			.fgetObjects = fgetObjects_object,
			.fgetObjects_end = fgetObjects_end_object,
			.fgetColumn = fgetColumn_object,
			.fgetColumn_end = fgetColumn_end_object,
			.fdestroy = initInfos->changefdestroy ? fdestroy_object : fdone_object,
			} ,
		};
}

static void Strategy_init_column(Strategy *strategy, InitInfos *initInfos)
{
	*strategy = (Strategy ) { //
		.strategy = (ZRTableStrategy ) { //
			.fget_column = fget_column_column,
			.fcpy_column = fcpy_column_column,
			.fset_column = fset_column_column,
			.fcpy = fcpy_column,
			.fset = fset_column,
			.freserve = freserve_column,
			.fdelete = fdelete_column,
			.fgetObjects = fgetObjects_column,
			.fgetObjects_end = fgetObjects_end_column,
			.fgetColumn = fgetColumn_column,
			.fgetColumn_end = fgetColumn_end_column,
			.fdestroy = initInfos->changefdestroy ? fdestroy_column : fdone_column,
			} ,
		};
}

static void Strategy_init_both(Strategy *strategy, InitInfos *initInfos)
{
	*strategy = (Strategy ) { //
		.strategy = (ZRTableStrategy ) { //
			.fget_column = fget_column_both,
			.fcpy_column = fcpy_column_both,
			.fset_column = fset_column_both,
			.fcpy = fcpy_both,
			.fset = fset_both,
			.freserve = freserve_both,
			.fdelete = fdelete_both,
			.fgetObjects = fgetObjects_both,
			.fgetObjects_end = fgetObjects_end_both,
			.fgetColumn = fgetColumn_both,
			.fgetColumn_end = fgetColumn_end_both,
			.fdestroy = initInfos->changefdestroy ? fdestroy_both : fdone_both,
			} ,
		};
}

static void VectorTable_init(ZRTable *table, InitInfos *initInfos)
{
	ZRInitInfos_t tmp_iinfos;
	VectorTable *vtable = VT(table);
	ZRTableStrategy *strategy;
	Strategy ref;
	ZRObjAlignInfos *ainfos;
	ZRVector *objects;
	ZRVector **columns;

	switch (initInfos->mode)
	{
	case ZRVectorTableModeE_object:
		Strategy_init_object(&ref, initInfos);
		break;
	case ZRVectorTableModeE_column:
		Strategy_init_column(&ref, initInfos);
		break;
	case ZRVectorTableModeE_both:
		Strategy_init_both(&ref, initInfos);
		break;
	}

	if (initInfos->staticStrategy)
	{
		strategy = ZRARRAYOP_GET(vtable, initInfos->ainfos[AlignInfos_strategy].offset);
		ZRPTYPE_CPY(strategy, &ref);
	}
	else
		strategy = zrlib_internPType(&ref);

	ainfos = ZRARRAYOP_GET(vtable, initInfos->ainfos[AlignInfos_columnInfos].offset);
	ZRPTYPE_CPY_NB(ainfos, initInfos->columnInfos, initInfos->nbColumns + 1);

	if (MODE_OBJECT(initInfos->mode))
	{
		objects = ZRARRAYOP_GET(vtable, initInfos->ainfos[AlignInfos_objects].offset);
		default_objects(&tmp_iinfos, initInfos);
		ZRVector2SideStrategy_init(objects, &tmp_iinfos);
	}
	else
		objects = NULL;

	if (MODE_COLUMN(initInfos->mode))
	{
		size_t const nbCols = initInfos->nbColumns;
		ZRObjAlignInfos infos[nbCols + 1];
		columns = ZRARRAYOP_GET(vtable, initInfos->ainfos[AlignInfos_columnsP].offset);
		size_t coffset = initInfos->ainfos[AlignInfos_columns].offset;
		makeColumnsInfos(infos, initInfos);

		for (size_t i = 0; i < nbCols; i++)
		{
			columns[i] = ZRARRAYOP_GET(vtable, coffset + infos[i].offset);
			default_column(&tmp_iinfos, initInfos, i);
			ZRVector2SideStrategy_init(columns[i], &tmp_iinfos);
		}
	}
	else
		columns = NULL;

	*vtable = (VectorTable ) { //
		.table = (ZRTable ) { //
			.strategy = strategy,
			.ainfos = ainfos,
			.nbColumns = initInfos->nbColumns,
			},
		.allocator = initInfos->allocator,
		.objects = objects,
		.columns = columns,
		};
}

void ZRVectorTable_init(ZRTable *table, void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
	initInfos_preInitValidate(initInfos);
	VectorTable_init(table, initInfos);
}

ZRTable* ZRVectorTable_new(void *iinfos)
{
	InitInfos *const initInfos = II(iinfos);
	initInfos_preInitValidate(initInfos);
	ZRTable *table = ZROBJALLOC(initInfos->allocator, ZROBJALIGNINFOS_CPYOBJINFOS(initInfos->ainfos[ALIGNINFOS_NB - 1]));

	initInfos->changefdestroy = 1;
	VectorTable_init(table, iinfos);
	initInfos->changefdestroy = 0;
	return table;
}
