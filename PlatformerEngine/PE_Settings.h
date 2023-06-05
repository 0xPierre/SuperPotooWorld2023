#pragma once

#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

//.................................................................................................
// Fonctions différentes window/linux

inline void PE_Memcpy(
    void *destination, long long destinationSize,
    void *source, long long sourceSize)
{
#ifdef _WIN32
    memcpy_s(destination, destinationSize, source, sourceSize);
#else
    memcpy(destination, source, sourceSize);
#endif
}

//.................................................................................................
// Constantes

#define PE_DOT_05 0.996194698091746f
#define PE_DOT_10 0.984807753012208f
#define PE_DOT_15 0.965925826289068f
#define PE_DOT_20 0.939692620785909f
#define PE_DOT_25 0.90630778703665f
#define PE_DOT_30 0.866025403784439f
#define PE_DOT_35 0.819152044288992f
#define PE_DOT_40 0.766044443118978f
#define PE_DOT_45 0.707106781186548f
#define PE_DOT_50 0.642787609686539f
#define PE_DOT_55 0.573576436351046f
#define PE_DOT_60 0.5f
#define PE_DOT_65 0.4226182617407f
#define PE_DOT_70 0.342020143325669f
#define PE_DOT_75 0.258819045102521f
#define PE_DOT_80 0.17364817766693f
#define PE_DOT_85 0.087155742747658f

#define PE_TIME_TO_SLEEP          0.25f
#define PE_LINEAR_SLEEP_TOLERANCE 0.05f
#define PE_SKIN_RADIUS            0.005f
#define PE_SOLVER_EPSILON         0.001f
#define PE_SOLVER_DOT_UP          PE_DOT_85
#define PE_FRICTION_SCALE         1.0f

#define PE_MAX_POLYGON_VERTICES 8
#define PE_CPAIR_MAX_ITER_COUNT 16

#define PE_RAD_TO_DEG 57.2957795130823f

//.................................................................................................
// Types

typedef unsigned char  uint8;
typedef signed short   sint16;
typedef unsigned short uint16;
typedef signed int     sint32;
typedef unsigned int   uint32;

typedef int PE_Id;

/// @brief Equivalent du pointeur null pour les ID sur 32 bits
#define PE_INVALID_ID (-1)

//.................................................................................................
// Debug et informations

#define PE_AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); }}

//#define PE_DEBUG_LIST
//#define PE_DEBUG_DYNAMIC_TREE
//#define PE_PROFILE
