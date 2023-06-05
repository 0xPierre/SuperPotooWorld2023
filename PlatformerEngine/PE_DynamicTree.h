#pragma once

#include "PE_Allocator.h"
#include "PE_Vector.h"
#include "PE_Collider.h"
#include "PE_Math.h"
#include "PE_IdStack.h"
#include "PE_Settings.h"

struct PE_BroadPhase;

struct PE_TreeNode
{
    /// @brief La boîte englobante du noeud.
    /// Dans le cas où le noeud est une feuille,
    /// elle est plus grande que la boîte englobante du collider associé.
    /// Cela permet de ne pas remettre à jour l'arbre quand le collider se déplace très peu.
    PE_AABB aabb;

    /// @brief Identifieur du noeud parent.
    PE_Id parent;

    /// @brief Indentifieur du prochain noeud libre dans la freeList.
    PE_Id next;

    /// @brief Identifieur du premier fils.
    PE_Id child1;

    /// @brief Identifieur du second fils.
    PE_Id child2;

    /// @brief La hauteur du noeud dans l'arbre.
    /// La racine est le noeud le plus haut.
    /// Les feuilles ont une hauteur de 0 et les noeuds libres (non inclus dans l'arbre)
    /// ont une hauteur de -1.
    int height;

    /// @brief booléen indiquant si le noeud à bougé depuis
    /// la dernière mise à jour des paires de la broad phase.
    bool moved;

    /// @brief Pointeur vers le proxy associé au noeud.
    /// Les feuilles de l'arbre sont les seuls noeuds pour lesquels ce pointeur est non nul.
    PE_ColliderProxy *proxy;
};

/// @brief Indique si un noeud est une feuille de l'arbre.
/// @param node le noeud.
/// @return true si le noeud est une feuille de l'arbre, false sinon.
inline bool PE_TreeNode_IsLeaf(PE_TreeNode *node)
{
    return (node->child1 == PE_INVALID_ID);
}

/// A dynamic AABB tree broad-phase, inspired by Nathanael Presson's btDbvt.
/// A dynamic tree arranges data in a binary tree to accelerate
/// queries such as volume queries and ray casts. Leafs are proxies
/// with an AABB. In the tree we expand the proxy AABB by b2_fatAABBFactor
/// so that the proxy AABB is bigger than the client object. This allows the client
/// object to move by small amounts without triggering a tree update.
///
/// Nodes are pooled and relocatable, so we use node indices rather than pointers.
struct PE_DynamicTree
{
    PE_Id m_root;
    PE_TreeNode *m_nodes;
    int m_nodeCount;
    int m_capacity;
    PE_Id m_freeList;

    PE_IdStack *m_stack;
    PE_IdStack *m_queryIds;
};

PE_DynamicTree *PE_DynamicTree_New();
void PE_DynamicTree_Free(PE_DynamicTree *tree);

PE_Id PE_DynamicTree_CreateProxy(PE_DynamicTree *tree, PE_AABB *aabb, PE_ColliderProxy *proxy);
void PE_DynamicTree_RemoveProxy(PE_DynamicTree *tree, PE_Id proxyId);
bool PE_DynamicTree_MoveProxy(PE_DynamicTree *tree, PE_Id proxyId, PE_AABB *aabb, PE_Vec2 displacement);

inline PE_ColliderProxy *PE_DynamicTree_GetProxy(PE_DynamicTree *tree, PE_Id proxyId)
{
    assert(0 <= proxyId && proxyId < tree->m_capacity);
    return tree->m_nodes[proxyId].proxy;
}

inline PE_AABB *PE_DynamicTree_GetFatAABB(PE_DynamicTree *tree, PE_Id proxyId)
{
    assert(0 <= proxyId && proxyId < tree->m_capacity);
    return &(tree->m_nodes[proxyId].aabb);
}

inline bool PE_DynamicTree_WasMoved(PE_DynamicTree *tree, PE_Id proxyId)
{
    assert(0 <= proxyId && proxyId < tree->m_capacity);
    return tree->m_nodes[proxyId].moved;
}

inline void PE_DynamicTree_ClearMoved(PE_DynamicTree *tree, PE_Id proxyId)
{
    assert(0 <= proxyId && proxyId < tree->m_capacity);
    tree->m_nodes[proxyId].moved = false;
}

inline int PE_DynamicTree_GetHeight(PE_DynamicTree *tree)
{
    if (tree->m_root == PE_INVALID_ID)
    {
        return 0;
    }

    return tree->m_nodes[tree->m_root].height;
}

struct PE_TreeQueryOutput
{
    PE_Id *ids;
    int idCount;
};

inline void PE_DynamicTree_Query(
    PE_DynamicTree *tree, const PE_AABB &aabb, PE_TreeQueryOutput *output)
{
    PE_IdStack *stack = tree->m_stack;
    PE_IdStack *queryIds = tree->m_queryIds;
    PE_IdStack_Clear(stack);
    PE_IdStack_Clear(queryIds);

    PE_IdStack_PushBack(stack, tree->m_root);
    while (PE_IdStack_GetSize(stack) > 0)
    {
        PE_Id nodeId = PE_IdStack_Pop(stack);
        if (nodeId == PE_INVALID_ID)
        {
            continue;
        }

        PE_TreeNode *node = tree->m_nodes + nodeId;

        if (node->aabb.Overlap(aabb))
        {
            if (PE_TreeNode_IsLeaf(node))
            {
                PE_IdStack_PushBack(queryIds, nodeId);
            }
            else
            {
                PE_IdStack_PushBack(stack, node->child1);
                PE_IdStack_PushBack(stack, node->child2);
            }
        }
    }

    output->idCount = PE_IdStack_GetSize(queryIds);
    output->ids = PE_IdStack_ToArray(queryIds);
}
