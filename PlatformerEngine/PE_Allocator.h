#pragma once

#include "PE_Settings.h"

struct PE_Block;

/// @brief Structure représentant un bloc de mémoire.
/// Quand un bloc est attribué à l'utilisateur, il peut y ecrire ce qu'il souhaite.
/// Quand le bloc est disponible dans l'allocateur, ces premiers octets servent à sauvegarder
/// l'adresse du prochain bloc disponible.
struct PE_Block
{
    /// @brief Quand le bloc est disponible dans l'allocateur,
    /// m_next contient l'adresse du prochain bloc disponible.
    PE_Block *m_next;
};

/// @brief Structure représentant une zone mémoire contenant plusieurs blocs.
/// La taille d'un chunk est fixe et définie dans le code source.
struct PE_Chunk
{
    /// @brief La taille en octet des blocs.
    int m_blockSize;

    /// @brief Pointeur vers la zone de mémoire.
    /// Cette zone est découpée en plusieurs blocs de taille m_blockSize.
    /// Les blocs disponibles sont chaînés.
    void *m_data;
};

/// @brief Structure représentant un allocateur ne pouvant allouer qu'une seule taille fixée de mémoire.
class PE_FixedAllocator
{
public:
    PE_FixedAllocator(int blockSize);
    ~PE_FixedAllocator();

    void *Allocate();
    void Deallocate(void *ptr);

private:
    /// @brief Tableau contenant les adresse des différentes zones mémoires pouvant être attribuées à l'utilisateur.
    /// Chaque zone est elle-même découpée en plusieurs blocs de taille fixe.
    PE_Chunk *m_chunks;

    /// @brief Le nombre de zones mémoires de l'allocateur.
    int m_chunkCount;

    /// @brief Le nombre maximal de zones mémoires de l'allocateur avant un redimensionnement de m_chunks;
    int m_chunkCapacity;

    /// @brief Liste chaînée de tous les blocs disponibles dans l'allocateur.
    /// Ces blocs sont situés dans les différentes zones mémoires pointées dans m_chunks.
    PE_Block *m_freeList;

    /// @brief La taille des blocs alloués par l'allocateur.
    int m_blockSize;
};

class PE_Allocator
{
public:
    PE_Allocator();
    PE_Allocator(PE_Allocator const &) = delete;
    PE_Allocator &operator=(PE_Allocator const &) = delete;
    ~PE_Allocator();

    void *Allocate(int size);
    void Deallocate(void *ptr, int size);

private:
    PE_FixedAllocator **m_fixedAlloc;
    int m_blockSizeCount;
    int *m_sizeMap;
    int m_maxBlockSize;
};
