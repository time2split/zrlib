/**
 * @author zuri
 * @date mercredi 4 décembre 2019, 23:53:30 (UTC+0100)
 */

#define STRATEGY(TREE) ((ZRSimpleTreeStrategy*)(TREE)->strategy)
#define DATA(TREE) ((ZRSimpleTreeData*)(TREE)->sdata)

// ============================================================================
//Data

typedef struct ZRSimpleTreeDataS ZRSimpleTreeData;
typedef struct ZRSimpleTreeStrategyS ZRSimpleTreeStrategy;

struct ZRSimpleTreeStrategyS
{
	ZRTREESTRATEGY_MEMBERS();
};

struct ZRSimpleTreeDataS
{
	size_t objSize;

	ZRAllocator *allocator;

	ZRVector *nodes;
};

// ============================================================================
// Node

typedef struct ZRSimpleTreeNodeS ZRSimpleTreeNode;

#define TYPEDEF_NODE(OBJSIZE) typedef STRUCT_NODE(OBJSIZE) ZRSimpleTreeNodeInstance
#define STRUCT_NODE(OBJSIZE) STRUCT_NODE_NAME(,OBJSIZE)
#define STRUCT_NODE_NAME(NAME,OBJSIZE) \
struct NAME{ \
	size_t nbChilds; \
	ZRTreeNode *parent; \
	ZRTreeNode *childs; \
	char obj[OBJSIZE]; \
}

#define TYPEDEF_NODE_AUTO(TREE) typedef STRUCT_NODE_AUTO(TREE) ZRSimpleTreeNodeInstance
#define STRUCT_NODE_AUTO(TREE) STRUCT_NODE(DATA(TREE)->objSize)

STRUCT_NODE_NAME(ZRSimpleTreeNodeS,);
