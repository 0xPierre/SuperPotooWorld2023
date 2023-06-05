#pragma once

#include "PE_Settings.h"
#include "PE_Math.h"

class PE_CollisionPair;
class PE_Collider;

class PE_CollisionListener
{
public:
    virtual void OnCollisionEnter(PE_CollisionPair &collision);
    virtual void OnCollisionStay(PE_CollisionPair &collision);
    virtual void OnCollisionExit(PE_CollisionPair &collision);
};

class PE_QueryCallback
{
public:
    virtual ~PE_QueryCallback() {}
    virtual bool ReportCollider(PE_Collider *collider) = 0;
};

struct PE_RayCastHit
{
    PE_Vec2 normal;
    PE_Vec2 hitPoint;
    float fraction;
};

class PE_RayCastCallback
{
public:
    virtual ~PE_RayCastCallback() {}
    virtual bool ReportCollider(PE_Collider *collider, const PE_RayCastHit &hit) = 0;
};
