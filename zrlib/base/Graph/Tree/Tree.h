/**
 * @author zuri
 * @date dimanche 1 décembre 2019, 15:06:14 (UTC+0100)
 */

#ifndef ZRTREE_H
#define ZRTREE_H

#include <zrlib/config.h>
#include <zrlib/base/Allocator/Allocator.h>
#include <zrlib/base/Iterator/Iterator.h>

#include "../Graph.h"

#include <stdlib.h>

// ============================================================================

typedef struct ZRTreeS ZRTree;
typedef struct ZRTreeStrategyS ZRTreeStrategy;

typedef ZRGraphNode ZRTreeNode;
typedef ZRGraphEdge ZRTreeEdge;

typedef struct ZRTreeBuilderS ZRTreeBuilder;

// ============================================================================

struct ZRTreeStrategyS
{
	ZRGraphStrategy graph;
	ZRTreeBuilder* (*fnewTreeBuilder)(ZRTree*, ZRTreeNode*);

	/**
	 * Change the root of the Tree.
	 * The function does not guarantee that the *newRoot pointer stay the same for the root ;
	 * the internal representation of the tree may change.
	 */
	void (*ftree_changeRoot)(ZRTree*, ZRTreeNode *newRoot);

	size_t (*ftreeNode_getNbAscendants)(_________ ZRTree*, ZRTreeNode*);
	size_t (*ftreeNode_getNbDescendants)(________ ZRTree*, ZRTreeNode*);

	ZRIterator* (*ftreeNode_getChilds)(_________ ZRTree*, ZRTreeNode*);
	ZRIterator* (*ftreeNode_getAscendants)(_____ ZRTree*, ZRTreeNode*);
	ZRIterator* (*ftreeNode_getDescendants)(____ ZRTree*, ZRTreeNode*);
	ZRIterator* (*ftreeNode_getDescendants_BF)(_ ZRTree*, ZRTreeNode*);
	ZRIterator* (*ftreeNode_getDescendants_DF)(_ ZRTree*, ZRTreeNode*);
};

#define ZRTREESTRATEGY(S) ((ZRTreeStrategy*)(S))
#define ZRTREESTRATEGY_GRAPH(TS) (&(TS)->graph)

struct ZRTreeS
{
	ZRGraph graph;
	ZRTreeNode *root;
};

#define ZRTREE(G) ((ZRTree*)(G))
#define ZRTREE_GRAPH(T) (&(T)->graph)

#define ZRTREE_STRATEGY(T) ((ZRTreeStrategy*)ZRTREE_GRAPH(T)->strategy)

// ============================================================================

ZRTreeNode* ZRTree_getRoot(ZRTree *tree);

ZRTreeBuilder* ZRTree_newBuilder(ZRTree *tree, ZRTreeNode *currentBuilderNode);

void ZRTree_changeRoot(ZRTree *tree, ZRTreeNode *newRoot);

// ============================================================================
// NODE
// ============================================================================

ZRTreeNode* ZRTreeNode_getTheParent(___ ZRTree *tree, ZRTreeNode *node);
void ______ ZRTreeNode_cpyTheParentEdge(ZRTree *tree, ZRTreeNode *node, ZRTreeEdge *edge);

size_t ZRTreeNode_getNbAscendants(__ ZRTree *tree, ZRTreeNode *node);
size_t ZRTreeNode_getNbDescendants(_ ZRTree *tree, ZRTreeNode *node);

ZRIterator* ZRTreeNode_getChilds(______ ZRTree *tree, ZRTreeNode *node);
ZRIterator* ZRTreeNode_getAscendants(__ ZRTree *tree, ZRTreeNode *node);
ZRIterator* ZRTreeNode_getDescendants(_ ZRTree *tree, ZRTreeNode *node);

/* Breadth first */
ZRIterator* ZRTreeNode_getDescendants_BF(ZRTree *tree, ZRTreeNode *node);

/* Depth first */
ZRIterator* ZRTreeNode_getDescendants_DF(ZRTree *tree, ZRTreeNode *node);

// ------
// Help
// ------

