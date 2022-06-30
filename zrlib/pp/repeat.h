#ifndef ZRPP_H
#error "No manual include allowed"
#endif


/**
 * macro(count,...)
 */
#define ZRREPEAT(count, macro, ...) \
    ZRIF(count) \
    ( \
        ZRREPEAT_INDIRECT ZRDEFER2() () \
        ( \
            ZRDEC(count), macro, __VA_ARGS__ \
        ) \
        macro ZRDEFER2() \
        ( \
            ZRDEC(count), __VA_ARGS__ \
        ) \
    )
#define ZRREPEAT_INDIRECT() ZRREPEAT
#define ZRREPEAT_E(...) ZREVAL(ZRREPEAT(__VA_ARGS__))

