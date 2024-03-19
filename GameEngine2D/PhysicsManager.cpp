#include "PhysicsManager.h"

#include "RigidBody.h"
#include "AABB.h"
#include "CircleRB.h"
#include "PlaneRB.h"
#include "BoxRB.h"

#include "glm/ext.hpp"

#include "iostream"

// function pointer array for doing our collisions
typedef bool(*fn)(RigidBody*, RigidBody*);
static const fn collisionFunctionArray[]
{
    PhysicsManager::PlaneToPlane,  PhysicsManager::PlaneToCircle,  PhysicsManager::PlaneToBox,
    PhysicsManager::CircleToPlane, PhysicsManager::CircleToCircle, PhysicsManager::CircleToBox,
    PhysicsManager::BoxToPlane,    PhysicsManager::BoxToCircle,    PhysicsManager::BoxToBox
};

PhysicsManager::PhysicsManager() {}

void PhysicsManager::Init()
{
}

void PhysicsManager::Update(float DeltaTime)
{
    // update physics at a fixed time step
    static float AccumulatedTime = 0.0f;
    AccumulatedTime += DeltaTime;

    while (AccumulatedTime >= m_TimeStep)
    {
        // Iterate backwards to handle errors when objects are deleted
        for (int i = m_PhysicsObjects.size() - 1; i >= 0; --i)
        {
            m_PhysicsObjects[i]->Update(m_TimeStep);
        }

        AccumulatedTime -= m_TimeStep;

        // Check for Collisions

        int ActorCount = m_RigidBodies.size();
        for (int Outer = 0; Outer < ActorCount - 1; Outer++)
        {
            for (int Inner = Outer + 1; Inner < ActorCount; Inner++)
            {
                RigidBody* object1 = m_RigidBodies[Outer];
                RigidBody* object2 = m_RigidBodies[Inner];
                int shapeId1 = object1->m_ShapeID;
                int shapeId2 = object2->m_ShapeID;

                // If one of the shapes are a joint,
                // Do not check for collisions
                if (shapeId1 < 0 || shapeId2 < 0)
                    continue;

                int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
                fn collisionFunctionPtr = collisionFunctionArray[functionIdx];

                if (collisionFunctionPtr != nullptr)
                {
                    collisionFunctionPtr(object1, object2);
                }
            }
        }
    }
}

void PhysicsManager::Draw()
{
    // Iterate backwards to handle errors when objects are deleted
    for (int i = m_PhysicsObjects.size() - 1; i >= 0; --i)
    {
        m_PhysicsObjects[i]->Draw(1);
    }
}

void PhysicsManager::AddPhysicsObject(PhysicsObject* a_Component)
{
    m_PhysicsObjects.push_back(a_Component);
}

void PhysicsManager::RemovePhysicsObject(PhysicsObject* a_Component)
{
    m_PhysicsObjects.erase(std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), a_Component));
}

void PhysicsManager::AddRigidBody(RigidBody* a_Component)
{
    m_RigidBodies.push_back(a_Component);
}

void PhysicsManager::RemoveRigidBody(RigidBody* a_Component)
{
    m_RigidBodies.erase(std::find(m_RigidBodies.begin(), m_RigidBodies.end(), a_Component));
}

void PhysicsManager::ApplyContactForces(RigidBody* a_RB1, RigidBody* a_RB2, vec2 a_Normal, float Penetration)
{
    float RB2Mass = a_RB2 ? a_RB2->GetMass() : INT_MAX;
    float RB1Factor = RB2Mass / (a_RB1->GetMass() + RB2Mass);

    if (a_RB1->GetCollisionType() == RigidBody::DYNAMIC)
        a_RB1->SetPosition(a_RB1->GetPosition() - RB1Factor * a_Normal * Penetration);

    if (a_RB2 == nullptr)
        return;
    if(a_RB2->GetCollisionType() == RigidBody::DYNAMIC)
       a_RB2->SetPosition(a_RB2->GetPosition() + (1 - RB1Factor) * a_Normal * Penetration);
}

