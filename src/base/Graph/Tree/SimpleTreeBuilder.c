/**
 * @author zuri
 * @date dimanche 1 décembre 2019, 15:36:56 (UTC+0100)
 */

#include <zrlib/base/Graph/Tree/Tree.h>
#include <zrlib/base/Graph/Tree/SimpleTree.h>
#include <zrlib/base/Vector/Vector2SideStrategy.h>

#include <assert.h>

#include "SimpleTree.h"

typedef struct
{
	ZRTREEBUILDERSTRATEGY_MEMBERS()
	;
} ZRSimpleTreeBuilderStrategy;

#define STRUCT_BUILDER_NODE(OBJSIZE) STRUCT_BUILDER_NODE_NAME(,OBJSIZE)
#define STRUCT_BUILDER_NODE_NAME(NAME,OBJSIZE) \
struct NAME \
{ \
	ZRVector *childs; \
	char obj[OBJSIZE]; \
}

typedef STRUCT_BUILDER_NODE()
ZRSimpleTreeBuilder_node;

typedef struct
{
	ZRTREEBUILDER_MEMBERS(ZRSimpleTreeBuilderStrategy);
	;
	size_t nodeSize;
	size_t objSize;
	size_t nbNodes;
	size_t build_nbNodes;
	ZRAllocator *allocator;
	ZRVector *nodeStack;
	ZRSimpleTreeBuilder_node *root;
	char rootSpace[];
} ZRSimpleTreeBuilder;

// ============================================================================

static void fBuilder_node(ZRSimpleTreeBuilder *builder, void *data)
{
	size_t const objSize = builder->objSize;
	size_t const stackNb = ZRVECTOR_NBOBJ(builder->nodeStack);
	ZRAllocator *const allocator = builder->allocator;

	typedef STRUCT_BUILDER_NODE(objSize)
	bnode_t;
	bnode_t *node;

	if (stackNb == 0)
	{
		node = (bnode_t*)builder->rootSpace;
		builder->root = (ZRSimpleTreeBuilder_node*)node;
	}
	else
	{
		bnode_t *current = *(void**)ZRVECTOR_GET(builder->nodeStack, stackNb - 1);
		size_t const nodePos = ZRVECTOR_NBOBJ(current->childs);

		ZRVECTOR_RESERVE(current->childs, nodePos, 1);
		node = ZRVECTOR_GET(current->childs, nodePos);
	}
	node->childs = ZRVector2SideStrategy_createDynamic(512, builder->nodeSize, allocator);

	if (data != NULL)
		memcpy(node->obj, data, builder->objSize);

	ZRVECTOR_ADD(builder->nodeStack, &node);
	builder->nbNodes++;
}

static void* fBuilder_currentNode(ZRSimpleTreeBuilder *builder)
{
	return ZRVECTOR_GET(builder->nodeStack, ZRVECTOR_NBOBJ(builder->nodeStack) - 1);
}

static void* fBuilder_currentObj(ZRSimpleTreeBuilder *builder)
{
	ZRSimpleTreeBuilder_node **current = ZRVECTOR_GET(builder->nodeStack, ZRVECTOR_NBOBJ(builder->nodeStack) - 1);
	return (*current)->obj;
}

static void fBuilder_end(ZRSimpleTreeBuilder *builder)
{
	assert(ZRVECTOR_NBOBJ(builder->nodeStack) > 0);
	ZRVECTOR_DEC(builder->nodeStack);
}

static inline void treeNodeFromBNode(ZRSimpleTreeBuilder *builder, ZRSimpleTreeBuilder_node *currentBNodes, ZRSimpleTree *tree, ZRSimpleTreeNode *parent, ZRSimpleTreeNode *treeNode_out)
{
	*treeNode_out = (ZRSimpleTreeNode ) { //
		.nbChilds = ZRVECTOR_NBOBJ(currentBNodes->childs), //
		.parent = (ZRTreeNode*)parent, //
		};
	TYPEDEF_NODE_AUTO(tree)
	;
	memcpy(((ZRSimpleTreeNodeInstance*)treeNode_out)->obj, currentBNodes->obj, builder->objSize);
}

static void fBuilder_build_rec(ZRSimpleTreeBuilder *builder, ZRSimpleTreeBuilder_node *currentBNode, ZRSimpleTree *tree, ZRSimpleTreeNode *currentTreeNode)
{
	ZRVector *const treeNodes = tree->nodes;
	size_t const nbChilds = ZRVECTOR_NBOBJ(currentBNode->childs);
	size_t const nbNodes = builder->build_nbNodes;
	size_t i;

	for (i = 0; i < nbChilds; i++)
	{
		ZRSimpleTreeBuilder_node *const bchild = ZRVECTOR_GET(currentBNode->childs, i);
		ZRSimpleTreeNode *const treeChild = ZRVECTOR_GET(treeNodes, nbNodes + i);
		treeNodeFromBNode(builder, bchild, tree, currentTreeNode, treeChild);
	}
	builder->build_nbNodes += nbChilds;

	for (i = 0; i < nbChilds; i++)
	{
		ZRSimpleTreeBuilder_node *const child = ZRVECTOR_GET(currentBNode->childs, i);
		ZRSimpleTreeNode *const treeChild = ZRVECTOR_GET(treeNodes, nbNodes + i);

		treeChild->childs = ZRVECTOR_GET(treeNodes, builder->build_nbNodes);
		fBuilder_build_rec(builder, child, tree, treeChild);
	}
}

