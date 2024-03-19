#include "CircleRB.h"

CircleRB::CircleRB(Transform* a_Owner, ECollisionType a_CollisionType, float a_Radius, vec4 a_Colour, float a_Mass, float a_Elasticity, vec2 a_Velocity, float a_AngularVelocity)
    : RigidBody(a_Owner, CIRCLE, a_CollisionType, a_Mass, a_Elasticity, a_Velocity, a_AngularVelocity, a_Colour)
{
    m_Radius = a_Radius;
    CalculateMoment();
}

void CircleRB::Update(float DeltaTime)
{
    RigidBody::Update(DeltaTime);
}

void CircleRB::Draw(float a_Alpha)
{
    aie::Gizmos::add2DCircle(GetPosition(), GetRadius(), 12, m_Colour);
    aie::Gizmos::add2DLine(GetPosition(), GetPosition() + GetNormal() * GetRadius(), vec4(0, 1, 0, 1));
}

void CircleRB::CalculateMoment()
{
    m_Moment = 0.5f * m_Mass * m_Radius * m_Radius;
}

bool CircleRB::IsInside(vec2 a_Point)
{
    return distance(a_Point, GetPosition()) <= GetRadius();
}
