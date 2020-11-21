/**
 * @author zuri
 * @date dim. 08 nov. 2020 12:41:50 CET
 */

#ifndef ZRTABLE_H
#define ZRTABLE_H

#include <zrlib/config.h>
#include <zrlib/base/struct.h>

#include <stdlib.h>

#pragma zrlib conf generate.target "/src/base/Table/Table.c" "."
#pragma zrlib conf generate.prefix ZRTable_

typedef struct ZRTableS ZRTable;
typedef struct ZRTableStrategyS ZRTableStrategy;

struct ZRTableStrategyS
{
	void* (*fget_column)(_ ZRTable*, size_t line, size_t column);
	void (*fcpy_column)(__ ZRTable*, size_t line, size_t column, size_t nb, void *dest);
	void (*fset_column)(__ ZRTable*, size_t line, size_t column, size_t nb, void *src);

	void (*fcpy)(__ ZRTable*, size_t line, size_t nb, void *dest);
	void (*fset)(__ ZRTable*, size_t line, size_t nb, void *src);
	void (*freserve)(ZRTable*, size_t line, size_t nb);
	void (*fdelete)(ZRTable*, size_t line, size_t nb);

	void* (*fgetObjects)(ZRTable*, size_t line, size_t nb);
	void (*fgetObjects_end)(ZRTable*, void *objects);
	void* (*fgetColumn)(ZRTable*, size_t line, size_t column, size_t nb);
	void (*fgetColumn_end)(ZRTable*, void *column);

	void (*fdestroy)(ZRTable*);
};

struct ZRTableS
{
	ZRTableStrategy *strategy;

	ZRObjAlignInfos *ainfos;

	size_t nbColumns;
	size_t nbLines;
};

#define ZRTABLE(T) ((ZRTable*)(T))

#pragma zrlib generate function size_t nbColumns(ZRTable*)
#define ZRTABLE_NBCOLUMNS(T) (T)->nbColumns

#pragma zrlib generate function size_t nbLines(ZRTable*)
#define ZRTABLE_NBLINES(T) (T)->nbLines

#pragma zrlib generate function size_t size(ZRTable*)
#define ZRTABLE_SIZE(T) (ZRTABLE_NBCOLUMNS(T) * ZRTABLE_NBLINES(T))

#define ZRTABLE_OBJINFOS(...) ZRCONCAT(ZRTABLE_OBJINFOS,ZRNARGS(__VA_ARGS__))(__VA_ARGS__)


#pragma zrlib generate function objAInfos
ZRMUSTINLINE
static inline ZRObjAlignInfos ZRTABLE_OBJAINFOS(ZRTable *table, size_t column)
{
	assert(column < table->nbColumns);
	return table->ainfos[column];
}

#pragma zrlib generate function objInfos
ZRMUSTINLINE
static inline ZRObjInfos ZRTABLE_OBJINFOS1(ZRTable *table)
{
	return ZROBJALIGNINFOS_CPYOBJINFOS(table->ainfos[ZRTABLE_NBCOLUMNS(table)]);
}

#pragma zrlib generate function objInfos_column
ZRMUSTINLINE
static inline ZRObjInfos ZRTABLE_OBJINFOS2(ZRTable *table, size_t column)
{
	assert(column < table->nbColumns);
	return ZROBJALIGNINFOS_CPYOBJINFOS(table->ainfos[column]);
}

#pragma zrlib generate function destroy
ZRMUSTINLINE
static inline void ZRTABLE_DESTROY(ZRTable *table)
{
	table->strategy->fdestroy(table);
}

#pragma zrlib generate function getObjects
ZRMUSTINLINE
static inline void* ZRTABLE_GETOBJECTS(ZRTable *table, size_t line, size_t nb)
{
	return table->strategy->fgetObjects(table, line, nb);
}

#pragma zrlib generate function getObjects_end
ZRMUSTINLINE
static inline void ZRTABLE_GETOBJECTS_END(ZRTable *table, void *objects)
{
	table->strategy->fgetObjects_end(table, objects);
}

#pragma zrlib generate function getColumn
ZRMUSTINLINE
static inline void* ZRTABLE_GETCOLUMN(ZRTable *table, size_t line, size_t column, size_t nb)
{
	return table->strategy->fgetColumn(table, line, column, nb);
}

#pragma zrlib generate function getColumn_end
ZRMUSTINLINE
static inline void ZRTABLE_GETCOLUMN_END(ZRTable *table, void *column)
{
	table->strategy->fgetColumn_end(table, column);
}

#pragma zrlib generate function delete_nb
ZRMUSTINLINE
static inline void ZRTABLE_DELETE_NB(ZRTable *table, size_t line, size_t nb)
{
	table->strategy->fdelete(table, line, nb);
}

