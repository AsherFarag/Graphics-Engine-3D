#pragma once

#include "Tile.h"

class GoalPoint :
    public Tile
{
public:
    GoalPoint(vec2 a_Pos, vec2 a_HalfExtents, vec4 a_Colour = vec4(0, 1, 0, 1));
    virtual ~GoalPoint() override {};

protected:
    void OnEnter(RigidBody* a_RB);
};

