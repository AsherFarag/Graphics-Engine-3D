#include "Plane.h"
#include "Gizmos.h"
#include "RigidBody.h"

Plane::Plane() : PhysicsObject(PLANE)
{
    m_DistanceToOrigin = -10;
    m_Normal = vec2(0, 1);
    m_Colour = glm::vec4(1);
}

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(PLANE)
{
    m_DistanceToOrigin = distance;
    m_Normal = normal;
    m_Normal = glm::normalize(m_Normal);
    m_Colour = glm::vec4(1);
}

void Plane::FixedUpdate(glm::vec2 a_Gravity, float a_TimeStep)
{
}

void Plane::Draw(float alpha)
{
    float lineSegmentLength = 300;
    vec2 centerPoint = m_Normal * m_DistanceToOrigin;
    // easy to rotate normal through 90 degrees around z
    vec2 parallel(m_Normal.y, -m_Normal.x);
    vec4 colourFade = m_Colour;
    colourFade.a = 0;
    vec2 start = centerPoint + (parallel * lineSegmentLength);
    vec2 end = centerPoint - (parallel * lineSegmentLength);
    //aie::Gizmos::add2DLine(start, end, colour);
    aie::Gizmos::add2DTri(start, end, start - m_Normal * 10.0f, m_Colour, m_Colour, colourFade);
    aie::Gizmos::add2DTri(end, end - m_Normal * 10.0f, start - m_Normal * 10.0f, m_Colour, colourFade, colourFade);
}

void Plane::ResolveCollision(RigidBody* a_RigidBody, glm::vec2 a_Contact)
{
    const glm::vec2 RelativeVelocity = a_RigidBody->GetVelocity();
    float Elasticity = a_RigidBody->GetElasticity();
    float ImpulseMagnitude = glm::dot(-(1 + Elasticity) * RelativeVelocity, m_Normal) / (1 / a_RigidBody->GetMass());

    glm::vec2 ResultantForce = m_Normal * ImpulseMagnitude;
    a_RigidBody->ApplyForce(ResultantForce, &(a_Contact - a_RigidBody->GetPosition()));
}
