#pragma once
#include "RigidBody.h"
class CircleRB :
    public RigidBody
{
protected:
    CircleRB() {};
public:
    CircleRB(Transform* a_Owner, ECollisionType a_CollisionType = DEFAULT_COLLISION_TYPE, float a_Radius = 1.f, vec4 a_Colour = DEFAULT_COLOUR, float a_Mass = DEFAULT_MASS, float a_Elasticity = DEFAULT_ELASTICITY, vec2 a_Velocity = DEFAULT_VELOCITY, float a_AngularVelocity = DEFAULT_ANGULAR_VELOCITY);
    virtual ~CircleRB() override {};

    virtual void Update(float DeltaTime) override;
    virtual void Draw(float a_Alpha) override;

    float GetRadius() { return m_Radius; }

    virtual void CalculateMoment() override;
    virtual bool IsInside(vec2 a_Point) override;

    void SetDrawSegments(int a_DrawSegments) { m_DrawSegments = a_DrawSegments; }

protected:
    float m_Radius;
    int m_DrawSegments = 12;
};