static void fBuilder_build(ZRSimpleTreeBuilder *builder, ZRSimpleTree *tree)
{
	size_t const nodeSize = sizeof(STRUCT_NODE(builder->objSize)
	);
	ZRVector *const treeNodes = ZRVector2SideStrategy_createFixed(builder->nbNodes, nodeSize, builder->allocator);

	tree->nodes = treeNodes;
	ZRVECTOR_RESERVE(treeNodes, 0, builder->nbNodes);

	ZRSimpleTreeNode *const treeRoot = ZRVECTOR_GET(treeNodes, 0);
	tree->root = (ZRTreeNode*)treeRoot;

	treeNodeFromBNode(builder, builder->root, tree, NULL, treeRoot);
	treeRoot->childs = ZRVECTOR_GET(treeNodes, 1);
	builder->build_nbNodes = 1;
	fBuilder_build_rec(builder, builder->root, tree, treeRoot);

	tree->nbNodes = builder->nbNodes;
	tree->nbEdges = builder->nbNodes ? builder->nbNodes - 1 : 0;
}

static ZRSimpleTree* fBuilder_new(ZRSimpleTreeBuilder *builder)
{
	ZRSimpleTree *tree = (ZRSimpleTree*)ZRSimpleTree_create(builder->objSize, builder->allocator, NULL);
	fBuilder_build(builder, tree);
	return tree;
}

static void fBuilder_done(ZRSimpleTreeBuilder *builder)
{
	ZRAllocator *allocator = builder->allocator;
	ZRFREE(allocator, builder->nodeStack);
}

// ============================================================================
// BUILDER HELP
// ============================================================================

static void ZRSimpleTreeBuilderS_init(ZRSimpleTreeBuilderStrategy *strategy)
{
	*strategy = (ZRSimpleTreeBuilderStrategy ) { //
		.fcurrentNode = (fcurrentNode_t)fBuilder_currentNode, //
		.fcurrentObj = (fcurrentObj_t)fBuilder_currentObj, //
		.fnode = (fnode_t)fBuilder_node, //
		.fend = (fend_t)fBuilder_end, //
		.fnew = (fnew_t)fBuilder_new, //
		.fdone = (fdone_t)fBuilder_done, //
		};
}

static void ZRSimpleTreeBuilder_init(ZRSimpleTreeBuilder *builder, ZRSimpleTreeBuilderStrategy *strategy, size_t objSize, ZRAllocator *allocator)
{
	size_t const nodeSize = objSize + sizeof(ZRSimpleTreeBuilder_node);

	*builder = (ZRSimpleTreeBuilder ) { //
		.strategy = strategy, //
		.nbNodes = 0, //
		.objSize = objSize, //
		.nodeSize = nodeSize, //
		.allocator = allocator, //
		.nodeStack = ZRVector2SideStrategy_createDynamic(512, sizeof(void*), allocator), //
		.root = NULL , //
		};
}

ZRTreeBuilder* ZRSimpleTreeBuilder_create(size_t objSize, ZRAllocator *allocator)
{
	ZRSimpleTreeBuilderStrategy *strategy = ZRALLOC(allocator, sizeof(ZRSimpleTreeBuilderStrategy));
	ZRSimpleTreeBuilderS_init(strategy);

	size_t const bnodeSize = sizeof(STRUCT_NODE(objSize)
	);
	ZRSimpleTreeBuilder *builder = ZRALLOC(allocator, sizeof(ZRSimpleTreeBuilder) + bnodeSize);
	ZRSimpleTreeBuilder_init(builder, strategy, objSize, allocator);
	return (ZRTreeBuilder*)builder;
}

void ZRSimpleTreeBuilder_done(ZRTreeBuilder *builder)
{
	ZRVector2SideStrategy_destroy(((ZRSimpleTreeBuilder*)builder)->nodeStack);
}

static void ZRSimpleTreeBuilder_destroyNode(ZRSimpleTreeBuilder *sbuilder, ZRSimpleTreeBuilder_node *node)
{
	size_t i;
	size_t const nb = node->childs->nbObj;

	for (i = 0; i < nb; i++)
		ZRSimpleTreeBuilder_destroyNode(sbuilder, ZRVECTOR_GET(node->childs, i));

	ZRVector2SideStrategy_destroy(node->childs);
}

void ZRSimpleTreeBuilder_destroy(ZRTreeBuilder *builder)
{
	ZRSimpleTreeBuilder *sbuilder = (ZRSimpleTreeBuilder*)builder;
	ZRAllocator *allocator = sbuilder->allocator;

	ZRSimpleTreeBuilder_destroyNode(sbuilder, sbuilder->root);
	ZRSimpleTreeBuilder_done(builder);

	ZRFREE(allocator, builder->strategy);
	ZRFREE(allocator, builder);
}
