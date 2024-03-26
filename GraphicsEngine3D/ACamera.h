#pragma once
#include "AActor.h"

// --- STD ---
using std::vector;
using std::list;

// --- AIE ---
#include "RenderTarget.h"

// --- GLM ---
#include "glm/trigonometric.hpp"

// --- Engine ---
#include "URenderer.h"
#include "UMeshRenderer.h"

class ACamera :
    public AActor
{
public:
    ACamera();
    virtual ~ACamera();

    friend class RenderingManager;

    virtual void Update() override;

protected:
    vec2 m_LastMousePosition;

    float m_Theta = 0.f; // Left & Right
    float m_Phi = 0.f; // Up & Down

    float m_AspectRatio = 16 / 9;
    float m_FOVDegrees = 90.f;
    float m_Near = 0.001f;
    float m_Far = 1000.f;

    mat4 m_ViewTransform;
    mat4 m_ProjectionViewTransform;

    aie::RenderTarget* m_RenderTarget = nullptr;

    bool m_UsePostProcessing = false;

protected:
    void BeginRender();
    void Render(ALight* a_AmbientLight, vector<ALight*>* a_Lights, list< UMeshRenderer* >* a_Meshes);
    void EndRender();

    void ClearScreen();

    void ApplyPostProcessing();
    float m_PostProcessPercent = 0.f;
    aie::ShaderProgram* m_PostProcessShader;
    RMesh m_PostProcessQuad;

#pragma region Getters & Setters
public:
    mat4 GetViewMatrix() { return m_ViewTransform; }
    mat4 GetProjectionMatrix() { return m_ProjectionViewTransform; }
    mat4 GetProjectionViewMatrix() { return m_ProjectionViewTransform; }

    void SetViewMatrix();
    void SetProjectionMatrix(float a_FOVDegrees, float a_AspectRatio, float a_Near, float a_Far);
    void SetProjectionMatrix(float a_FOVDegrees, float a_Width, float a_Height, float a_Near, float a_Far);

    void UpdateProjectionMatrix();

    float GetFOV() { return m_FOVDegrees; }
    void  SetFOV(float a_FOVDegrees) { m_FOVDegrees = a_FOVDegrees; UpdateProjectionMatrix(); }

    float GetAspectRatio() { return m_AspectRatio; }
    void SetAspectRatio(float a_AspectRatio);
    void SetAspectRatio(float a_Width, float a_Height);

    vec3 GetForward();

    aie::RenderTarget* GetRenderTarget() { return m_RenderTarget; }
    void SetRenderTarget(aie::RenderTarget* a_RenderTarget) { m_RenderTarget = a_RenderTarget; }

    bool IsUsingPostProcessing() const { return m_UsePostProcessing; }
    void UsePostProcessing(bool Use) { m_UsePostProcessing = Use; }
#pragma endregion

#pragma region ImGui

protected:
    virtual void OnDraw_ImGui();

#pragma endregion
};