float PhysicsManager::GetTotalEnergy()
{
    float Total = 0;
    for (auto it = m_RigidBodies.begin(); it != m_RigidBodies.end(); it++)
    {
        RigidBody* RB = *it;
        Total += RB->GetEnergy();
    }
    return Total;
}

bool PhysicsManager::PlaneToPlane(RigidBody* a_Plane1, RigidBody* a_Plane2)
{
    return false;
}

bool PhysicsManager::PlaneToCircle(RigidBody* a_Plane, RigidBody* a_Circle)
{
    return CircleToPlane(a_Circle, a_Plane);
}

bool PhysicsManager::PlaneToBox(RigidBody* a_Plane, RigidBody* a_Box)
{
    // TODO : Fix Error when on opposite side of Plane Normal

    PlaneRB* Plane = dynamic_cast<PlaneRB*>(a_Plane);
    BoxRB* Box = dynamic_cast<BoxRB*>(a_Box);

    if (Box == nullptr || Plane == nullptr)
        return false;

    int NumContacts = 0;
    vec2 Contact = vec2(0);
    float ContactV = 0.f;

    // Get a representative point on the plane
    vec2 PlaneOrigin = Plane->GetPosition();
    vec2 PlaneNormal = Plane->GetNormal();

    for (float x = -Box->GetExtents().x; x < Box->GetWidth(); x += Box->GetWidth())
    {
        for (float y = -Box->GetExtents().y; y < Box->GetHeight(); y += Box->GetHeight())
        {
            // Get the position of the corner in world space
            vec2 Point = Box->GetPosition() + x * Box->GetLocalX() + y * Box->GetLocalY();

            float PointDistanceFromPlane = dot(Point - PlaneOrigin, PlaneNormal);
            //float PlaneDistanceFromOrigin = dot(PlaneOrigin, PlaneNormal);
            //PointDistanceFromPlane -= PlaneDistanceFromOrigin;
            //PointDistanceFromPlane = abs(PointDistanceFromPlane);

            // this is the total velocity of the point in world space
            vec2 displacement = x * Box->GetLocalX() + y * Box->GetLocalY();
            vec2 pointVelocity = Box->GetVelocity() + Box->GetAngularVelocity() * vec2(-displacement.y, displacement.x);
            // and this is the component of the point velocity into the Plane
            float velocityIntoPlane = dot(pointVelocity, PlaneNormal);

            // and moving further in, we need to resolve the collision
            if (PointDistanceFromPlane <= 0 && velocityIntoPlane <= 0)
            {
                NumContacts++;
                Contact += Point;
                ContactV += velocityIntoPlane;
            }
        }
    }

    if (NumContacts > 0)
    {
        Plane->ResolveCollision(Box, Contact / (float)NumContacts);
        return true;
    }

    return false;
}

bool PhysicsManager::CircleToPlane(RigidBody* a_Circle, RigidBody* a_Plane)
{
    CircleRB* CircleRB1 = dynamic_cast<CircleRB*>(a_Circle);
    PlaneRB* PlaneRB2 = dynamic_cast<PlaneRB*>(a_Plane);
    //if we are successful then test for collision
    if (CircleRB1 != nullptr && PlaneRB2 != nullptr)
    {
        vec2 PlaneNormal = PlaneRB2->GetNormal();
        float PointDistanceFromPlane = dot(CircleRB1->GetPosition(), PlaneNormal);
        float PlaneDistanceFromOrigin = dot(PlaneRB2->GetPosition(), PlaneNormal);

        PointDistanceFromPlane -= PlaneDistanceFromOrigin;
        PointDistanceFromPlane = abs(PointDistanceFromPlane);

        float CircleDistanceFromPlane = PointDistanceFromPlane - CircleRB1->GetRadius();

        if (CircleDistanceFromPlane <= 0)
        {
            vec2 ContactPoint = CircleRB1->GetPosition() + (PlaneNormal * -CircleRB1->GetRadius());
            PlaneRB2->ResolveCollision(CircleRB1, ContactPoint);
            return true;
        }
    }
    return false;
}

