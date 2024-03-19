#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

using namespace glm;

class Plane :
    public PhysicsObject
{
public:
    Plane();
    Plane(glm::vec2 normal, float distance);
    ~Plane();

    virtual void FixedUpdate(glm::vec2 a_Gravity, float a_TimeStep);
    virtual void Draw(float alpha) override;
    virtual void ResetPosition() {};

    void ResolveCollision(RigidBody* a_RigidBody, glm::vec2 a_Contact);

    vec2 GetNormal() { return m_Normal; }
    float GetDistance() { return m_DistanceToOrigin; }

protected:
    vec4 m_Colour;
    vec2 m_Normal;
    float m_DistanceToOrigin;
};

