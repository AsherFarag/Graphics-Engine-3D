#include "APlayer.h"

#include "CircleRB.h"
#include "Renderer.h"

#include "glm/ext.hpp"

#include <iostream>

APlayer::APlayer(vec2 a_Pos, float a_Radius, vec4 a_Colour, float a_Mass)
	: AActor(a_Pos)
{
	m_Input = aie::Input::getInstance();

	SetPosition(a_Pos);
	m_BaseColour = a_Colour;

	m_RB = AddComponent<CircleRB>(this, RigidBody::DYNAMIC, a_Radius, a_Colour, a_Radius * a_Radius * pi<float>());
	//m_RB->SetMass(10);
	m_RB->SetDrawSegments(20);
	m_RB->SetDynamicFrictionCoefficient(9.5f);
	m_RB->SetStaticFrictionCoefficient(10.f);

	m_Renderer = AddComponent<Renderer>(this, "../bin/");
}

APlayer::~APlayer()
{
}

void APlayer::Update(float DeltaTime)
{
	if (m_Input == nullptr)
		m_Input = aie::Input::getInstance();

	int ScreenX, ScreenY;
	m_Input->getMouseXY(&ScreenX, &ScreenY);
	m_MousePosition = vec2(ScreenX, ScreenY);

	// Gets Clicked by Mouse
	if (m_Input->isMouseButtonDown(0) && !m_IsHeld && m_CanHold)
	{
		m_ClickPosition = m_MousePosition;
		m_IsHeld = true;
		m_CanHold = false;

		m_Renderer->SetColour(m_HeldColour);
		m_RB->SetColour(m_HeldColour);
	}
	// Gets Unclicked by Mouse
	else if (m_Input->isMouseButtonUp(0) && m_IsHeld)
	{
		m_IsHeld = false;
		m_TimesShot += 1;

		vec2 Local = m_MousePosition - m_ClickPosition;
		if (length(Local) != 0)
		{
			float ShootPower = distance(m_MousePosition, m_ClickPosition) * m_RB->GetMass() * m_ShootPowerMultiplier;
			ShootPower = min(ShootPower, m_MaxShootPower);

			m_RB->ApplyForce(normalize(-Local) * ShootPower);
		}

		m_Renderer->SetColour(m_BaseColour);
		m_RB->SetColour(m_BaseColour);
	}

	if (m_IsHeld)
	{
		m_RB->SetVelocity();
		m_RB->SetAngularVelocity();

		vec2 Local = m_MousePosition - m_ClickPosition;
		float ShootPower = distance(m_MousePosition, m_ClickPosition) * m_RB->GetMass() * m_ShootPowerMultiplier;
		ShootPower = min(ShootPower, m_MaxShootPower);

		m_ShootPosition = normalize(-Local) * ShootPower;
		m_ShootVelocity = GetPosition();
	}

	if (length(m_RB->GetVelocity()) <= 15.5f)
	{
		m_CanHold = true;
	}
	else
	{
		m_CanHold = false;
	}
}

void APlayer::Draw()
{
	if (m_Input == nullptr)
		m_Input = aie::Input::getInstance();

	if (m_IsHeld)
	{
		aie::Gizmos::add2DLine(m_ClickPosition, m_MousePosition, vec4(1));

		// Draw Predictive Projectile Trajectory
		vec2 PreviousTrajectoryPoint = m_ShootVelocity;
		const int Points = 20;
		const float StepAmount = 15.f;
		for (int i = 0; i < 20; i++)
		{
			vec2 TrajectoryPoint = GetTrajectoryPoint(m_ShootVelocity, m_ShootPosition / m_RB->GetMass(), i * StepAmount);
			aie::Gizmos::add2DCircle(TrajectoryPoint, (float)(Points - i) / 4.f + 1, 7, vec4(1));
			PreviousTrajectoryPoint = TrajectoryPoint;
		}

	}
}

vec2 APlayer::GetTrajectoryPoint(vec2& a_StartingPos, vec2& a_StartingVelocity, float n)
{
	const float t = World::GetPhysicsManager()->GetTimeStep(); // seconds per time step (at 60fps)
	vec2 stepVelocity = t * a_StartingVelocity; // m/s
	vec2 stepGravity = t * t * (World::GetPhysicsManager()->GetGravity()); // m/s/s
	vec2 NewPos = n * stepVelocity + 0.5f * (n * n + n) * stepGravity;

	return a_StartingPos + NewPos;
}

