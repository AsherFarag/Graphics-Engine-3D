#include "PhysicsObject.h"
#include "World.h"
#include "PhysicsManager.h"

PhysicsObject::PhysicsObject(Transform* a_Owner, vec4 a_Colour)
	: Component(a_Owner)
{
	m_PhysicsManager = World::GetPhysicsManager();
	m_PhysicsManager->AddPhysicsObject(this);

	m_Colour = a_Colour;

	if (a_Owner)
		CalculateAxis();
}

void PhysicsObject::CalculateAxis()
{
	float sn = sinf(GetRotation());
	float cs = cosf(GetRotation());
	m_LocalX = normalize(vec2(cs, sn));
	m_LocalY = normalize(vec2(-sn, cs));
}