bool PhysicsManager::CircleToCircle(RigidBody* a_Circle1, RigidBody* a_Circle2)
{
    // try to cast objects to sphere and sphere
    CircleRB* Circle1 = dynamic_cast<CircleRB*>(a_Circle1);
    CircleRB* Circle2 = dynamic_cast<CircleRB*>(a_Circle2);
    // if we are successful then test for collision
    if (Circle1 == nullptr || Circle2 == nullptr)
        return false;

    float Distance = pow(Circle1->GetPosition().x - Circle2->GetPosition().x, 2) + pow(Circle1->GetPosition().y - Circle2->GetPosition().y, 2);
    Distance = sqrt(Distance);
    if (Distance <= Circle1->GetRadius() + Circle2->GetRadius())
    {
        float Penetration = Circle1->GetRadius() + Circle2->GetRadius() - Distance;
        if (Penetration > 0)
        {
            Circle1->ResolveCollision(Circle2, (Circle1->GetPosition() + Circle2->GetPosition()) * 0.5f, nullptr, Penetration);
            return true;
        }
        return true;
    }

    return false;
}

bool PhysicsManager::CircleToBox(RigidBody* a_Circle, RigidBody* a_Box)
{
    return BoxToCircle(a_Box, a_Circle);
}

bool PhysicsManager::BoxToPlane(RigidBody* a_Box, RigidBody* a_Plane)
{
    return PlaneToBox(a_Plane, a_Box);
}

bool PhysicsManager::BoxToCircle(RigidBody* a_Box, RigidBody* a_Circle)
{
    BoxRB* Box = dynamic_cast<BoxRB*>(a_Box);
    CircleRB* circle = dynamic_cast<CircleRB*>(a_Circle);

    if (Box == nullptr || circle == nullptr)
        return false;

    // transform the circle into the Box's coordinate space
    vec2 circlePosWorld = circle->GetPosition() - Box->GetPosition();
    vec2 circlePosBox = vec2(dot(circlePosWorld, Box->GetLocalX()), dot(circlePosWorld, Box->GetLocalY()));

    // find the closest point to the circle centre on the Box by clamping the coordinates in Box-space to the Box's extents
    vec2 closestPointOnBoxBox = circlePosBox;
    vec2 extents = Box->GetExtents();
    if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
    if (closestPointOnBoxBox.x > extents.x)  closestPointOnBoxBox.x = extents.x;
    if (closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = -extents.y;
    if (closestPointOnBoxBox.y > extents.y)  closestPointOnBoxBox.y = extents.y;
    // and convert back into world coordinates
    vec2 closestPointOnBoxWorld = Box->GetPosition() + closestPointOnBoxBox.x * Box->GetLocalX() + closestPointOnBoxBox.y * Box->GetLocalY();

    vec2 circleToBox = circle->GetPosition() - closestPointOnBoxWorld;
    float Penetration = circle->GetRadius() - length(circleToBox);
    if (Penetration > 0)
    {
        vec2 direction = normalize(circleToBox);
        vec2 contact = closestPointOnBoxWorld;
        Box->ResolveCollision(circle, contact, &direction, Penetration);
        return true;
    }

    return false;
}

bool PhysicsManager::BoxToBox(RigidBody* a_Box1, RigidBody* a_Box2)
{
    BoxRB* Box1 = dynamic_cast<BoxRB*>(a_Box1);
    BoxRB* Box2 = dynamic_cast<BoxRB*>(a_Box2);
    if (Box1 == nullptr || Box2 == nullptr)
        return false;

    vec2 boxPos = Box2->GetPosition() - Box1->GetPosition();
    vec2 norm(0, 0);
    vec2 contact(0, 0);
    float pen = 0;
    int numContacts = 0;
    Box1->CheckBoxCorners(Box2, contact, numContacts, pen, norm);
    if (Box2->CheckBoxCorners(Box1, contact, numContacts, pen, norm))
    {
        norm = -norm;
    }

    if (pen > 0)
    {
        Box1->ResolveCollision(Box2, contact / float(numContacts), &norm, pen);
        return true;
    }
    return false;
}
