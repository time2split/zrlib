/**
 * @author zuri
 * @date dimanche 9 décembre 2018, 21:11:56 (UTC+0100)
 */

#ifndef ZRBITS_H
#define ZRBITS_H

#include <zrlib/config.h>
#include <zrlib/base/ArrayOp.h>
#include <zrlib/base/macro.h>

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// ============================================================================

typedef uint_fast32_t ZRBits;

// ============================================================================

/**
 * Number of bits in an object ZRBits.
 */
#define ZRBITS_NBOF (sizeof(ZRBits) * CHAR_BIT)

#define ZRBITS_0 ((ZRBits)0)

#define ZRBITS_MASK_1R ((ZRBits)1)
#define ZRBITS_MASK_1L ~(ZRBITS_MASK_FULL >> 1)
#define ZRBITS_MASK_FULL (~(ZRBits)0)

// ============================================================================

#define ZRBITS_1ROTATE(bits,nb,shiftOp,shiftOpReverse) \
	(((bits) shiftOp nb) | ((bits) shiftOpReverse (sizeof(bits) * CHAR_BIT - nb)))
#define ZRBITS_1ROTATEL(bits,nb) ZRBITS_1ROTATE(bits,nb,<<,>>)
#define ZRBITS_1ROTATER(bits,nb) ZRBITS_1ROTATE(bits,nb,>>,<<)

// ============================================================================

#define ZRBITS_PACK_ARRAY(bits,nbBits,source) \
do{\
	register size_t pos = 0;\
	register size_t i = 0;\
	register size_t ss = ZRCARRAY_NBOBJ(source);\
	\
	while (ss--)\
	{\
		ZRBits_setBitsFromTheRight(bits, pos, nbBits, source[i++]);\
		pos += nbBits;\
	}\
}while(0)

#define ZRBITS_PACK_ARRAY_TYPE(bits,source) \
	ZRBITS_PACK_(bits,nbBits,sizeof(*source),ZRCARRAY_NBOBJ(source))

// ============================================================================

ZRBits ZRBits_getMask(__ size_t nbBits, bool toTheRight);
ZRBits ZRBits_getLMask(_ size_t nbBits);
ZRBits ZRBits_getRMask(_ size_t nbBits);

void ZRBits_cpack(_ ZRBits _________*bits, size_t nbBits, char ___________*source, size_t sourceSize);
void ZRBits_pack(__ ZRBits *restrict bits, size_t nbBits, ZRBits *restrict source, size_t sourceSize);

void ZRBits_setBit(______________ ZRBits *bits, size_t pos, bool bit);
void ZRBits_setBits(_____________ ZRBits *bits, size_t pos, size_t nbBits, ZRBits source, bool fromTheRight);
void ZRBits_setBitsFromTheRight(_ ZRBits *bits, size_t pos, size_t nbBits, ZRBits source);
void ZRBits_setBitsFromTheLeft(__ ZRBits *bits, size_t pos, size_t nbBits, ZRBits source);

bool ZRBits_getBit(__ ZRBits const _________*bits, size_t pos);
void ZRBits_getBits(_ ZRBits const *restrict bits, size_t pos, size_t nbBits, ZRBits * restrict out);
void ZRBits_copy(____ ZRBits const *restrict bits, size_t pos, size_t nbBits, ZRBits * restrict out, size_t outPos);

void ZRBits_inArrayShift(__ ZRBits *bits, size_t nbZRBits, size_t shift, size_t toTheRight);
void ZRBits_inArrayLShift(_ ZRBits *bits, size_t nbZRBits, size_t shift);
void ZRBits_inArrayRShift(_ ZRBits *bits, size_t nbZRBits, size_t shift);

#endif
