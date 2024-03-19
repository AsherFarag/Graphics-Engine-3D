#pragma once
#include "RigidBody.h"
class PlaneRB :
    public RigidBody
{
public:
    PlaneRB(Transform* a_Owner, ECollisionType a_CollisionType = DEFAULT_COLLISION_TYPE, float a_HalfLength = 10.f,vec4 a_Colour = vec4(1), float a_Mass = 1.f, float a_Elasticity = 1.f, vec2 a_Velocity = vec2(), float a_AngularVelocity = 0.f);
    virtual ~PlaneRB() override {};

    virtual void Update(float DeltaTime) override {}
    virtual void Draw(float a_Alpha) override;

    virtual void ResolveCollision(RigidBody* a_RigidBody, vec2 a_Contact, vec2* a_CollisionNormal = nullptr, float a_Penetration = 0.f) override;

    float m_HalfLength;
};

