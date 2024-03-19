#pragma once
#include "Sphere.h"
#include "Gizmos.h"
#include "glm/glm.hpp"

class RocketShip :
	public Sphere
{
	aie::Texture* m_Texture;
	float m_ThrustSpeed;
	float m_MassLossRate;

public:
	RocketShip() {};
	RocketShip(glm::vec2 a_Position, float a_MassLossRate, float a_ThrustSpeed, float a_Mass);
	~RocketShip();

	void AddRotation(float a_AngleRadians) { m_Rotation += a_AngleRadians; }

	void Update(float DeltaTime);
	virtual void FixedUpdate(glm::vec2 a_Gravity, float a_TimeStep) override;
	virtual void Draw(float alpha) override;
};

