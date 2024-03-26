#include "AFlyCamera.h"

// --- AIE ---
#include "World.h"
#include "Input.h"

AFlyCamera::AFlyCamera()
	: ACamera()
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

	//vec3 Forward(cos(YawR) * cos(PitchR), sin(YawR), cos(YawR) * sin(PitchR));
	//vec3 Right(-sin(PitchR), 0, cos(PitchR));
	vec3 Forward = GetForward();
	vec3 Right(sin(ThetaR), 0, cos(ThetaR));
	vec3 Up(0, 1, 0);

	if (Input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_Position += Forward * m_ForwardMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_Position -= Forward * m_ForwardMoveSpeed * DeltaTime * m_MoveSpeedMultiplier;
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
		m_Rotation.y += m_LookSensitivity * DeltaTime;
	}
	if (Input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		m_Rotation.y -= m_LookSensitivity * DeltaTime;
	}

	float MouseX = Input->getMouseX();
	float MouseY = Input->getMouseY();

	if (Input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		/*m_Rotation.y -= m_LookSensitivity * (MouseX - m_LastMousePosition.x) * DeltaTime;
		m_Rotation.x -= m_LookSensitivity * (MouseY - m_LastMousePosition.y) * DeltaTime;*/

		m_Rotation.y -= m_LookSensitivity * (MouseX - m_LastMousePosition.x) * DeltaTime;
		m_Rotation.z += m_LookSensitivity * (MouseY - m_LastMousePosition.y) * DeltaTime;
		//m_Rotation.z = clamp(m_Phi, -87.5f, 87.5f);
	}

	m_LastMousePosition = vec2(MouseX, MouseY);

#pragma endregion

}

void AFlyCamera::OnDraw_ImGui()
{
	ACamera::OnDraw_ImGui();
	ImGui::SliderFloat("Look Sensitivity", &m_LookSensitivity, 5.f, 200.f, "%.1f");
	ImGui::Text("Speed: %.1f", m_MoveSpeedMultiplier);
}
