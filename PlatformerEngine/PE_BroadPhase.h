#pragma once

#include "PE_Allocator.h"
#include "PE_Collider.h"
#include "PE_DynamicTree.h"
#include "PE_Callback.h"

/// @brief Structure repr�senant une paire de proxys potentiellement en collision selon la broad phase.
/// Par construction, on a proxyIdA < proxyIdB pour �viter les doublons.
struct PE_ProxyPair
{
    /// @brief Identifiant du premier proxy.
    PE_Id proxyIdA;

    /// @brief Identifiant du second proxy.
    PE_Id proxyIdB;
};

/// @brief Structure repr�sentant la broad phase du moteur physique.
/// Elle utilise en interne un arbre dynamique pour d�terminer les colliders potentiellement en collisions.
/// L'insertion et la suppression d'un collider s'effectue en O(log(n))
/// o� n est le nombre de colliders dans la broad phase.
/// La recherche des paires � une complexit� en O(m log(n)) avec m le nombre de colliders
/// qui ont �t� d�plac�s.
struct PE_BroadPhase
{
    /// @brief Arbre dynamique utilis� par la broad phase.
    PE_DynamicTree *m_tree;

    /// @brief Nombre de proxys.
    int m_proxyCount;

    /// @brief Tableau contenant les identifiants des proxy qui ont �t� d�plac�s
    /// depuis la derni�re mise � jour des paires.
    PE_Id* m_moveBuffer;

    /// @brief Capacit� du tableau m_moveBuffer.
    int m_moveCapacity;

    /// @brief Nombre de proxys qui ont �t� d�plac�s depuis la derni�re mise � jour
    /// des paires. Il s'agit de la taille du tableau m_moveBuffer.
    int m_moveCount;

    /// @brief Tableau contenant les derni�res paires calcul�es.
    PE_ProxyPair* m_pairBuffer;

    /// @brief Capacit� du tableau m_pairBuffer.
    int m_pairCapacity;

    /// @brief Nombre de paires. Il s'agit de la taille du tableau m_pairBuffer.
    int m_pairCount;
};

/// @brief Cr�e la broad phase du moteur physique.
/// @return Un pointeur vers la broad phase cr��e ou NULL en cas d'erreur.
PE_BroadPhase *PE_BroadPhase_New();

/// @brief D�truit la broad phase du moteur physique.
/// @param[in,out] bp la broad phase � d�truire.
void PE_BroadPhase_Free(PE_BroadPhase *bp);

//.................................................................................................
// Proxy

/// @brief Cr�e un nouveau proxy dans la broad phase.
/// @param[in,out] bp la broad phase.
/// @param[in] aabb la bo�te englobante du collider associ� au proxy.
/// @param[in] proxy pointeur vers le proxy du collider qui doit �tre allou�.
/// @return L'identifiant du proxy dans la broad phase, ou PE_INVALID_ID en cas d'erreur.
PE_Id PE_BroadPhase_CreateProxy(PE_BroadPhase *bp, PE_AABB *aabb, PE_ColliderProxy *proxy);

/// @brief Supprime un proxy dans la broad phase.
/// @param[in,out] bp la broad phase.
/// @param[in] proxyId l'identifiant du proxy � supprimer.
void PE_BroadPhase_RemoveProxy(PE_BroadPhase *bp, PE_Id proxyId);

/// @brief D�place un proxy dans la broad phase.
/// @param[in,out] bp la broad phase.
/// @param[in] proxyId l'identifiant du proxy � supprimer.
/// @param[in] aabb la bo�te englobante du collider associ� au proxy.
/// @param[in] displacement le vecteur de d�placement du proxy.
void PE_BroadPhase_MoveProxy(PE_BroadPhase *bp, PE_Id proxyId, PE_AABB *aabb, PE_Vec2 displacement);

/// @brief Renvoie la bo�te englobante �largie associ�e � un proxy.
/// @param[in] bp la broad phase.
/// @param[in] proxyId l'identifiant du proxy � supprimer.
/// @return La bo�te englobante �largie du proxy.
PE_AABB *PE_BroadPhase_GetFatAABB(PE_BroadPhase *bp, PE_Id proxyId);

/// @brief Renvoie le proxy associ� � un identifiant dans la broad phase.
/// @param[in] bp la broad phase.
/// @param[in] proxyId l'identifiant du proxy � supprimer.
/// @return Le proxy associ� � l'identifiant.
PE_ColliderProxy *PE_BroadPhase_GetProxy(PE_BroadPhase *bp, PE_Id proxyId);

//.................................................................................................
// Calcule des paires

/// @brief Calcule les nouvelles paires de proxy potentiellement en collision.
/// Cette fonction ne consid�re que les proxy qui ont �t� d�plac�s depuis le dernier appel.
/// La recherche des paires � une complexit� en O(m log(n)) avec m le nombre de proxys
/// qui ont �t� d�plac�s et n le nombre total de proxys dans la broad phase.
/// On peut ensuite r�cup�rer les paires avec les m�thodes
///   PE_BroadPhase_GetPairCount() et
///   PE_BroadPhase_GetPairs()
/// 
/// @param[in,out] bp la broad phase.
void PE_BroadPhase_UpdatePairs(PE_BroadPhase *bp);

/// @brief Renvoie le nombre de nouvelles paires calcul�es lors du dernier appel � la m�thode
/// PE_BroadPhase_UpdatePairs().
/// @param[in] bp la broad phase.
/// @return Le nombre de nouvelle paires.
inline int PE_BroadPhase_GetPairCount(PE_BroadPhase *bp)
{
    return bp->m_pairCount;
}

/// @brief Renvoie un pointeur vers le tableau des nouvelles paires calcul�es lors du dernier
/// appel � la m�thode PE_BroadPhase_UpdatePairs().
/// Ce pointeur n'est accessible qu'en lecture.
/// @param[in] bp la broad phase.
/// @return Un pointeur vers le tableau des nouvelles paires.
inline PE_ProxyPair *PE_BroadPhase_GetPairs(PE_BroadPhase *bp)
{
    return bp->m_pairBuffer;
}

//.................................................................................................
// Tests et requ�tes

inline bool PE_BroadPhase_Overlap(PE_BroadPhase *bp, PE_Id proxyIdA, PE_Id proxyIdB)
{
    PE_AABB *aabbA = PE_DynamicTree_GetFatAABB(bp->m_tree, proxyIdA);
    PE_AABB *aabbB = PE_DynamicTree_GetFatAABB(bp->m_tree, proxyIdB);
    return aabbA->Overlap(*aabbB);
}

void PE_BroadPhase_Query(PE_BroadPhase *bp, const PE_AABB &aabb, PE_QueryCallback &callback);
