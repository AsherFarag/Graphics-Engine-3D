#pragma once
#include "Component.h"
#include "glm/ext.hpp"
#include <functional>
#include <list>

class RigidBody;

class Collider :
    public Component
{
public:
    enum EShapeType
    {
        JOINT = -1,
        PLANE = 0,
        CIRCLE,
        BOX
    };

public:
    Collider(EShapeType a_ShapeID);
    virtual ~Collider() override;

    virtual void CalculateMoment() {};
    virtual void ResolveCollision(Collider* a_Collider, vec2 a_Contact, vec2* a_CollisionNormal = nullptr, float a_Penetration = 0.f);

protected:
#pragma region Collision Callbacks
    // Called when a collider collides with this collider. 
    std::function<void(Collider*)> CollisionCallBack;
    // Called ONLY when a collider enters this collider. 
    std::function<void(Collider*)> TriggerEnterCallBack;
    // Called ONLY when a collider leaves this collider. 
    std::function<void(Collider*)> TriggerExitCallBack;

    void TriggerEnter(Collider* a_RigidBody);
    void TriggerExit(Collider* a_RigidBody);

    std::list<Collider*> m_CollidersInside;
    std::list<Collider*> m_CollidersInsideThisFrame;
#pragma endregion

    // The Rigidbody this Collider affects. Multiple Colliders can reference the same RigidBody
    RigidBody* m_AttachedRigidBody;

    const EShapeType m_ShapeID;

    // Specify if this collider is configured as a trigger.
    bool  m_IsTrigger;

    float m_Moment;
    float m_Elasticity;

};

