#include "Sphere.h"
#include "Gizmos.h"
#include "PhysicsScene.h"

Sphere::Sphere() : RigidBody(SPHERE)
{
	m_Radius = 1.f;
	m_Colour = glm::vec4();
}

Sphere::Sphere(glm::vec2 a_Position, glm::vec2 a_Velocity, float a_Mass, float a_Radius, glm::vec4 a_Colour = glm::vec4(), float a_Elasticity = 1.f)
	: RigidBody(SPHERE, a_Position, a_Velocity, 0.f, a_Mass, a_Elasticity)
{
	m_Radius = a_Radius;
	m_Colour = a_Colour;
}

void Sphere::Draw(float alpha)
{
	glm::vec2 End = glm::vec2(std::sin(m_Rotation), std::cos(m_Rotation)) * m_Radius;
	aie::Gizmos::add2DCircle(m_Position, m_Radius, 6 * m_Radius, m_Colour);
	aie::Gizmos::add2DLine(m_Position, m_Position + End, glm::vec4(1));
}
