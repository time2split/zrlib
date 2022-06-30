#ifndef ZRMACRO_CODE
#define ZRMACRO_CODE


#ifdef __GNUC__
#define ZRMUSTINLINE __attribute__((always_inline))
#else
#define ZRMUSTINLINE
#endif

#define ZRCODE(...) __VA_ARGS__

#define ZRBLOCK(...) \
do { \
	__VA_ARGS__ \
}while(0)

#endif
