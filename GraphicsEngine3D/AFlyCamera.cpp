#include "AFlyCamera.h"

// --- GLM ---
#include "glm/ext.hpp"


// --- AIE ---
#include "World.h"
#include "Input.h"

AFlyCamera::AFlyCamera(aie::AIERenderTarget* a_RenderTarget)
	: ACamera(a_RenderTarget)
{
}

AFlyCamera::~AFlyCamera()
{
}

void AFlyCamera::Update()
{
	ACamera::Update();

	float DeltaTime = World::DeltaTime();

	aie::Input* Input = aie::Input::getInstance();

#pragma region Movement Input

	// Scroll Input
	if (Input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT) && Input->getMouseScroll() != m_LastMouseScroll)
	{
		constexpr float ScrollSpeedMultiplier = 25.f;
		vec3 Translation = m_Forward * ScrollSpeedMultiplier * DeltaTime;
		// Get the direction from the scroll amount
		GetActorPosition() += Translation * ((m_LastMouseScroll - Input->getMouseScroll()) < 0.f ? 1.f : -1.f);
	}
	else
	{
		m_MoveSpeedMultiplier -= (m_LastMouseScroll - Input->getMouseScroll()) * m_MouseScrollSensitivity;
	}

	m_MoveSpeedMultiplier = glm::clamp(m_MoveSpeedMultiplier, 0.f, m_MaxMoveSpeedMultiplier);
	m_LastMouseScroll = Input->getMouseScroll();

	float ThetaR = glm::radians(GetActorRotation().y);
	float PhiR = glm::radians(GetActorRotation().z);

	constexpr vec3 Up(0.f, 1.f, 0.f);
	vec3 Right = cross(m_Forward, Up);


	if (Input->isKeyDown(aie::INPUT_KEY_W))
	{
		GetActorPosition() += m_Forward * m_ForwardMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_S))
	{
		GetActorPosition() -= m_Forward * m_ForwardMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_D))
	{
		GetActorPosition() += Right * m_StrafeMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_A))
	{
		GetActorPosition() -= Right * m_StrafeMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_UP))
	{
		GetActorPosition() += Up * m_ElevationMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_DOWN))
	{
		GetActorPosition() -= Up * m_ElevationMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		GetActorRotation().y -= m_LookSensitivity * DeltaTime;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		GetActorRotation().y += m_LookSensitivity * DeltaTime;
	}

	float MouseDeltaX = Input->getMouseX() - m_LastMousePosition.x;
	float MouseDeltaY = Input->getMouseY() - m_LastMousePosition.y;
	// Rotation
	if (Input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		if (MouseDeltaX != 0.f || MouseDeltaY != 0.f)
		{
			float PitchDelta = MouseDeltaY * m_LookSensitivity * DeltaTime;
			float YawDelta = MouseDeltaX * m_LookSensitivity * DeltaTime;

			glm::quat Quat = normalize(cross(angleAxis(PitchDelta, Right), angleAxis(-YawDelta, Up)));
			m_Forward = rotate(Quat, m_Forward);
			GetActorRotation() = vec3(Quat.x * Quat.w, Quat.y * Quat.w, Quat.z * Quat.w);
		}

		//m_Rotation.y -= m_LookSensitivity * (MouseX - m_LastMousePosition.x) * DeltaTime;
		//m_Rotation.z += m_LookSensitivity * (MouseY - m_LastMousePosition.y) * DeltaTime;
	}

	m_LastMousePosition = vec2(Input->getMouseX(), Input->getMouseY());

#pragma endregion

	m_Transform->Update();
}

void AFlyCamera::OnDraw_ImGui()
{
	ACamera::OnDraw_ImGui();
	ImGui::SliderFloat("Look Sensitivity", &m_LookSensitivity, 0.f, 10.f, "%.1f");
	ImGui::Text("Speed: %.1f", m_MoveSpeedMultiplier);
}
