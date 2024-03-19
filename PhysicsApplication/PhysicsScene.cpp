#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"

PhysicsScene::PhysicsScene()
{
    m_TimeStep = 0.01f;
}

PhysicsScene::~PhysicsScene()
{
    for (auto Actor : m_Actors)
    {
        delete Actor;
    }
}

void PhysicsScene::AddActor(PhysicsObject* a_Actor)
{
    m_Actors.push_back(a_Actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* a_Actor)
{
    m_Actors.erase(std::find(m_Actors.begin(), m_Actors.end(), a_Actor));
}

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
static const fn collisionFunctionArray[]
{
    PhysicsScene::PlaneToPlane,     PhysicsScene::PlaneToSphere,
    PhysicsScene::SphereToPlane, PhysicsScene::SphereToSphere,
};

void PhysicsScene::Update(const float DeltaTime)
{
    // update physics at a fixed time step

    static float AccumulatedTime = 0.0f;
    AccumulatedTime += DeltaTime;

    while (AccumulatedTime >= m_TimeStep)
    {
        for (auto pActor : m_Actors)
        {
            // Call each PhysicsObject's FixedUpdate()
            pActor->FixedUpdate(m_Gravity, m_TimeStep);
        }

        AccumulatedTime -= m_TimeStep;

        // check for collisions (ideally you'd want to have some sort of 
        // scene management in place)
        int ActorCount = m_Actors.size();

        // need to check for collisions against all objects except this one.
        for (int Outer = 0; Outer < ActorCount - 1; Outer++)
        {
            for (int Inner = Outer + 1; Inner < ActorCount; Inner++)
            {
                PhysicsObject* object1 = m_Actors[Outer];
                PhysicsObject* object2 = m_Actors[Inner];
                int shapeId1 = object1->m_ShapeType;
                int shapeId2 = object2->m_ShapeType;

                // using function pointers
                int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
                fn collisionFunctionPtr = collisionFunctionArray[functionIdx -1];
                if (collisionFunctionPtr != nullptr)
                {
                    // did a collision occur?
                    collisionFunctionPtr(object1, object2);
                }
            }
        }
    }
}

void PhysicsScene::Draw()
{
    for (auto pActor : m_Actors)
    {
        pActor->Draw(1);
    }
}

bool PhysicsScene::PlaneToPlane(PhysicsObject* Plane1, PhysicsObject* Plane2)
{
    return false;
}

bool PhysicsScene::PlaneToSphere(PhysicsObject* Plane, PhysicsObject* Sphere)
{
    // reverse the order of arguments, as obj1 is the plane and obj2 is the sphere
    return SphereToPlane(Sphere, Plane);
}

bool PhysicsScene::SphereToPlane(PhysicsObject* a_Sphere, PhysicsObject* a_Plane)
{
    Sphere* SphereObj = dynamic_cast<Sphere*>(a_Sphere);
    Plane* PlaneObj = dynamic_cast<Plane*>(a_Plane);
    //if we are successful then test for collision
    if (SphereObj != nullptr && PlaneObj != nullptr)
    {
        glm::vec2 collisionNormal = PlaneObj->GetNormal();
        float sphereToPlane = glm::dot(SphereObj->GetPosition(), PlaneObj->GetNormal()) - PlaneObj->GetDistance();

        float intersection = SphereObj->GetRadius() - sphereToPlane;
        float velocityOutOfPlane = glm::dot(SphereObj->GetVelocity(), PlaneObj->GetNormal());
        if (intersection > 0 && velocityOutOfPlane < 0)
        {
            glm::vec2 ContactPoint = SphereObj->GetPosition() + (collisionNormal * -SphereObj->GetRadius());
            PlaneObj->ResolveCollision(SphereObj, ContactPoint);
            return true;
        }
    }
    return false;
}

bool PhysicsScene::SphereToSphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
    // try to cast objects to sphere and sphere
    Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
    Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
    // if we are successful then test for collision
    if (sphere1 != nullptr && sphere2 != nullptr)
    {
        // TODO do the necessary maths in here
        // TODO if the spheres touch, set their velocities to zero for now

        float Distance = pow(sphere1->GetPosition().x - sphere2->GetPosition().x, 2) + glm::pow(sphere1->GetPosition().y - sphere2->GetPosition().y, 2);
        if (sqrt(Distance) <= sphere1->GetRadius() + sphere2->GetRadius())
        { 
            vec2 ContactPoint = 0.5f * (sphere1->GetPosition() + sphere2->GetPosition());
            sphere1->ResolveCollision(sphere2, 0.5f * (sphere1->GetPosition() + sphere2->GetPosition()));
            return true;
        }
    }
    return false;
}
