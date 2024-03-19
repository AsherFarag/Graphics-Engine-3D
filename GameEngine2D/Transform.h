#pragma once
// std
#include <vector>
#include <typeinfo>

// Maths
#include "glm/glm.hpp"
using namespace glm;

class Component;

class Transform
{
public:
	Transform();
	Transform(vec2 a_Position, float a_Rotation = 0.f);
	virtual ~Transform();

protected:
	std::vector<Transform*> m_Children;
	std::vector<std::pair<size_t, Component*>> m_Components;

public:
	vec2 m_Pos;
	float m_Rot;

	vec2 GetPosition() { return m_Pos; }
	void SetPosition(vec2 a_NewPos = vec2()) { m_Pos = a_NewPos; }

	float GetRotation() { return m_Rot; }
	void  SetRotation(float a_NewRot = 0.f) { m_Rot = a_NewRot; }

	vec2 GetNormal() { return normalize(vec2(std::sin(m_Rot), std::cos(m_Rot))); }

	template < typename T, typename... Args >
	T* AddComponent(Args&&... a_Args)
	{
		T* NewComponent = new T(std::forward< Args >(a_Args)...);
		m_Components.push_back({ typeid(T).hash_code(), NewComponent });
		NewComponent->SetOwner(this);
		return NewComponent;
	}

	template < typename T >
	T* GetComponent(int a_Index = 0)
	{
		size_t Hash = typeid(T).hash_code();

		for (auto C : m_Components)
		{
			if (C.first == Hash)
			{
				if (a_Index == 0)
					return dynamic_cast<T*>(C.second);
				else
					--a_Index;
			}
		}

		return nullptr;
	}

	template < typename T >
	void RemoveComponent(int a_Index = 0)
	{
		size_t Hash = typeid(T).hash_code();

		for (auto C : m_Components)
		{
			if (C.first == Hash)
			{
				if (a_Index == 0)
				{
					m_Components.erase(C);
					return;
				}
				else
					--a_Index;
			}
		}
	}
};


