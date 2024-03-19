#pragma once
#include "RigidBody.h"

class BoxRB :
    public RigidBody
{
public:
    BoxRB(Transform* a_Owner, ECollisionType a_CollisionType = DEFAULT_COLLISION_TYPE, vec2 a_Extents = vec2(10), vec4 a_Colour = DEFAULT_COLOUR, float a_Mass = DEFAULT_MASS, float a_Elasticity = DEFAULT_ELASTICITY, vec2 a_Velocity = DEFAULT_VELOCITY, float a_AngularVelocity = DEFAULT_ANGULAR_VELOCITY);
    virtual ~BoxRB() override {} 

    virtual void Update(float DeltaTime) override;
    virtual void Draw(float a_Alpha) override;

    vec2 GetExtents() { return m_Extents; }
    float GetWidth() { return m_Extents.x * 2.f; }
    float GetHeight() { return m_Extents.y * 2.f; }

    virtual void CalculateMoment() override;
    virtual bool IsInside(vec2 a_Point) override;

    bool CheckBoxCorners(BoxRB * a_Box, vec2 & a_Contact, int & a_NumContacts, float & a_Penetration, vec2 & a_EdgeNormal);

protected:
    vec2 m_Extents;  // Halfedge Lengths
};

