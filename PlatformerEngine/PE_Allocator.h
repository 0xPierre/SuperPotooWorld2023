#pragma once

#include "PE_Settings.h"

struct PE_Block;

/// @brief Structure repr�sentant un bloc de m�moire.
/// Quand un bloc est attribu� � l'utilisateur, il peut y ecrire ce qu'il souhaite.
/// Quand le bloc est disponible dans l'allocateur, ces premiers octets servent � sauvegarder
/// l'adresse du prochain bloc disponible.
struct PE_Block
{
    /// @brief Quand le bloc est disponible dans l'allocateur,
    /// m_next contient l'adresse du prochain bloc disponible.
    PE_Block *m_next;
};

/// @brief Structure repr�sentant une zone m�moire contenant plusieurs blocs.
/// La taille d'un chunk est fixe et d�finie dans le code source.
struct PE_Chunk
{
    /// @brief La taille en octet des blocs.
    int m_blockSize;

    /// @brief Pointeur vers la zone de m�moire.
    /// Cette zone est d�coup�e en plusieurs blocs de taille m_blockSize.
    /// Les blocs disponibles sont cha�n�s.
    void *m_data;
};

/// @brief Structure repr�sentant un allocateur ne pouvant allouer qu'une seule taille fix�e de m�moire.
class PE_FixedAllocator
{
public:
    PE_FixedAllocator(int blockSize);
    ~PE_FixedAllocator();

    void *Allocate();
    void Deallocate(void *ptr);

private:
    /// @brief Tableau contenant les adresse des diff�rentes zones m�moires pouvant �tre attribu�es � l'utilisateur.
    /// Chaque zone est elle-m�me d�coup�e en plusieurs blocs de taille fixe.
    PE_Chunk *m_chunks;

    /// @brief Le nombre de zones m�moires de l'allocateur.
    int m_chunkCount;

    /// @brief Le nombre maximal de zones m�moires de l'allocateur avant un redimensionnement de m_chunks;
    int m_chunkCapacity;

    /// @brief Liste cha�n�e de tous les blocs disponibles dans l'allocateur.
    /// Ces blocs sont situ�s dans les diff�rentes zones m�moires point�es dans m_chunks.
    PE_Block *m_freeList;

    /// @brief La taille des blocs allou�s par l'allocateur.
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
