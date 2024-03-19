#pragma once

#include "glm/glm.hpp"
#include "Renderer2D.h"

enum CollisionType
{
    TRIGGER = 0,
    DYNAMIC,
    STATIC
};

enum ShapeType
{
    PLANE = 0,
    SPHERE,
    BOX,
    SHAPE_COUNT
};

class PhysicsObject
{
protected:
    PhysicsObject() {};
    PhysicsObject(ShapeType a_ShapeType);

public:
    virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    virtual void Draw(float alpha) = 0;
    virtual void ResetPosition() {};

public:
    CollisionType m_CollisionType = DYNAMIC;
    ShapeType m_ShapeType;
};

