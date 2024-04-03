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

	// Change the Speed Multiplier if the user has scrolled the mouse wheel
	if (Input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
		m_MoveSpeedMultiplier -= ((m_LastMouseScroll - Input->getMouseScroll()) * m_MouseScrollSensitivity) * 25.f;
	else
		m_MoveSpeedMultiplier -= (m_LastMouseScroll - Input->getMouseScroll()) * m_MouseScrollSensitivity;

	m_MoveSpeedMultiplier = clamp(m_MoveSpeedMultiplier, 0.f, m_MaxMoveSpeedMultiplier);
	m_LastMouseScroll = Input->getMouseScroll();

	float ThetaR = radians(m_Rotation.y);
	float PhiR = radians(m_Rotation.z);

	constexpr vec3 Up(0.f, 1.f, 0.f);
	vec3 Right = cross(m_Forward, Up);


	if (Input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_Position += m_Forward * m_ForwardMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_Position -= m_Forward * m_ForwardMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_Position += Right * m_StrafeMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_Position -= Right * m_StrafeMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_UP))
	{
		m_Position += Up * m_ElevationMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_DOWN))
	{
		m_Position -= Up * m_ElevationMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		m_Rotation.y -= m_LookSensitivity * DeltaTime;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		m_Rotation.y += m_LookSensitivity * DeltaTime;
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
			m_Rotation = vec3(Quat.x * Quat.w, Quat.y * Quat.w, Quat.z * Quat.w);
		}

		//m_Rotation.y -= m_LookSensitivity * (MouseX - m_LastMousePosition.x) * DeltaTime;
		//m_Rotation.z += m_LookSensitivity * (MouseY - m_LastMousePosition.y) * DeltaTime;
	}

	m_LastMousePosition = vec2(Input->getMouseX(), Input->getMouseY());

#pragma endregion

}

void AFlyCamera::OnDraw_ImGui()
{
	ACamera::OnDraw_ImGui();
	ImGui::SliderFloat("Look Sensitivity", &m_LookSensitivity, 0.f, 10.f, "%.1f");
	ImGui::Text("Speed: %.1f", m_MoveSpeedMultiplier);
}
