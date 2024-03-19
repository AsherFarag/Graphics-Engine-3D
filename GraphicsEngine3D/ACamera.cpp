#include "ACamera.h"

// --- GLM ---
#include "glm/common.hpp"

// --- Engine ---
#include "GraphicsEngine3DApp.h"

ACamera::ACamera()
	: AActor()
{
	m_Name = "Camera";
}

ACamera::~ACamera()
{
}

void ACamera::Update()
{
	SetViewMatrix();
	SetAspectRatio(Engine->getWindowWidth(), Engine->getWindowHeight());
}

void ACamera::SetViewMatrix()
{
	float ThetaR = radians(m_Theta);
	float PhiR = radians(m_Phi);
	vec3 Forward(cos(PhiR) * cos(ThetaR), sin(PhiR), cos(PhiR) * sin(ThetaR));
	m_ViewTransform = lookAt(m_Position, m_Position + Forward, vec3(0, 1, 0));
}

void ACamera::UpdateProjectionMatrix()
{
	m_ProjectionViewTransform = perspective(radians(m_FOVDegrees), m_AspectRatio, m_Near, m_Far);
}

void ACamera::SetProjectionMatrix(float a_FOVDegrees, float a_AspectRatio, float a_Near, float a_Far)
{
	m_FOVDegrees = a_FOVDegrees;
	m_AspectRatio = a_AspectRatio;
	m_Near = a_Near;
	m_Far = a_Far;

	m_ProjectionViewTransform = perspective(radians(m_FOVDegrees), m_AspectRatio, m_Near, m_Far);
}

void ACamera::SetProjectionMatrix(float a_FOVDegrees, float a_Width, float a_Height, float a_Near, float a_Far)
{
	SetProjectionMatrix(a_FOVDegrees, a_Width / a_Height, a_Near, a_Far);
}

void ACamera::SetAspectRatio(float a_AspectRatio)
{
	m_AspectRatio = a_AspectRatio;
	UpdateProjectionMatrix();
}

void ACamera::SetAspectRatio(float a_Width, float a_Height)
{
	SetAspectRatio(a_Width / a_Height);
}

void ACamera::OnDraw_ImGui()
{
	ImGui::Text("Aspect Ratio: %0.3f", m_AspectRatio);
	float FOV = m_FOVDegrees;
	if (ImGui::SliderFloat("FOV", &FOV, 0.f, 180.f, "%.f"))
		SetFOV(FOV);
}
