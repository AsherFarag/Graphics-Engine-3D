#include "Collider.h"
#include <iostream>
#include "RigidBody.h"

Collider::Collider(EShapeType a_ShapeID)
{
	RigidBody* OwnersRB = m_Owner->GetComponent<RigidBody>();
	if (OwnersRB)
		m_AttachedRigidBody = OwnersRB;
	else
		std::cout << "Warning: Collider unable to find Rigidbody on initialisation" << std::endl;

	m_ShapeID = a_ShapeID;

}

Collider::~Collider()
{
}

void Collider::ResolveCollision(Collider* a_Collider, vec2 a_Contact, vec2* a_CollisionNormal, float a_Penetration)
{
	RigidBody* a_RigidBody = a_Collider->m_AttachedRigidBody;

	if (m_AttachedRigidBody == nullptr || a_RigidBody == nullptr)
		return;



	m_CollidersInsideThisFrame.push_back(a_Collider);
	a_Collider->m_CollidersInsideThisFrame.push_back(this);

	const vec2 Normal = normalize(a_CollisionNormal ? *a_CollisionNormal : a_Collider->GetPosition() - GetPosition());
	const vec2 Perpendicular(Normal.y, -Normal.x);

	// The radius from axis to application of force
	float Radius1 = dot(a_Contact - GetPosition(), -Perpendicular);
	float Radius2 = dot(a_Contact - a_Collider->GetPosition(), Perpendicular);

	// velocity of the contact point on this object
	float Velocity1 = dot(m_AttachedRigidBody->m_Velocity, Normal) - Radius1 * m_AttachedRigidBody->m_AngularVelocity;
	// velocity of contact point on actor2
	float Velocity2 = dot(a_RigidBody->GetVelocity(), Normal) + Radius2 * a_RigidBody->GetAngularVelocity();

	if (m_IsTrigger || a_Collider->m_IsTrigger)
	{
		TriggerEnter(a_Collider);
		a_Collider->TriggerEnter(this);
		return;
	}

	if (Velocity1 > Velocity2)
	{
		float Mass1 = 1.f / (1.f / m_AttachedRigidBody->GetMass() + (Radius1 * Radius1) / m_AttachedRigidBody->GetMoment());
		float Mass2 = 1.f / (1.f / a_RigidBody->GetMass() + (Radius2 * Radius2) / a_Actor->GetMoment());

		float AverageElasticity = (GetElasticity() + a_Actor->GetElasticity()) / 2.f;
		vec2 ResultForce = (1.f + AverageElasticity) * Mass1 * Mass2 / (Mass1 + Mass2) * (Velocity1 - Velocity2) * Normal;
		ApplyForce(-ResultForce, a_Contact - GetPosition());
		a_Actor->ApplyForce(ResultForce, a_Contact - a_Actor->GetPosition());

		if (CollisionCallBack != nullptr)
			CollisionCallBack(a_Actor);
		if (a_Actor->CollisionCallBack)
			a_Actor->CollisionCallBack(this);

		if (a_Penetration > 0)
			World::GetPhysicsManager()->ApplyContactForces(this, a_Actor, Normal, a_Penetration);
	}
}
