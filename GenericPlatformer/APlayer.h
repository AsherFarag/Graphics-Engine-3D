#pragma once
#include "AActor.h"
#include "Input.h"

class CircleRB;
class Renderer;

class APlayer :
    public AActor
{
public:
    APlayer(vec2 a_Pos = vec2(), float a_Radius = 5.f, vec4 a_Colour = vec4(0,0,1,1), float a_Mass = 10.f);
    virtual ~APlayer() override;

    virtual void Update(float DeltaTime) override;
    void Draw();

    int GetTimesShot() const { return m_TimesShot; }
    void SetTimesShot(int a_TimesShot) { m_TimesShot = a_TimesShot; }

protected:
    vec2 GetTrajectoryPoint(vec2& a_StartingPos, vec2& a_StartingVelocity, float n);

protected:
    aie::Input* m_Input;

    bool  m_CanHold = false;
    bool  m_IsHeld = false;
    int   m_TimesShot = 0;
    float m_ShootPowerMultiplier = 1.f;
    float m_MaxShootPower = 250000.f;
    vec2  m_ClickPosition;
    vec2  m_MousePosition;

    // These variables are only used for drawing the Projectile Trajectory
    vec2 m_ShootPosition;
    vec2 m_ShootVelocity;

    vec4  m_BaseColour;
    vec4  m_HeldColour = vec4(1);


    CircleRB* m_RB;
    Renderer* m_Renderer;
};

