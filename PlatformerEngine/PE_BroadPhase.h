#pragma once

#include "PE_Allocator.h"
#include "PE_Collider.h"
#include "PE_DynamicTree.h"
#include "PE_Callback.h"

/// @brief Structure représenant une paire de proxys potentiellement en collision selon la broad phase.
/// Par construction, on a proxyIdA < proxyIdB pour éviter les doublons.
struct PE_ProxyPair
{
    /// @brief Identifiant du premier proxy.
    PE_Id proxyIdA;

    /// @brief Identifiant du second proxy.
    PE_Id proxyIdB;
};

/// @brief Structure représentant la broad phase du moteur physique.
/// Elle utilise en interne un arbre dynamique pour déterminer les colliders potentiellement en collisions.
/// L'insertion et la suppression d'un collider s'effectue en O(log(n))
/// où n est le nombre de colliders dans la broad phase.
/// La recherche des paires à une complexité en O(m log(n)) avec m le nombre de colliders
/// qui ont été déplacés.
struct PE_BroadPhase
{
    /// @brief Arbre dynamique utilisé par la broad phase.
    PE_DynamicTree *m_tree;

    /// @brief Nombre de proxys.
    int m_proxyCount;

    /// @brief Tableau contenant les identifiants des proxy qui ont été déplacés
    /// depuis la dernière mise à jour des paires.
    PE_Id* m_moveBuffer;

    /// @brief Capacité du tableau m_moveBuffer.
    int m_moveCapacity;

    /// @brief Nombre de proxys qui ont été déplacés depuis la dernière mise à jour
    /// des paires. Il s'agit de la taille du tableau m_moveBuffer.
    int m_moveCount;

    /// @brief Tableau contenant les dernières paires calculées.
    PE_ProxyPair* m_pairBuffer;

    /// @brief Capacité du tableau m_pairBuffer.
    int m_pairCapacity;

    /// @brief Nombre de paires. Il s'agit de la taille du tableau m_pairBuffer.
    int m_pairCount;
};

/// @brief Crée la broad phase du moteur physique.
/// @return Un pointeur vers la broad phase créée ou NULL en cas d'erreur.
PE_BroadPhase *PE_BroadPhase_New();

/// @brief Détruit la broad phase du moteur physique.
/// @param[in,out] bp la broad phase à détruire.
void PE_BroadPhase_Free(PE_BroadPhase *bp);

//.................................................................................................
// Proxy

/// @brief Crée un nouveau proxy dans la broad phase.
/// @param[in,out] bp la broad phase.
/// @param[in] aabb la boîte englobante du collider associé au proxy.
/// @param[in] proxy pointeur vers le proxy du collider qui doit être alloué.
/// @return L'identifiant du proxy dans la broad phase, ou PE_INVALID_ID en cas d'erreur.
PE_Id PE_BroadPhase_CreateProxy(PE_BroadPhase *bp, PE_AABB *aabb, PE_ColliderProxy *proxy);

/// @brief Supprime un proxy dans la broad phase.
/// @param[in,out] bp la broad phase.
/// @param[in] proxyId l'identifiant du proxy à supprimer.
void PE_BroadPhase_RemoveProxy(PE_BroadPhase *bp, PE_Id proxyId);

/// @brief Déplace un proxy dans la broad phase.
/// @param[in,out] bp la broad phase.
/// @param[in] proxyId l'identifiant du proxy à supprimer.
/// @param[in] aabb la boîte englobante du collider associé au proxy.
/// @param[in] displacement le vecteur de déplacement du proxy.
void PE_BroadPhase_MoveProxy(PE_BroadPhase *bp, PE_Id proxyId, PE_AABB *aabb, PE_Vec2 displacement);

/// @brief Renvoie la boîte englobante élargie associée à un proxy.
/// @param[in] bp la broad phase.
/// @param[in] proxyId l'identifiant du proxy à supprimer.
/// @return La boîte englobante élargie du proxy.
PE_AABB *PE_BroadPhase_GetFatAABB(PE_BroadPhase *bp, PE_Id proxyId);

/// @brief Renvoie le proxy associé à un identifiant dans la broad phase.
/// @param[in] bp la broad phase.
/// @param[in] proxyId l'identifiant du proxy à supprimer.
/// @return Le proxy associé à l'identifiant.
PE_ColliderProxy *PE_BroadPhase_GetProxy(PE_BroadPhase *bp, PE_Id proxyId);

//.................................................................................................
// Calcule des paires

/// @brief Calcule les nouvelles paires de proxy potentiellement en collision.
/// Cette fonction ne considère que les proxy qui ont été déplacés depuis le dernier appel.
/// La recherche des paires à une complexité en O(m log(n)) avec m le nombre de proxys
/// qui ont été déplacés et n le nombre total de proxys dans la broad phase.
/// On peut ensuite récupérer les paires avec les méthodes
///   PE_BroadPhase_GetPairCount() et
///   PE_BroadPhase_GetPairs()
/// 
/// @param[in,out] bp la broad phase.
void PE_BroadPhase_UpdatePairs(PE_BroadPhase *bp);

/// @brief Renvoie le nombre de nouvelles paires calculées lors du dernier appel à la méthode
/// PE_BroadPhase_UpdatePairs().
/// @param[in] bp la broad phase.
/// @return Le nombre de nouvelle paires.
inline int PE_BroadPhase_GetPairCount(PE_BroadPhase *bp)
{
    return bp->m_pairCount;
}

/// @brief Renvoie un pointeur vers le tableau des nouvelles paires calculées lors du dernier
/// appel à la méthode PE_BroadPhase_UpdatePairs().
/// Ce pointeur n'est accessible qu'en lecture.
/// @param[in] bp la broad phase.
/// @return Un pointeur vers le tableau des nouvelles paires.
inline PE_ProxyPair *PE_BroadPhase_GetPairs(PE_BroadPhase *bp)
{
    return bp->m_pairBuffer;
}

//.................................................................................................
// Tests et requêtes

inline bool PE_BroadPhase_Overlap(PE_BroadPhase *bp, PE_Id proxyIdA, PE_Id proxyIdB)
{
    PE_AABB *aabbA = PE_DynamicTree_GetFatAABB(bp->m_tree, proxyIdA);
    PE_AABB *aabbB = PE_DynamicTree_GetFatAABB(bp->m_tree, proxyIdB);
    return aabbA->Overlap(*aabbB);
}

void PE_BroadPhase_Query(PE_BroadPhase *bp, const PE_AABB &aabb, PE_QueryCallback &callback);
