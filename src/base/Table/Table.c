#include <zrlib/base/Table/Table.h>

void* ZRTable_get_column(ZRTable *table, size_t line, size_t column)
{
	return ZRTABLE_GET_COLUMN(table, line, column);
}

void ZRTable_cpy_column_nb(ZRTable *table, size_t line, size_t column, size_t nb, void *dest)
{
	ZRTABLE_CPY_COLUMN_NB(table, line, column, nb, dest);
}

void ZRTable_set_column_nb(ZRTable *table, size_t line, size_t column, size_t nb, void *src)
{
	ZRTABLE_SET_COLUMN_NB(table, line, column, nb, src);
}

void ZRTable_cpy_oai_nb(ZRTable *table, size_t line, ZRObjAlignInfos *destInfos, size_t nb, void *dest)
{
	ZRTABLE_CPY_OAI_NB(table, line, destInfos, nb, dest);
}

void ZRTable_cpy_nb(ZRTable *table, size_t line, size_t nb, void *dest)
{
	ZRTABLE_CPY_NB(table, line, nb, dest);
}

void ZRTable_set_oai_nb(ZRTable *table, size_t line, ZRObjAlignInfos *srcInfos, size_t nb, void *src)
{
	ZRTABLE_SET_OAI_NB(table, line, srcInfos, nb, src);
}

void ZRTable_set_nb(ZRTable *table, size_t line, size_t nb, void *src)
{
	ZRTABLE_SET_NB(table, line, nb, src);
}

void ZRTable_reserve_nb(ZRTable *table, size_t line, size_t nb)
{
	ZRTABLE_RESERVE_NB(table, line, nb);
}

void ZRTable_delete_nb(ZRTable *table, size_t line, size_t nb)
{
	ZRTABLE_DELETE_NB(table, line, nb);
}

void ZRTable_delete(ZRTable *table, size_t line)
{
	ZRTABLE_DELETE(table, line);
}

void ZRTable_delete_all(ZRTable *table)
{
	ZRTABLE_DELETE_ALL(table);
}

void* ZRTable_getObjects(ZRTable *table, size_t line, size_t nb)
{
	return ZRTABLE_GETOBJECTS(table, line, nb);
}

void ZRTable_getObjects_end(ZRTable *table, void *objects)
{
	ZRTABLE_GETOBJECTS_END(table, objects);
}

void* ZRTable_getColumn(ZRTable *table, size_t line, size_t column, size_t nb)
{
	return ZRTABLE_GETCOLUMN(table, line, column, nb);
}

void ZRTable_getColumn_end(ZRTable *table, void *column)
{
	ZRTABLE_GETCOLUMN_END(table, column);
}

void ZRTable_destroy(ZRTable *table)
{
	ZRTABLE_DESTROY(table);
}