#define ZRTREENODE_FROMARRAYCOORDINATE(tree,array) ZRTreeNode_getNodeFromCoordinate(tree, ZRCARRAY_NBOBJ(array), array)
ZRTreeNode* ZRTreeNode_getNodeFromCoordinate(ZRTree *tree, size_t nb, size_t coord[nb]);

// -----
// Std
// -----

ZRIterator* ZRTreeNode_std_getDescendants_BF(ZRTree *tree, ZRTreeNode *node, ZRAllocator *allocator);
ZRIterator* ZRTreeNode_std_getDescendants_DF(ZRTree *tree, ZRTreeNode *node, ZRAllocator *allocator);

// ============================================================================
// TREE
// ============================================================================

ZRMUSTINLINE
static inline ZRTreeNode* ZRTREE_GETROOT(ZRTree *tree)
{
	return tree->root;
}

ZRMUSTINLINE
static inline void ZRTREE_CHANGEROOT(ZRTree *tree, ZRTreeNode *newRoot)
{
	return ZRTREE_STRATEGY(tree)->ftree_changeRoot(tree, newRoot);
}

ZRMUSTINLINE
static inline ZRTreeBuilder* ZRTREE_NEWBUILDER(ZRTree *tree, ZRTreeNode *currentBuilderNode)
{
	return ZRTREE_STRATEGY(tree)->fnewTreeBuilder(tree, currentBuilderNode);
}

ZRMUSTINLINE
static inline size_t ZRTREENODE_GETNBASCENDANTS(ZRTree *tree, ZRTreeNode *node)
{
	return ZRTREE_STRATEGY(tree)->ftreeNode_getNbAscendants(tree, node);
}

ZRMUSTINLINE
static inline size_t ZRTREENODE_GETNBDESCENDANTS(ZRTree *tree, ZRTreeNode *node)
{
	return ZRTREE_STRATEGY(tree)->ftreeNode_getNbDescendants(tree, node);
}

ZRMUSTINLINE
static inline ZRIterator* ZRTREENODE_GETCHILDS(ZRTree *tree, ZRTreeNode *node)
{
	return ZRTREE_STRATEGY(tree)->ftreeNode_getChilds(tree, node);
}

ZRMUSTINLINE
static inline ZRIterator* ZRTREENODE_GETASCENDANTS(ZRTree *tree, ZRTreeNode *node)
{
	return ZRTREE_STRATEGY(tree)->ftreeNode_getAscendants(tree, node);
}

ZRMUSTINLINE
static inline ZRIterator* ZRTREENODE_GETDESCENDANTS(ZRTree *tree, ZRTreeNode *node)
{
	return ZRTREE_STRATEGY(tree)->ftreeNode_getDescendants(tree, node);
}

ZRMUSTINLINE
static inline ZRIterator* ZRTREENODE_GETDESCENDANTS_BF(ZRTree *tree, ZRTreeNode *node)
{
	return ZRTREE_STRATEGY(tree)->ftreeNode_getDescendants_BF(tree, node);
}

ZRMUSTINLINE
static inline ZRIterator* ZRTREENODE_GETDESCENDANTS_DF(ZRTree *tree, ZRTreeNode *node)
{
	return ZRTREE_STRATEGY(tree)->ftreeNode_getDescendants_DF(tree, node);
}

// ============================================================================
// HELP
// ============================================================================

ZRMUSTINLINE
static inline ZRTreeNode* ZRTREENODE_GETTHEPARENT(ZRTree *tree, ZRTreeNode *node)
{
	ZRTreeNode *ret;
	ZRGRAPHNODE_GETNPARENTS(ZRTREE_GRAPH(tree), node, &ret, 0, 1);
	return ret;
}

ZRMUSTINLINE
static inline void ZRTREENODE_CPYTHEPARENTEDGE(ZRTree *tree, ZRTreeNode *node, ZRTreeEdge *edge)
{
	ZRGRAPHNODE_CPYNEDGES(ZRTREE_GRAPH(tree), node, edge, 0, 1, ZRGraphEdge_selectIN);
}

// ============================================================================

#include "TreeBuilder.h"

#endif
