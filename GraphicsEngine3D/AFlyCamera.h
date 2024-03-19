#pragma once
#include "ACamera.h"
class AFlyCamera :
    public ACamera
{
public:
    AFlyCamera();
    virtual ~AFlyCamera();

    virtual void Update() override;

public:

    // Position Move Speed
    float m_ForwardMoveSpeed = 1.f;
    float m_StrafeMoveSpeed = 1.f;
    float m_ElevationMoveSpeed = 1.f;

    float m_MoveSpeedMultiplier = 1.f;
    float m_MaxMoveSpeedMultiplier = 100.f;

    float m_LookSensitivity = 100.f;
    float m_MouseScrollSensitivity = 0.15f;
    float m_LastMouseScroll = 0.f;

#pragma region ImGui

protected:
    virtual void OnDraw_ImGui();

#pragma endregion
};

