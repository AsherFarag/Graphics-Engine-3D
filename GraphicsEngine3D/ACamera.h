#pragma once
#include "AActor.h"

// --- GLM ---
#include "glm/trigonometric.hpp"

class ACamera :
    public AActor
{
public:
    ACamera();
    virtual ~ACamera();

    virtual void Update() override;

protected:
    vec2 m_LastMousePosition;

    float m_Theta = 0.f; // Left & Right
    float m_Phi = 0.f; // Up & Down

    float m_AspectRatio = 16/9;
    float m_FOVDegrees = 90.f;
    float m_Near = 0.01f;
    float m_Far = 1000.f;

    mat4 m_ViewTransform;
    mat4 m_ProjectionViewTransform;

// === Getter Functions ===
public:
    mat4 GetViewMatrix() { return m_ViewTransform; }
    mat4 GetProjectionMatrix() { return m_ProjectionViewTransform; }
    mat4 GetProjectionViewMatrix() { return m_ProjectionViewTransform; }
    void SetViewMatrix();
    void UpdateProjectionMatrix();
    void SetProjectionMatrix(float a_FOVDegrees, float a_AspectRatio, float a_Near, float a_Far);
    void SetProjectionMatrix(float a_FOVDegrees, float a_Width, float a_Height, float a_Near, float a_Far);

    float GetFOV() { return m_FOVDegrees; }
    void  SetFOV(float a_FOVDegrees) { m_FOVDegrees = a_FOVDegrees; UpdateProjectionMatrix(); }

    float GetAspectRatio() { return m_AspectRatio; }
    void SetAspectRatio(float a_AspectRatio);
    void SetAspectRatio(float a_Width, float a_Height);


#pragma region ImGui

protected:
    virtual void OnDraw_ImGui();

#pragma endregion
};

