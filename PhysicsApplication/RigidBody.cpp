#include "RigidBody.h"

RigidBody::RigidBody(ShapeType a_ShapeType) : PhysicsObject(a_ShapeType)
{
	m_Elasticity = 1.f;
	m_Position = glm::vec2();
	m_Velocity = glm::vec2();
	m_Rotation = 0.f;
	m_Mass = 1.f;
}

RigidBody::RigidBody(ShapeType a_ShapeType, glm::vec2 position, glm::vec2 velocity, float Rotation, float mass, float elasticity = 1.f) : PhysicsObject(a_ShapeType)
{
	SetElasticity(elasticity);
	m_Position = position;
	m_Velocity = velocity;
	m_Rotation = Rotation;
	m_Mass = mass;
}

void RigidBody::FixedUpdate(glm::vec2 a_Gravity, float a_TimeStep)
{
	m_Position += m_Velocity * a_TimeStep;
	ApplyForce(a_Gravity * m_Mass * a_TimeStep);

	m_Rotation += 0.01f * m_AngularVelocity * a_TimeStep;
}

void RigidBody::ApplyForce(glm::vec2 a_Force, glm::vec2* a_ImpulsePosition)
{
	m_Velocity += a_Force / GetMass();
	if (a_ImpulsePosition)
		m_AngularVelocity += (a_Force.y * a_ImpulsePosition->x - a_Force.x * a_ImpulsePosition->y) / GetMoment();
}

void RigidBody::ApplyForceToActor(RigidBody* actor2, glm::vec2 force)
{
	ApplyForce(-force);
	actor2->ApplyForce(force);
}

void RigidBody::ResolveCollision(RigidBody* a_Actor, glm::vec2 a_Contact, glm::vec2* a_CollisionNormal)
{
	const glm::vec2 Normal = glm::normalize(a_CollisionNormal ? *a_CollisionNormal : a_Actor->GetPosition() - m_Position);
	const glm::vec2 Perpendicular(Normal.y, -Normal.x);

	// The radius frm axis to application of force
	float Radius1 = glm::dot(a_Contact - m_Position, -Perpendicular);
	float Radius2 = glm::dot(a_Contact - a_Actor->GetPosition(), Perpendicular);

	// velocity of the contact point on this object
	float Velocity1 = glm::dot(m_Velocity, Normal) - Radius1 * m_AngularVelocity;
	// velocity of contact point on actor2
	float Velocity2 = glm::dot(a_Actor->GetVelocity(), Normal) + Radius2 * a_Actor->GetAngularVelocity();

	if (Velocity1 > Velocity2)
	{
		float Mass1 = 1.f / (1.f / GetMass() + (Radius1 * Radius1) / GetMoment());
		float Mass2 = 1.f / (1.f / a_Actor->GetMass() + (Radius2 * Radius2) / a_Actor->GetMoment());

		glm::vec2 ResultForce = (1.f + GetElasticity()) * Mass1 * Mass2 / (Mass1 + Mass2) * (Velocity1 - Velocity2) * Normal;
		ApplyForce(-ResultForce, &(a_Contact - m_Position));
		a_Actor->ApplyForce(ResultForce, &(a_Contact - a_Actor->GetPosition()));
	}
}
