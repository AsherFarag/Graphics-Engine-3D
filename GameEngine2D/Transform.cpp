#include "Transform.h"
#include "Component.h"

Transform::Transform() : Transform(vec2()){}

Transform::Transform(vec2 a_Position, float a_Rotation)
{
	m_Pos = a_Position;
	m_Rot = a_Rotation;
	World::AddTransform(this);
}

Transform::~Transform()
{
	for (auto it : m_Components)
	{
		delete it.second;
	}
	World::RemoveTransform(this);
}
