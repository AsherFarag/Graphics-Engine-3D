#pragma once
#include "UBaseObject.h"

// --- STD ---
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <string>

// --- GLM ---
#include <glm/gtc/matrix_transform.hpp>

// --- Engine ---
class UBaseComponent;

class AActor :
    public UBaseObject
{
public:
    AActor();
    virtual ~AActor();

	virtual void Begin() {}
	virtual void Update();

protected:
    // --- Transforms ---
    mat4 m_Transform = mat4(1);

	vec3 m_Position = vec3();
	vec3 m_Rotation = vec3(); // In Degrees
	vec3 m_Scale = vec3(1);

	bool m_Dirty = false;

	// --- Components ---
	//					  Type_Hash, Component Ref
    std::vector<std::pair<size_t, UBaseComponent*>> m_Components;

	virtual void OnDestroyed() {};

	std::string m_Name;

// === Getter Functions ===
public:

	#pragma region --- Transforms ---

	mat4* GetTransform() { UpdateTransform(); return &m_Transform; }

	vec3 GetPosition()	{ return m_Position; }
	vec3 GetRotation()	{ return m_Rotation; }
	vec3 GetScale()		{ return m_Scale;	 }

	void SetPosition(vec3 a_Position) { m_Position = a_Position; m_Dirty = true; }
	void SetRotation(vec3 a_Rotation) { m_Rotation = a_Rotation; m_Dirty = true; }
	void SetScale(vec3 a_Scale) { m_Scale = a_Scale; m_Dirty = true; }

	void UpdateTransform(bool a_Force = false);

	#pragma endregion

	#pragma region 	--- Components ---

	// Instantiates a new component of the type
	template < typename T, typename... Args >
	T* AddComponent(Args&&... a_Args)
	{
		T* NewComponent = new T(std::forward< Args >(a_Args)...);
		m_Components.push_back({ typeid(T).hash_code(), NewComponent });
		return NewComponent;
	}

	// Stop using a pointer as the type you idiot!
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

	#pragma endregion

	void SetActive(bool NewActive, bool PropagateToChildren = true, bool PropagateToComponents = true);
	virtual void OnEnabled() override;
	virtual void OnDisabled() override;

	std::string GetName() const { return m_Name; }
	void SetName(std::string a_Name) { m_Name = a_Name; }

#pragma region ImGui

public:
	void Draw_ImGui();

protected:
	virtual void OnDraw_ImGui();

#pragma endregion

#if IS_EDITOR

	// Allow the Inspector to access properties
public:
	friend class Inspector;

#endif
};

