#pragma once
#include "RigidBody.h"
class Sphere :
    public RigidBody
{
public:
    Sphere();
    Sphere(glm::vec2 a_Position, glm::vec2 a_Velocity,
        float a_Mass, float a_Radius, glm::vec4 a_Colour, float a_Elasticity);
    ~Sphere() {};

    virtual void Draw(float alpha) override;

    float GetRadius() { return m_Radius; }
    glm::vec4 GetColour() { return m_Colour; }

    virtual float GetMoment() override
    {
        m_Moment = 0.5f * m_Mass * m_Radius * m_Radius;
        return m_Moment;
    }

protected:
    float m_Radius;
    glm::vec4 m_Colour;
};

