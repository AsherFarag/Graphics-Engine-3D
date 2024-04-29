#pragma once
#include "UBaseObject.h"

// --- STD ---
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <string>

// --- Engine ---
#include "UTransform.h"
class UBaseComponent;

class AActor :
    public UBaseObject
{
	INSPECTABLE;

public:
    AActor();
    virtual ~AActor();

	virtual void Begin() {}
	virtual void Update();

protected:
	UTransform* m_Transform;

	//					  Type_Hash, Component Ref
    std::vector<std::pair<size_t, UBaseComponent*>> m_Components;

	virtual void OnDestroyed() {};

	std::string m_Name;

public:

	#pragma region --- Transforms ---

	UTransform& GetTransform() { return *m_Transform; }

	vec3& GetActorPosition();
	quat& GetActorRotation();
	vec3& GetActorRotationEular();
	vec3& GetActorScale();
	vec3  GetForward();

	void SetActorPosition(const vec3& a_Position);
	void SetActorRotation(const quat& a_Rotation);
	void SetActorScale	 (const vec3& a_Scale);

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
	virtual void OnDraw_ImGui() {}

#pragma endregion
};

