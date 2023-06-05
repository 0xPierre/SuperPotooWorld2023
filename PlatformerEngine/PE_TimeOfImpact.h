#pragma once

#include "PE_Math.h"
#include "PE_Shape.h"
#include "PE_ShapeDistance.h"

struct PE_TOIInput
{
    PE_Shape *shapeA;
    PE_Shape *shapeB;
    PE_Sweep sweepA;
    PE_Sweep sweepB;
};

enum PE_TOIState
{
    PE_TOI_FAILED,
    PE_TOI_WILL_TOUCH,
    PE_TOI_OVERLAPPING,
    PE_TOI_SEPARATED
};

struct PE_TOIOutput
{
    int state;
    float beta;
    int iterCount;
};

void PE_TimeOfImpact(PE_TOIInput *input, PE_SimplexCache *cache, PE_TOIOutput *output);
