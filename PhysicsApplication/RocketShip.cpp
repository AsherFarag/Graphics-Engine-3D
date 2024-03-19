#include "RocketShip.h"
#include "Texture.h"
#include "Input.h"

RocketShip::RocketShip(glm::vec2 a_Position, float a_MassLossRate, float a_ThrustSpeed, float a_Mass)
{
	m_Rotation = 0.f;
	m_Mass = a_Mass;
	m_ThrustSpeed = a_ThrustSpeed;
	m_MassLossRate = a_MassLossRate;
	m_Position = a_Position;
	m_Texture = new aie::Texture("../bin/textures/ship.png");
}

RocketShip::~RocketShip()
{
	delete m_Texture;
}

void RocketShip::FixedUpdate(glm::vec2 a_Gravity, float a_TimeStep)
{
	aie::Input* input = aie::Input::getInstance();
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		AddRotation(0.5f * a_TimeStep);

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		AddRotation(-0.5f * a_TimeStep);

	if (m_Mass >= 0.f)
	{
		m_Mass -= m_MassLossRate * a_TimeStep;
		glm::vec2 MoveAmount;
		MoveAmount = m_ThrustSpeed * glm::vec2(glm::sin(m_Rotation), glm::cos(m_Rotation));
		//ApplyForce(MoveAmount);
		Sphere::FixedUpdate(a_Gravity, a_TimeStep);
	}
}

void RocketShip::Draw(float alpha)
{
	/*a_2DRenderer.setUVRect(0, 0, 1, 1);
	a_2DRenderer.drawSprite(m_Texture, m_Position.x, m_Position.y, 100, 100, -m_Rotation);*/
	return;
}
