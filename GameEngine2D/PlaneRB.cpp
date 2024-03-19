#include "PlaneRB.h"

#include "iostream"

PlaneRB::PlaneRB(Transform* a_Owner, ECollisionType a_CollisionType, float a_HalfLength, vec4 a_Colour, float a_Mass, float a_Elasticity, vec2 a_Velocity, float a_AngularVelocity)
    : RigidBody(a_Owner, PLANE, a_CollisionType, a_Mass, a_Elasticity, a_Velocity, a_AngularVelocity, a_Colour)
{
    m_HalfLength = a_HalfLength;
}

void PlaneRB::Draw(float a_Alpha)
{
    vec2 Normal = GetTransform()->GetNormal();
    //vec2 centerPoint = m_Normal * m_DistanceToOrigin;
    // easy to rotate normal through 90 degrees around z
    vec2 parallel(Normal.y, -Normal.x);
    vec4 colourFade = m_Colour;
    colourFade.a = 1;
    vec2 start = GetPosition() + (parallel * m_HalfLength);
    vec2 end = GetPosition() - (parallel * m_HalfLength);
    aie::Gizmos::add2DLine(start, end, m_Colour);


    //aie::Gizmos::add2DTri(start, end, start - m_Normal * 10.0f, m_Colour, m_Colour, colourFade);
    //aie::Gizmos::add2DTri(end, end - m_Normal * 10.0f, start - m_Normal * 10.0f, m_Colour, colourFade, colourFade);
}

void PlaneRB::ResolveCollision(RigidBody* a_RigidBody, vec2 a_Contact, vec2* a_CollisionNormal, float a_Penetration)
{
	const vec2 Normal = GetNormal();

	// the position at which we'll apply the force relative to the object's COM
	vec2 LocalContact = a_Contact - a_RigidBody->GetPosition();

	// the plane isn't moving, so the relative velocity is just a_RigidBody's velocity at the Contact point
	vec2 RelativeVelocity = a_RigidBody->GetVelocity() + a_RigidBody->GetAngularVelocity() * vec2(-LocalContact.y, LocalContact.x);
	float VelocityIntoPlane = dot(RelativeVelocity, Normal);

	// this is the perpendicular distance we apply the force at relative to the COM, so Torque = F*r
	float r = dot(LocalContact, vec2(Normal.y, -Normal.x));

	// work out the "effective mass" - this is a combination of moment of
	// inertia and mass, and tells us how much the Contact point velocity 
	// will change with the force we're applying
	float EffectiveMass = 1.0f / (1.0f / a_RigidBody->GetMass() + (r * r) / a_RigidBody->GetMoment());

	float AverageElasticity = (GetElasticity() + a_RigidBody->GetElasticity()) / 2.f;
	float ImpulseMagnitude = -(1 + AverageElasticity) * VelocityIntoPlane * EffectiveMass;

	vec2 ResultForce = Normal * ImpulseMagnitude;
	a_RigidBody->ApplyForce(ResultForce, a_Contact - a_RigidBody->GetPosition());

	if (a_RigidBody->CollisionCallBack)
		a_RigidBody->CollisionCallBack(this);

	float PlaneDistanceFromOrigin = dot(GetPosition(), Normal);
	float pen = dot(a_Contact, Normal) - PlaneDistanceFromOrigin;
	World::GetPhysicsManager()->ApplyContactForces(a_RigidBody, nullptr, Normal, pen);
}
