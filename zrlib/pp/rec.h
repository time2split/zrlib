#ifndef ZRPP_H
#error "No manual include allowed"
#endif

#include "rec_gen.h"

/* https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms */
#define ZREVAL(...)   ZREVAL64(__VA_ARGS__)
#define ZREVAL64(...) ZREVAL32(ZREVAL32(__VA_ARGS__))
#define ZREVAL32(...) ZREVAL16(ZREVAL16(__VA_ARGS__))
#define ZREVAL16(...) ZREVAL8(ZREVAL8(__VA_ARGS__))
#define ZREVAL8(...)  ZREVAL4(ZREVAL4(__VA_ARGS__))
#define ZREVAL4(...)  ZREVAL2(ZREVAL2(__VA_ARGS__))
#define ZREVAL2(...)  ZREVAL1(ZREVAL1(__VA_ARGS__))
#define ZREVAL1(...) __VA_ARGS__
