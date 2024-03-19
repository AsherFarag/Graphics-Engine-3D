#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class SpringRB :
    public PhysicsObject
{
public:
    SpringRB(Transform* a_Owner, RigidBody* a_Body1, RigidBody* a_Body2, float a_SpringCoefficient, float a_Damping, vec4 a_Colour = DEFAULT_COLOUR, float a_RestLength = 0.f, vec2 a_Contact1 = vec2(), vec2 a_Contact2 = vec2());
    virtual ~SpringRB() override = default;

    virtual void Update(float DeltaTime) override;
    virtual void Draw(float a_Alpha) override;

    vec2 GetContact1(float a_Alpha = 1.f) { return m_Body1 ? m_Body1->ToWorld(m_Contact1, a_Alpha) : m_Contact1; }
    vec2 GetContact2(float a_Alpha = 1.f) { return m_Body2 ? m_Body2->ToWorld(m_Contact2, a_Alpha) : m_Contact2; }

protected:
    RigidBody* m_Body1;
    RigidBody* m_Body2;

    vec2 m_Contact1;
    vec2 m_Contact2;
           
    vec4 m_Color;

    float m_Damping;
    float m_RestLength;
    float m_SpringCoefficient;
};

