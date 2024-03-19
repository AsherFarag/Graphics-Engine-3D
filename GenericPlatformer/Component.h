#pragma once
#include "Transform.h"
#include "World.h"

class Component
{
public:
	Component(Transform* a_Owner) { m_OwnerTransform = a_Owner; }
	virtual ~Component() = default;

	virtual void Update(float DeltaTime) = 0;
	virtual void Draw(float a_Alpha) = 0;

	Transform* GetTransform() const { return m_OwnerTransform; }
	void SetTransform(Transform* a_NewOwnerTransform) { m_OwnerTransform = a_NewOwnerTransform; }

	vec2 GetPosition() { return m_OwnerTransform->m_Pos; }
	float GetRotation() { return m_OwnerTransform->m_Rot; }

protected:
	Transform* m_OwnerTransform;
};

