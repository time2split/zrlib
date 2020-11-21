/**
 * @author zuri
 * @date mercredi 13 novembre 2019, 18:53:08 (UTC+0100)
 */

#ifndef ZRIDENTIFIER_H
#define ZRIDENTIFIER_H

#include <zrlib/config.h>
#include <zrlib/base/struct.h>

#include <stdbool.h>

#pragma zrlib conf generate.target "/src/base/Identifier/Identifier.c" "."
#pragma zrlib conf generate.prefix ZRIdentifier_

// ============================================================================

typedef struct ZRIdentifierStrategyS ZRIdentifierStrategy;
typedef struct ZRIdentifierS ZRIdentifier;

// ============================================================================

struct ZRIdentifierStrategyS
{
	ZRID (*fgetID)(ZRIdentifier *identifier, void *obj);
	void* (*fintern)(ZRIdentifier *identifier, void *obj);
	void* (*ffromID)(ZRIdentifier *identifier, ZRID id);

	ZRObjectP (*fobjectP)(ZRIdentifier *identifier, ZRID id);

	bool (*fcontains)(ZRIdentifier *identifier, void *obj);
	bool (*frelease)(ZRIdentifier *identifier, void *obj);
	bool (*freleaseID)(ZRIdentifier *identifier, ZRID id);
	bool (*freleaseAll)(ZRIdentifier *identifier);

	void (*fdone)(ZRIdentifier *identifier);
	void (*fdestroy)(ZRIdentifier *identifier);
};

struct ZRIdentifierS
{
	ZRIdentifierStrategy *strategy;

	size_t nbObj;
};

// ============================================================================

#pragma zrlib generate function done
ZRMUSTINLINE
static inline void ZRIDENTIFIER_DONE(ZRIdentifier *identifier)
{
	identifier->strategy->fdone(identifier);
}

#pragma zrlib generate function destroy
ZRMUSTINLINE
static inline void ZRIDENTIFIER_DESTROY(ZRIdentifier *identifier)
{
	identifier->strategy->fdestroy(identifier);
}

#pragma zrlib generate function nbObj
ZRMUSTINLINE
static inline size_t ZRIDENTIFIER_NBOBJ(ZRIdentifier *identifier)
{
	return identifier->nbObj;
}

/**
 * Return the Identifier of obj.
 * If it's not present, add a new one in the Identifier memory.
 */
#pragma zrlib generate function getID
ZRMUSTINLINE
static inline ZRID ZRIDENTIFIER_GETID(ZRIdentifier *identifier, void *obj)
{
	return identifier->strategy->fgetID(identifier, obj);
}

/**
 * Return the canonical representation of obj.
 * If it's not present, add a new one in the Identifier memory.
 */
#pragma zrlib generate function intern
ZRMUSTINLINE
static inline void* ZRIDENTIFIER_INTERN(ZRIdentifier *identifier, void *obj)
{
	return identifier->strategy->fintern(identifier, obj);
}

#pragma zrlib generate function fromID
ZRMUSTINLINE
static inline void* ZRIDENTIFIER_FROMID(ZRIdentifier *identifier, ZRID id)
{
	return identifier->strategy->ffromID(identifier, id);
}

#pragma zrlib generate function objectP
ZRMUSTINLINE
static inline ZRObjectP ZRIDENTIFIER_OBJECTP(ZRIdentifier *identifier, ZRID id)
{
	return identifier->strategy->fobjectP(identifier, id);
}

#pragma zrlib generate function contains
ZRMUSTINLINE
static inline bool ZRIDENTIFIER_CONTAINS(ZRIdentifier *identifier, void *obj)
{
	return identifier->strategy->fcontains(identifier, obj);
}

#pragma zrlib generate function release
ZRMUSTINLINE
static inline bool ZRIDENTIFIER_RELEASE(ZRIdentifier *identifier, void *obj)
{
	return identifier->strategy->frelease(identifier, obj);
}

#pragma zrlib generate function releaseID
ZRMUSTINLINE
static inline bool ZRIDENTIFIER_RELEASEID(ZRIdentifier *identifier, ZRID id)
{
	return identifier->strategy->freleaseID(identifier, id);
}

#pragma zrlib generate function releaseAll
ZRMUSTINLINE
static inline bool ZRIDENTIFIER_RELEASEALL(ZRIdentifier *identifier)
{
	return identifier->strategy->freleaseAll(identifier);
}

// ============================================================================

#pragma zrlib write generate headers



#endif
