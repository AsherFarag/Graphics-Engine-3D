#pragma once
#include "PhysicsObject.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
#include <functional>
#include <list>

#include "AABB.h"

#define DEFAULT_COLLISION_TYPE DYNAMIC
#define DEFAULT_MASS 1.f
#define DEFAULT_ELASTICITY 0.3f
#define DEFAULT_VELOCITY vec2()
#define DEFAULT_ANGULAR_VELOCITY 0.f
#define DEFAULT_LINEAR_DRAG vec2(0)  // Between 0 and 1
#define DEFAULT_ANGULAR_DRAG 0.5f    // Between 0 and 1
#define DEFAULT_FRICTION 0.95f
#define DEFAULT_STATIC_FRICTION 1.f

#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ANGULAR_THRESHOLD 0.05f

const int SHAPE_COUNT = 3;

class RigidBody :
    public PhysicsObject
{
public:
    enum ECollisionType
    {
        STATIC = 0,
        DYNAMIC,
        TRIGGER
    };

    enum EShapeType
    {
        JOINT = -1,
        PLANE = 0,
        CIRCLE,
        BOX
    };
protected:
    RigidBody() {};

public:
    RigidBody(Transform* a_Owner, EShapeType a_ShapeID, ECollisionType a_CollisionType = DEFAULT_COLLISION_TYPE, float a_Mass = DEFAULT_MASS, float a_Elasticity = DEFAULT_ELASTICITY, vec2 a_Velocity = DEFAULT_VELOCITY, float a_AngularVelocity = DEFAULT_ANGULAR_VELOCITY, vec4 a_Colour = DEFAULT_COLOUR);
    virtual ~RigidBody() override;

    virtual void Update(float DeltaTime) override;

    void CalculateSmoothedPosition(float a_Alpha);
    virtual void CalculateMoment() {};

    void ApplyForce(vec2 force, vec2 a_ImpulsePosition = vec2());
    virtual void ResolveCollision(RigidBody* a_Actor, vec2 a_Contact, vec2* a_CollisionNormal = nullptr, float a_Penetration = 0.f);

#pragma region Getters & Setters

public:

    ECollisionType GetCollisionType() { return m_CollisionType; }
    void SetCollisionType(ECollisionType a_NewCollisionType) { m_CollisionType = a_NewCollisionType; }

    vec2  GetPosition() { return m_OwnerTransform->GetPosition(); }
    void  SetPosition(vec2 a_NewPosition = vec2()) { m_OwnerTransform->SetPosition(a_NewPosition); }

    vec2 ToWorld(vec2 a_Position, float a_Alpha) { return GetPosition() + GetLocalX() * a_Position.x + GetLocalY() * a_Position.y; }

    float GetRotation() { return m_OwnerTransform->GetRotation(); }
    void  GetRotation(float a_NewRotation) { m_OwnerTransform->SetRotation(a_NewRotation); }

    vec2  GetVelocity() { return m_Velocity; }
    void  SetVelocity(vec2 a_Velocity = vec2()) { m_Velocity = a_Velocity; }

    float GetElasticity() { return m_Elasticity; }
    void  SetElasticity(float a_Elasticity) { m_Elasticity = a_Elasticity; }

    float GetMass() { return m_CollisionType == STATIC ? INT_MAX : m_Mass; }
    void  SetMass(float a_NewMass) { m_Mass = a_NewMass; CalculateMoment(); }

    float GetAngularVelocity() { return m_AngularVelocity; }
    void  SetAngularVelocity(float a_NewAngularVelocity = 0.f) { m_AngularVelocity = a_NewAngularVelocity; }

    vec2  GetLinearDrag() { return m_LinearDrag; }
    void  SetLinearDrag(vec2 a_NewDrag) { m_LinearDrag.x = clamp(a_NewDrag.x, 0.f, 1.f); m_LinearDrag.y = clamp(a_NewDrag.y, 0.f, 1.f); }

    float GetAngularDrag() { return m_AngularDrag; }
    void  SetAngularDrag(float a_NewDrag) { m_AngularDrag = clamp(a_NewDrag, 0.f, 1.f);; }

    float GetPotentialEnergy() { return -m_Mass * dot(World::GetPhysicsManager()->GetGravity(), GetPosition()); }
    float GetKineticEnergy() { return 0.5f * (m_Mass * dot(m_Velocity, m_Velocity) + GetMoment() * m_AngularVelocity * m_AngularVelocity); }
    float GetEnergy() { return GetPotentialEnergy() + GetKineticEnergy(); }

    float GetDynamicFrictionCoefficient() { return m_FrictionCoefficient; }
    float GetStaticFrictionCoefficient() { return m_StaticFrictionCoefficient; }
    void SetDynamicFrictionCoefficient(float a_New) { m_FrictionCoefficient = a_New; }
    void SetStaticFrictionCoefficient(float a_New) { m_StaticFrictionCoefficient = a_New; }

    void  SetDebugDraw(bool a_Draw);

    //AABB* GetBoundingBox() { return &m_AABB; };

    float GetMoment() { return m_CollisionType == STATIC ? INT_MAX : m_Moment; }
    vec2  GetNormal() { return GetTransform()->GetNormal(); }

    bool  IsStatic()  { return m_CollisionType == STATIC; }
    bool  IsDynamic() { return m_CollisionType == DYNAMIC; }
    bool  IsTrigger() { return m_CollisionType == TRIGGER; }

    virtual bool IsInside(vec2 a_Point) { return false; };

#pragma endregion

#pragma region Member Variables
public:
    ECollisionType m_CollisionType;
    EShapeType m_ShapeID;

    std::function<void(RigidBody*)> TriggerEnterCallBack;
    std::function<void(RigidBody*)> TriggerExitCallBack;

    void TriggerEnter(RigidBody* a_RigidBody);
    void TriggerExit(RigidBody* a_RigidBody);

    std::function<void(RigidBody*)> CollisionCallBack;

    std::list<RigidBody*> m_ObjectsInside;
    std::list<RigidBody*> m_ObjectsInsideThisFrame;

protected:
    //AABB  m_AABB;
    //virtual void CalculateBoundingBox() { m_AABB = AABB(); };

    vec2  m_Velocity;
    float m_AngularVelocity;

    vec2  m_LinearDrag;
    float m_AngularDrag;

    float m_Mass;
    float m_Moment;
    float m_Elasticity;

    float m_FrictionCoefficient = DEFAULT_FRICTION;
    float m_StaticFrictionCoefficient = DEFAULT_STATIC_FRICTION;

    vec2  m_LastPosition;
    float m_LastRotation;

    vec2  m_SmoothedPosition;
    vec2  m_SmoothedLocalX;
    vec2  m_SmoothedLocalY;

    bool  m_DebugDraw;

#pragma endregion
};