#pragma zrlib generate function delete
ZRMUSTINLINE
static inline void ZRTABLE_DELETE(ZRTable *table, size_t line)
{
	table->strategy->fdelete(table, line, 1);
}

#pragma zrlib generate function delete_all
ZRMUSTINLINE
static inline void ZRTABLE_DELETE_ALL(ZRTable *table)
{
	table->strategy->fdelete(table, 0, ZRTABLE_NBLINES(table));
}

#pragma zrlib generate function reserve_nb
ZRMUSTINLINE
static inline void ZRTABLE_RESERVE_NB(ZRTable *table, size_t line, size_t nb)
{
	table->strategy->freserve(table, line, nb);
}

#pragma zrlib generate function reserve
ZRMUSTINLINE
static inline void ZRTABLE_RESERVE(ZRTable *table, size_t line)
{
	table->strategy->freserve(table, line, 1);
}

#pragma zrlib generate function get_column
ZRMUSTINLINE
static inline void* ZRTABLE_GET_COLUMN(ZRTable *table, size_t line, size_t column)
{
	return table->strategy->fget_column(table, line, column);
}

#pragma zrlib generate function cpy_column_nb
ZRMUSTINLINE
static inline void ZRTABLE_CPY_COLUMN_NB(ZRTable *table, size_t line, size_t column, size_t nb, void *dest)
{
	table->strategy->fcpy_column(table, line, column, nb, dest);
}

#pragma zrlib generate function cpy_column
ZRMUSTINLINE
static inline void ZRTABLE_CPY_COLUMN(ZRTable *table, size_t line, size_t column, void *dest)
{
	table->strategy->fcpy_column(table, line, column, 1, dest);
}

#pragma zrlib generate function set_column_nb
ZRMUSTINLINE
static inline void ZRTABLE_SET_COLUMN_NB(ZRTable *table, size_t line, size_t column, size_t nb, void *src)
{
	table->strategy->fset_column(table, line, column, nb, src);
}

#pragma zrlib generate function set_column
ZRMUSTINLINE
static inline void ZRTABLE_SET_COLUMN(ZRTable *table, size_t line, size_t column, void *src)
{
	table->strategy->fset_column(table, line, column, 1, src);
}

#pragma zrlib generate function cpy_oai_nb
ZRMUSTINLINE
static inline void ZRTABLE_CPY_OAI_NB(ZRTable *table, size_t line, ZRObjAlignInfos *destInfos, size_t nb, void *dest)
{
	void *objects = ZRTABLE_GETOBJECTS(table, line, nb);
	ZRSTRUCT_CPY_OAI_NB(objects, table->ainfos, destInfos, table->nbColumns, nb, dest);
	ZRTABLE_GETOBJECTS_END(table, objects);
}

#pragma zrlib generate function cpy_oai
ZRMUSTINLINE
static inline void ZRTABLE_CPY_OAI(ZRTable *table, size_t line, ZRObjAlignInfos *destInfos, void *dest)
{
	ZRTABLE_CPY_OAI_NB(table, line, destInfos, 1, dest);
}

#pragma zrlib generate function cpy_nb
ZRMUSTINLINE
static inline void ZRTABLE_CPY_NB(ZRTable *table, size_t line, size_t nb, void *dest)
{
	table->strategy->fcpy(table, line, nb, dest);
}

#pragma zrlib generate function cpy
ZRMUSTINLINE
static inline void ZRTABLE_CPY(ZRTable *table, size_t line, void *dest)
{
	table->strategy->fcpy(table, line, 1, dest);
}

#pragma zrlib generate function set_oai_nb
ZRMUSTINLINE
static inline void ZRTABLE_SET_OAI_NB(ZRTable *table, size_t line, ZRObjAlignInfos *srcInfos, size_t nb, void *src)
{
	//TODO: better buffer
	char objSrc[srcInfos[table->nbColumns].size * nb];
	ZRSTRUCT_CPY_OAI_NB(src, srcInfos, table->ainfos, table->nbColumns, nb, objSrc);
	table->strategy->fset(table, line, nb, objSrc);
}

#pragma zrlib generate function set_oai
ZRMUSTINLINE
static inline void ZRTABLE_SET_OAI(ZRTable *table, size_t line, ZRObjAlignInfos *srcInfos, void *src)
{
	ZRTABLE_SET_OAI_NB(table, line, srcInfos, 1, src);
}

#pragma zrlib generate function set_nb
ZRMUSTINLINE
static inline void ZRTABLE_SET_NB(ZRTable *table, size_t line, size_t nb, void *src)
{
	table->strategy->fset(table, line, nb, src);
}

#pragma zrlib generate function set
ZRMUSTINLINE
static inline void ZRTABLE_SET(ZRTable *table, size_t line, void *src)
{
	table->strategy->fset(table, line, 1, src);
}

/* ========================================================================= */
/* HELP */

/* ========================================================================= */

#pragma zrlib write generate headers




#endif
