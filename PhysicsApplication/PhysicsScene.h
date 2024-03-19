#pragma once
#include "glm/glm.hpp"
#include <iostream>
#include "PhysicsObject.h"
#include "Sphere.h"
#include <vector>

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* a_Actor);
	void RemoveActor(PhysicsObject* a_Actor);
	void Update(const float DeltaTime);
	void Draw();

	void SetGravity(const glm::vec2 a_Gravity) { m_Gravity = a_Gravity; }
	glm::vec2 GetGravity() const { return m_Gravity; }

	void SetTimeStep(const float a_TimeStep) { m_TimeStep = a_TimeStep; }
	float GetTimeStep() const { return m_TimeStep; }

protected:
	glm::vec2 m_Gravity = glm::vec2(0.f, -9.81f);
	float m_TimeStep;
	std::vector<PhysicsObject*> m_Actors;

#pragma region Collision Scenarios
public:
	static bool PlaneToPlane(PhysicsObject*, PhysicsObject*);
	static bool PlaneToSphere(PhysicsObject*, PhysicsObject*);
	static bool SphereToPlane(PhysicsObject*, PhysicsObject*);
	static bool SphereToSphere(PhysicsObject*, PhysicsObject*);

#pragma endregion
};

