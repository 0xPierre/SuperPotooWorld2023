#pragma once

#include "PE_Allocator.h"

/// @brief An array with dynamic size.
struct PE_Vector
{
    /// @protected
    /// @brief The array of data.
    void *m_data;

    /// @protected
    /// @brief The maximum number of elements.
    int m_capacity;

    /// @protected
    /// @brief The number of elements.
    int m_size;

    /// @protected
    /// @brief The size of each element.
    int m_dataSize;
};

/// @brief Constructs a vector (dynamic array). 
/// @param[in] dataSize the number of byte for each object in the pool list.
/// @param[in] capacity the maximum number of objects in the list.
/// @return The vector.
PE_Vector *PE_Vector_New(int dataSize, int capacity);

/// @brief Destructs the vector.
/// Frees all objects.
/// If at least one element contains one external dynamic allocation,
/// you must free the memory before calling this function.
/// You must not dereference or free the list pointer after this call.
/// You should manually set the list pointer to NULL.
/// @param[in,out] vector this.
void PE_Vector_Free(PE_Vector *vector);

/// @brief Appends one element to the vector container.
/// The data is copied in this new element.
/// @param[in,out] vector this.
/// @param[in] data the data of the new element.
void PE_Vector_PushBack(PE_Vector *vector, void *data);

/// @brief Pops the last element of the vector.
/// @param[in,out] vector this.
inline void PE_Vector_PopBack(PE_Vector *vector)
{
    if (vector->m_size > 0)
        vector->m_size--;
}

void PE_Vector_Pop(PE_Vector *vector, int index);

/// @brief Returns the element at the specified position in this vector.
/// @param[in] vector this.
/// @param[in] index the index of the element to return.
/// @return The element at the specified position.
inline void *PE_Vector_Get(PE_Vector *vector, int index)
{
    return (void *)((char *)vector->m_data + (size_t)index * vector->m_dataSize);
}

inline void PE_Vector_GetCopy(PE_Vector *vector, int index, void *dst)
{
    void *pointer = PE_Vector_Get(vector, index);
    int dataSize = vector->m_dataSize;
    PE_Memcpy(dst, dataSize, pointer, dataSize);
}

/// @brief Sets the element at the specified position in this vector.
/// @param[in,out] vector this.
/// @param[in] index the index of the element
/// @param[in] data the data of the element.
void PE_Vectoret(PE_Vector *vector, int index, void *data);

/// @brief Clears the vector.
/// @param[in,out] vector this.
inline void PE_Vector_Clear(PE_Vector *vector)
{
    vector->m_size = 0;
}

/// @brief Returns pointer to the underlying array serving as element storage.
/// @param[in] vector this.
/// @return A pointer to the elements in the vector.
inline void *PE_Vector_ToArray(PE_Vector *vector)
{
    return vector->m_data;
}

/// @brief Returns the number of elements in the vector container.
/// @param[in] vector this.
/// @return The number of elements.
inline int PE_Vector_GetSize(PE_Vector *vector)
{
    return vector->m_size;
}

/// @brief Returns the maximum number of elements in the vector container.
/// @param[in] vector this.
/// @return The maximum number of elements.
inline int PE_Vector_GetCapacity(PE_Vector *vector)
{
    return vector->m_capacity;
}
