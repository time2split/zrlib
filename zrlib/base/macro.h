/**
 * @author zuri
 * @date mardi 18 décembre 2018, 22:46:00 (UTC+0100)
 */

#define ZRSIZE_UNKNOW (~(size_t)0)
#define ZRTOSTRING(V) #V

#define ZRCONCAT(A,B) ZRCONCAT_(A,B)
#define ZRCONCAT_(A,B) A ## B

#define ZRMIN(a,b) ((a) < (b) ? (a) : (b))

#define ZRCARRAY_NBOBJ(array) (sizeof(array)/sizeof(*array))
