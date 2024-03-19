#pragma once
#include "Transform.h"
#include "World.h"

class Component
{
public:
	Component() {}
	Component(Transform* a_Owner) { m_Owner = a_Owner; }
	virtual ~Component() = default;

	virtual void Update(float DeltaTime) = 0;
	virtual void Draw(float a_Alpha) = 0;

	Transform* GetTransform() const { return m_Owner; }

	template <typename T>
	T* GetOwner() { return dynamic_cast<T>(m_Owner); }
	void SetOwner(Transform* a_Owner);

	vec2 GetPosition() { return m_Owner->m_Pos; }
	float GetRotation() { return m_Owner->m_Rot; }

protected:
	Transform* m_Owner;
};

