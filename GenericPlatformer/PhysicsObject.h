#pragma once
#include "Component.h"

#define DEFAULT_COLOUR vec4(1,0,0,1)

class PhysicsManager;

class PhysicsObject :
    public Component
{
protected:
    PhysicsObject();
public:
    PhysicsObject(Transform* a_Owner, vec4 a_Colour = DEFAULT_COLOUR);
    virtual ~PhysicsObject();

    virtual void Update(float DeltaTime) = 0;
    virtual void Draw(float a_Alpha) = 0;

    void CalculateAxis();

    vec2  GetLocalX() { return m_LocalX; }
    vec2  GetLocalY() { return m_LocalY; }

    vec4 GetColour() { return m_Colour; }
    void SetColour(vec4 a_Colour) { m_Colour = a_Colour; }

protected:
    PhysicsManager* m_PhysicsManager;

    vec4 m_Colour;

    vec2 m_LocalX;
    vec2 m_LocalY;
};

