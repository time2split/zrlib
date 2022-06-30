#ifndef ZRMACRO_CARRAY
#define ZRMACRO_CARRAY


#include "math.h"
#include "code.h"


#define ZRCARRAY_NBOBJ(array) (sizeof(array)/sizeof(*array))
#define ZRCARRAY_CPY(A,B) memcpy(A, B, sizeof(A))


#define ZRCARRAY_XCPY(dest,src,nb,X) ZRBLOCK( \
	for(size_t _i = 0, _c = (nb) ; _i < _c ; _i++) \
		dest[_i] = X(src[_i]); \
	)

#define ZRCARRAY_XXCPY(dest,src,nb,SET) ZRBLOCK( \
	for(size_t _i = 0, _c = (nb) ; _i < _c ; _i++) \
		SET(dest[_i], src[_i]); \
	)

#define ZRCARRAY_CHECKFORCPY(offset, nbObj, maxNbCpy, CODE_ERROR, CODE_END) ZRBLOCK( \
	size_t const _offset = (offset); \
	size_t const _nbObj = (nbObj); \
	\
	if (_offset >= _nbObj) \
		CODE_ERROR; \
	\
	size_t const _nbFromOffset = _nbObj - _offset; \
	size_t const _maxNbCpy = (maxNbCpy); \
	size_t const _nb = ZRMIN(_maxNbCpy, _nbFromOffset); \
	\
	CODE_END; \
)

#define ZRCARRAY_SAFECPY(offset, nbObj, maxNbCpy, i, CODE_ERROR, CODE, CODE_END) \
	ZRCARRAY_CHECKFORCPY(offset, nbObj, maxNbCpy, CODE_ERROR, ZRCODE( \
		for (size_t i = 0; i < _nb; i++) { \
			CODE; \
		} \
		CODE_END \
	) \
)

//(T, T** pointers, TS, TS* source, size_t nbObj)
#define ZRCARRAY_TOPOINTERS(TP, pointers, TS, source, nbObj) ZRBLOCK( \
	size_t _nb = (nbObj); \
	TP** P = (pointers); \
	TS* S = (source); \
	\
	while (_nb--) \
		P[_nb] = (TP*)(&S[_nb]); \
)

//(TO, TO* offset, TS, TS** source, size_t nbObj)
#define ZRCARRAY_FROMPOINTERS(TO, offset, TS, source, nbObj) ZRBLOCK( \
	size_t _nb = (nbObj); \
	TO* O = (offset); \
	TS** S = (source); \
	\
	while (_nb--) \
		O[_nb] = *S[_nb]; \
)

//(TO, TO** offset, TS, TS** source, size_t nbObj)
#define ZRCARRAY_CPYPOINTERS(TO, offset, TS, source, nbObj) ZRBLOCK( \
	size_t _nb = (nbObj); \
	TO** O = (offset); \
	TS** S = (source); \
	\
	while (_nb--) \
		O[_nb] = (TO*)S[_nb]; \
)

//(T, T** pointers, T* source, size_t nbObj)
#define ZRCARRAY_TOPOINTERSDATA(T, pointers, source, nbObj) ZRBLOCK( \
	size_t _nb = (nbObj); \
	T** P = (pointers); \
	T* S = (source); \
	\
	while (_nb--) \
		*P[_nb] = S[_nb]; \
)

//(T, T* offset, T** source, size_t nbObj)
#define ZRCARRAY_FROMPOINTERSDATA(T, offset, source, nbObj) ZRBLOCK( \
	size_t _nb = (nbObj); \
	T* O = (offset); \
	T** S = (source); \
	\
	while (_nb--) \
		O[_nb] = *S[_nb]; \
)

#endif
