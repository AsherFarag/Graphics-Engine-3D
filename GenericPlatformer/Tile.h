#pragma once
#include "Transform.h"
#include "BoxRB.h"
class Tile :
    public Transform
{
public:
    Tile(vec2 a_Pos, vec2 a_HalfExtents, vec4 a_Colour, RigidBody::ECollisionType a_CollisionType = RigidBody::STATIC);
    virtual ~Tile() override {}

    BoxRB* GetBoxRB() { return m_BoxRB; }

protected:
    virtual void Init(vec2 a_HalfExtents, vec4 a_Colour, RigidBody::ECollisionType a_CollisionType);

    BoxRB* m_BoxRB;
};

