#pragma once
#include "PhysicsObject.h"
class RigidBody :
    public PhysicsObject
{
public:
    RigidBody() {};
    RigidBody(ShapeType a_ShapeType);
    RigidBody(ShapeType a_ShapeType, glm::vec2 position,
        glm::vec2 velocity, float Rotation, float mass, float elasticity);
    ~RigidBody() {};

    virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

    void ApplyForce(glm::vec2 force, glm::vec2* a_ImpulsePosition = nullptr);
    void ApplyForceToActor(RigidBody* actor2, glm::vec2 force);
    void ResolveCollision(RigidBody* a_Actor, glm::vec2 a_Contact, glm::vec2* a_CollisionNormal = nullptr);

public:
    glm::vec2 GetPosition() { return m_Position; }
    void SetPosition(glm::vec2 a_NewPosition) { m_Position = a_NewPosition; }

    glm::vec2 GetVelocity() { return m_Velocity; }
    void SetVelocity(glm::vec2 a_Velocity = glm::vec2()) { m_Velocity = a_Velocity; }

    float GetElasticity() { return m_Elasticity; }
    void  SetElasticity(float a_Elasticity) { m_Elasticity = glm::clamp(a_Elasticity, 0.f, 1.f); }

    float GetMass() { return m_Mass; }
    void  SetMass(float a_NewMass) { m_Mass = a_NewMass; }

    float GetRotation() { return m_Rotation; }
    void  SetRotation(float a_NewRotation) { m_Rotation = a_NewRotation; }

    float GetAngularVelocity() { return m_AngularVelocity; }
    void  SetAngularVelocity(float a_NewAngularVelocity) { m_AngularVelocity = a_NewAngularVelocity; }

    virtual float GetMoment() { return m_Moment; }

protected:
    glm::vec2 m_Position = glm::vec2();
    glm::vec2 m_Velocity = glm::vec2();
    float m_Elasticity;
    float m_Mass;
    float m_Rotation; // In Radians
    float m_AngularVelocity = 0.f;
    float m_Moment;
};

